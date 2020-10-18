/*
 * smartconf_task.h
 *
 *  Created on: Mar 8, 2019
 *      Author: giangnv3
 */

#ifndef MAIN_SMART_CONFIG_SMARTCONF_TASK_H_
#define MAIN_SMART_CONFIG_SMARTCONF_TASK_H_
#include "../main.h"


void SmartConf_Task(void *pvParameters);

void SmartConf_start(void);

void SmartConf_stop(void);

void SmartConf_EventStart( void );


void (*SmartConf_finish)( void );


#endif /* MAIN_SMART_CONFIG_SMARTCONF_TASK_H_ */
