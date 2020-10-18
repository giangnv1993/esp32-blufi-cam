/*
 * ACInf_Task.c
 *
 *  Created on: Mar 7, 2019
 *      Author: thaonm1
 */
#include "../main.h"
#include "../ACInterface/ACInf.h"


void ACInf_Task(void *pvParameters)
{
	vTaskDelay(1000);
	printf("ACInf task init\r\n");

	while(1)
	{
		ACInf_Process();
		vTaskDelay(1);
	}
}





