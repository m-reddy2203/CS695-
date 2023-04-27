#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"
#include "core/lib.h"
#include "core/struct-device.h"


struct device dev;

long count, init_t;

int main ()
{	
    config(&dev);			
    initPeripherals(&count);	
    connectNetwork(&dev, true);	

    while (1)
    {	
	    init_t = take_time();

	    pnp_sensors();		

	    getData(&dev, &count);

	    generateJson(&dev);


	    t_delay(dev.interv, take_time() - init_t);  /* compensated delay */
    }    
    return 0;
}
