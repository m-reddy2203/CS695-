#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "struct-device.h"
#include "config.h"
#include "devices.h"


void config(struct device *z)
{   
    /* User assignments */
    z->id = id_name;
        
    z->addr = address;
    z->addr_port = port;

    #ifdef MQTT
    	z->user_mqtt = user;
	    z->pass_mqtt = password;
	    z->top = topic;
    #endif

    #ifdef MICROCONTROLLER
        z->ssid_wifi = ssid_WiFi;
        z->pass_wifi = pass_WiFi;
    #endif

    z->isEnable[0] = isEnable_VoltageIntern;
    z->isEnable[1] = isEnable_TemperatureExtern;
    z->isEnable[2] = isEnable_Humidity;
    z->isEnable[3] = isEnable_Pressure;
    
    z->s_name[0] = "InternalVoltage"; 
    z->s_name[1] = "Temperature";
    z->s_name[2] = "Humidity";
    z->s_name[3] = "Pressure";


    z->interv = interval;

}

void initPeripherals(long* c) 
{
    *c = 0;		// Init counter
    
    #ifdef SHELLPRINT
    	welcome_msg();	// Printf in shell
    #endif
	
    init_LEDs();
    // init_i2c(); // Not necesary in BBB
	
	init_internal(true);
    init_bme280(true);
   


void led_blinks(int led, int iter, int usec)	// LED Blink function-> led: 0 Green LED, 1 Red LED - iter: iterations quantity - usec: delay time in usec
{	
     int i;
     for (i=0;i<iter;i++)
     {
		 led_GPIO(led, 1);
		 udelay_basics (usec);
		 led_GPIO(led, 0);
		 udelay_basics (usec);
     }
}

void connectNetwork(struct device *z, bool first_t)
{	
	#ifdef MICROCONTROLLER			
	if (first_t)
	{				
		while ( !connectAttempt(z->ssid_wifi, z->pass_wifi) )    /* Attempt to connect to the network via WiFi, in RaspberryPi only check connection to the network. */
		{
			 led_blinks(0, 1, 600000);	// Blink in green GREEN - ERROR 0 (No WiFi connection);
			 led_blinks(1, 1, 600000);	// Blink in green RED - ERROR 0 (No WiFi connection);
		}
	}
	#endif
	if ( !init_socket(z->addr, z->addr_port, z->user_mqtt, z->pass_mqtt, first_t) )     /* Check Endpoint */
	{	
		udelay_basics ( 100000 );
		led_blinks(1, 3, 70000);	// Blink in green RED - ERROR 1 (Bad connection with the endpoint);
		#ifdef MICROCONTROLLER
			restart_basic();
		#endif
	}
}

void pnp_sensors()
{
    init_bme280(false);
    
}


void getData(struct device *z, long *c)
{
    int i;
    ++(*c);
	
    #ifdef SHELLPRINT	// Printf in shell
	d_collect_msg( c );
	print_sensors_state();
    #endif  

	
    /* GET DATA INTERNAL TEMPERATURE */
    strcpy(z->d[0], get_internal());
	
    /* GET DATA BME280 */
    if (check_bme280())
    {
		for (i=0; i<3; i++)
	   	    strcpy(z->d[i+1], get_bme280(i));
    }
    else
    {
		for (i=0; i<3; i++)
			strcpy(z->d[i+1], "0");
    }

   
}


void generateJson(struct device *z)
{
    int i, aux;

    strcpy(z->json, "{\"iot2tangle\":[");
    
    aux = 0;
    strcat(z->json, "{\"sensor\":\"Internal\",\"data\":[");
    for (i=0;i<1;i++)
    {
		if (z->isEnable[i+0])
		{
			if (aux != i) strcat(z->json, ",");
			strcat(z->json, "{\"");
			strcat(z->json, z->s_name[i+0]);
			strcat(z->json, "\":\"");
			strcat(z->json, z->d[i+0]);
			strcat(z->json, "\"}");
		}
	else
	    aux++;
    }
    strcat(z->json, "]}");
	
    if (check_bme280())
    {
		aux = 0;
		strcat(z->json, ",{\"sensor\":\"Environmental\",\"data\":[");
		for (i=0;i<3;i++)
		{
			if (z->isEnable[i+1])
			{
				if (aux != i) strcat(z->json, ",");
				strcat(z->json, "{\"");
				strcat(z->json, z->s_name[i+1]);
				strcat(z->json, "\":\"");
				strcat(z->json, z->d[i+1]);
				strcat(z->json, "\"}");
			}
			else
			aux++;
		}
		strcat(z->json, "]}");
    }
		 
	strcat(z->json, "],\"device\": \"");
	strcat(z->json, z->id);
	strcat(z->json, "\",\"timestamp\": \"0\"}");
	
    #ifdef SHELLPRINT	
    	print_json(z->json);	// Printf in shell
    #endif
}

bool sendtoEndpoint(struct device *z)
{
    bool b_socket = socket_sender(z->addr, z->addr_port, z->top, z->user_mqtt, z->pass_mqtt, z->json, z->interv);
    if (b_socket)
	 	led_blinks(0, 2, 60000);	// Blink in green LED;
    else
		led_blinks(1, 3, 70000);	// Blink in green RED - ERROR 1 (Bad connection with the endpoint);

    return b_socket;
}

void t_delay(long d, long l) 
{  
	if (l >= d)		/* To prevent crashes */
		l = d;
    udelay_basics ( (d - l) * 1000000 );	/* Time set by user  minus  loss time by operation */ 
}

long take_time() 
{  
    return take_time_basics();
}


