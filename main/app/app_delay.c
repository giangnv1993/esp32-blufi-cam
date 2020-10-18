/*
 * delay_ms_partical.c
 *
 *  Created on: Mar 26, 2020
 *      Author: Thao
 */
#include "app_delay.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../Interface/UserTimer.h"

#define TIMER_MS_TIME_BASE					1//ms



uint32_t delay_ms_table[DELAY_PARTICAL_MAX] = {0};
bool delay_start[DELAY_PARTICAL_MAX] = {false};



static void delay_ms_partical_timer_1ms_callback( void )
{
	for(uint8_t index = 0; index < DELAY_PARTICAL_MAX; index++)
	{
		if(delay_start[index] == true)
			delay_ms_table[index]++;
	}
}



//delay init
void app_delay_init( void )
{
	Timer_RegisterCallBack(delay_ms_partical_timer_1ms_callback);
}



//delay register. TRUE - delay finish, FALSE - waitting
bool bDelay_ms( delay_ms_list_t delay_id, uint32_t waittime_ms )
{
	delay_start[delay_id] = true;

	if(delay_ms_table[delay_id]* TIMER_MS_TIME_BASE >= waittime_ms)
	{
		delay_start[delay_id] = false;
		delay_ms_table[delay_id] = 0;
		return true;
	}
	return false;
}













