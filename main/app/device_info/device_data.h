/*
 * device_info.h
 *
 *  Created on: Aug 25, 2020
 *      Author: admin
 */


#include <stdlib.h>
#include <stdint.h>


typedef struct
{
   uint8_t lock;
   uint8_t light_uv;
   uint8_t time_on_uv;      /*min*/ 
}device_data_t;
