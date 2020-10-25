/*
 * MQTTClient.h
 *
 *  Created on: Apr 12, 2019
 *      Author: giangnv
 */

#ifndef MAIN_NETWORK_MQTTCLIENT_H_
#define MAIN_NETWORK_MQTTCLIENT_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "mqtt_client.h"

// #include "../app.h"
//
#define MQTT_SUB_TOPIC_LOCK_FORMAT   				"%s/LOCK"
#define MQTT_SUB_TOPIC_UV_FORMAT   					"%s/LIGHT"
//
//
////#define MQTT_SUB_TOPIC_RESP_FOMAT					"VSM/DMP/VSMART/SMH/%s/telemetryACK"
//
#define MQTT_PUB_TOPIC_RESP_STATUS					"%s/STATUS"
//
#define INDEX_PUB_TOPIC_RESP_STATUS			(0)
//
#define INDEX_SUB_TOPIC_LOCK_FORMAT			(0)
#define INDEX_SUB_TOPIC_UV_FORMAT			(1)
//#define INDEX_SUB_TOPIC_COMMON				(0)		// mqtt sub chung cung 1 topic
//
//
//#define NUM_PUB_TOPIC						(5)
//
////define mode
//
//#define MQTT_CONN_NONE_SSL					(0)
//#define MQTT_CONN_SSL						(1)
//
//
//

//
#define SIZE_TOPIC_MQTT							(64)
#define SIZE_USER_PASS_MQTT						(64)
//
#define SIZE_BROKER_MQTT						(64)
#define SIZE_CLIENT_ID_MQTT						(16)		//mac wifi
//
#define subtopic_max			5
//#define MQTT_DATA_LEN_MAX		(1024)
//
//
//
//
#define MQTT_DATA_LENGTH_MAX			1024
//
//
//
typedef void (*mqtt_messagehandle)(char* /*msg_rev*/, uint16_t /*len*/, uint8_t /*index_topic*/);

typedef struct
{
	char subtopic[SIZE_TOPIC_MQTT];
	mqtt_messagehandle mqtt_rev_message;
}list_subtopic_t;


typedef struct
{
	char						broker[SIZE_BROKER_MQTT + 1];
	int						    port;
	char						client_ID[SIZE_CLIENT_ID_MQTT + 1];
	list_subtopic_t				list_topic[subtopic_max];
	char 						pubtopic[SIZE_TOPIC_MQTT + 1];
	char 						username[SIZE_USER_PASS_MQTT + 1];
	char						password[SIZE_USER_PASS_MQTT + 1];
	// uint8_t						keepalive_s;
	// uint8_t						mode;
	void (*publish)(char* /*topic*/, char* /*msg*/);
	// void (*received)(char* /*msg*/,  uint16_t /*len*/, uint8_t /*index_topic*/);
}MQTTClient_Config_t;


void mqtt_register_status(void status_cb(uint32_t/*stt*/));


void mqtt_app_start(void);

/*******************************************************************************
 * Function		: void vMQTTClient_Start(void)
 * Description	: mqtt client start
 * Param		: None
 * Return		: None
 ******************************************************************************/





/*******************************************************************************
 * Function		: void vMQTTClient_Stop(void)
 * Description	: mqtt client stop
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Stop(void);



/*******************************************************************************

 ******************************************************************************/


void MQTT_Register_RxCallBack(void rxcb(char* /*msg*/,  uint16_t /*len*/, uint8_t /*index_topic*/));



/*******************************************************************************
for user
 ******************************************************************************/

void Mqtt_SendData(unsigned char* data_send, unsigned char data_len);

#endif /* MAIN_NETWORK_MQTTCLIENT_H_ */
















