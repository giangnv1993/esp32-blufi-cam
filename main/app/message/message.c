/*
 * message.c
 *
 *  Created on: Apr 4, 2020
 *      Author: giangnv
 */

#ifndef MESSAGE_MESSAGE_C_
#define MESSAGE_MESSAGE_C_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "message.h"
// #include "crc.h"
#include "../app.h"
// #include <MQTT.h>

// extern char pub_topic[NUM_PUB_TOPIC][SIZE_TOPIC_MQTT + 1];
extern Flag_t xFlag;

bool (*pfnMessage_SendtoServer)(char*/*topic*/, char*/*data*/);

void (*pfnMessage_SetEvent)(uint32_t	/*event*/);



void message_register_sendtoServer_CallBack(bool txcb(char*/*topic*/, char*/*data*/) )
{
	if(txcb != NULL)
		pfnMessage_SendtoServer = txcb;
}


void message_register_setevent(void setEvt_cb(uint32_t))
{
	if(setEvt_cb != NULL)
		pfnMessage_SetEvent = setEvt_cb;
}




//rx callback
void message_received_fromServer(char* msg, uint16_t len, uint8_t __fromtopic)
{
	Server_Message_Parser(msg, len, __fromtopic);
}


void Message_ShowInfo(device_data_t* __device_data)
{
	printf("\r\n==========================\r\n");
	printf("---- message show info\r\n");

	printf("---- light status = %s\r\n", __device_data->light_uv == 1?"on":"off");
	printf("---- light on time = %d\r\n", __device_data->time_on_uv);
	printf("---- lock status current = %s\r\n", __device_data->lock == 1?"on":"off");
}



#endif /* MESSAGE_MESSAGE_C_ */













