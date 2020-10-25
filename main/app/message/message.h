/*
 * message.h
 *
 *  Created on: Apr 4, 2020
 *      Author: giang
 */

#ifndef MESSAGE_MESSAGE_H_
#define MESSAGE_MESSAGE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../app.h"
#include "../device_info/device_info.h"
#include "server_message.h"
#include "../app_debug.h"

// typedef enum
// {
// 	OK		= 0x01,
// 	NOK		= 0x00,
// }result_t;

extern void (*pfnMessage_SetEvent)(uint32_t	/*event*/);



void message_register_sendtoServer_CallBack(bool txcb(char*/*topic*/, char*/*data*/));



void message_register_setevent(void set_event(uint32_t));


void message_received_fromServer(char* msg, uint16_t len, uint8_t __fromtopic);



void Message_ShowInfo(device_data_t* __device_data);

#endif /* MESSAGE_MESSAGE_H_ */
