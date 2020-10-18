/*
 * delay_ms_partical.h
 *
 *  Created on: Mar 26, 2020
 *      Author: Thao
 */

#ifndef DRIVERS_DELAY_MS_PARTICAL_H_
#define DRIVERS_DELAY_MS_PARTICAL_H_


#include <stdbool.h>
#include <stdint.h>

typedef enum
{
	DELAY_DEBUG1,
	DELAY_DEBUG2,
	GPIO_LIGH,
	DELAY_PARTICAL_MAX,
}delay_ms_list_t;


//delay init
void app_delay_init( void );


//delay register. true - delay finish, false - waitting
bool bDelay_ms( delay_ms_list_t delay_id, uint32_t waittime_ms );



#endif /* DRIVERS_DELAY_MS_PARTICAL_H_ */






