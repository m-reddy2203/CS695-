#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#include <unistd.h>
#include "basics.h"
#include "sensors/bme280/bme280.h"


char buffer[100];
char* s;

void welcome_msg()
{
    printf("\n					----  BeagleBone Black -- APACHE SERVER  --");
    
    #ifdef HTTP	
    	printf("  HTTP  ----\n\n");	// Printf in shell
    #endif
   
} 

void d_collect_msg(long* n)
{
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf ("Data collect - %ld\n", *n);
}

void print_sensors_state()
{
	printf ("   Sensors Detection:  ||	");
	print_bme280();
	
	
}

void print_json(char* js)
{
    printf("\nJSON: %s\n", js);
    printf("\n	Sending Data to Apache...\n");
}

void udelay_basics (long d)
{
    usleep(d);
}

long take_time_basics()
{
    time_t t;
    time(&t);
    return t;
}
