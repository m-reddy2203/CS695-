#ifndef _CONFIG_H
#define _CONFIG_H

/* ----- CONFIG FILE ----- */

/* Device */
const char* id_name = "BeagleBoneB-HTTP";

/* Enable Sensors */
bool isEnable_VoltageIntern = true;
bool isEnable_TemperatureExtern = true;	/*                     true: Enable  --  false: Disable                            */
bool isEnable_Humidity = true;		
bool isEnable_Pressure = true;


/* Interval of time */
long interval = 10;    /* Time in seconds between */


#endif
