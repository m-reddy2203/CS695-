#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "internal.h"

char buffer[400];
char *s;
float num;
FILE *fptr; 

void init_internal(bool ft)
{
		// Not necessary in Raspberry Pi
}

char* get_internal() 
{
    s = "";	

    if ((fptr = fopen("/sys/bus/iio/devices/iio:device0/in_voltage1_raw","r")) == NULL)
	return "NULL";
    
    fscanf(fptr,"%f", &num);
    fclose(fptr);
    sprintf(buffer, "%.2f", (num / 1000) );	
    s = buffer;
    return s;    
}
