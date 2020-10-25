/*
 * message.c
 *
 *  Created on: Mar 26, 2020
 *      Author: Thao
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "message.h"
#include "server_message.h"

#include "cJSON.h"




// extern char pub_topic[NUM_PUB_TOPIC][SIZE_TOPIC_MQTT + 1];


char list_obj_conf[JSON_CONF_NUM_OBJ][JSON_CONF_SIZE_OBJ] =
{
	"status",
	"timer"	 
};



extern AppConfig_t AppConfig;

extern device_data_t device_data;

extern Flag_t xFlag;


/*muc tieu la xu ly json lay data, roi goi ham mainboard_parser*/


static void Server_Message_Handle_Light(cJSON* __root);


static void Server_Message_Handle_Lock(cJSON* __root);

static char* server_packagedata(device_data_t* __msg);


void Server_Message_Parser(char* data_rx, uint16_t len_rx, uint8_t __fromTopic)
{
	APP_DEBUG("msg_sv =  %s\r\n", data_rx);
	//paser json
	cJSON* m_root = cJSON_Parse(data_rx);
	if(__fromTopic == INDEX_SUB_TOPIC_UV_FORMAT)				// topic light
	{
		Server_Message_Handle_Light(m_root);
	}
	else if(__fromTopic == INDEX_SUB_TOPIC_LOCK_FORMAT)			//  from topic command
	{
		Server_Message_Handle_Lock(m_root);
	}

	cJSON_Delete(m_root);
	memset(data_rx, 0x00, len_rx);
}



static void Server_Message_Handle_Light(cJSON* __root)
{
	APP_DEBUG("rev msg config from server....\r\n");
	//get Data
	cJSON* j_value	= cJSON_GetObjectItem(__root, (char*)JSON_KEY_OBJ_LIGHT);
	int j_size = cJSON_GetArraySize(j_value);
	if(j_size != JSON_CONF_NUM_OBJ)
	{
		APP_DEBUG("not enough to object\r\n");
		return;
	}
	for(uint8_t ui = 0; ui < j_size; ui ++)
	{
//		get object
		if(strlen(list_obj_conf[ui]) == 0)
			break;
		cJSON* obj = cJSON_GetObjectItem(j_value, list_obj_conf[ui]);
		if(obj && cJSON_IsNumber(obj))
		{
			if(strcmp(list_obj_conf[ui], (char*)"status") == 0)
				device_data.light_uv = (uint8_t)obj->valueint;
			else if(strcmp(list_obj_conf[ui], (char*)"timer") == 0)
				device_data.time_on_uv = (uint8_t)obj->valueint;
			else
			{
				APP_DEBUG("object config not support\r\n");
				return;
			}
		}
		else
		{
			APP_DEBUG("value of object config format incorrect\r\n");
			return;
		}
	}
	//set msg and send to mainboard
	Message_ShowInfo(&device_data);
	if(pfnMessage_SetEvent != NULL)
		pfnMessage_SetEvent(EVT_SET_LIGHT_ON);
}



static void Server_Message_Handle_Lock(cJSON* __root)
{
	APP_DEBUG("rev msg config lock from server....\r\n");
	//get Data
	cJSON* j_value	= cJSON_GetObjectItem(__root, (char*)JSON_KEY_OBJ_LOCK);
	int j_size = cJSON_GetArraySize(j_value);
	if(j_size != JSON_CONF_NUM_OBJ - 1)
	{
		APP_DEBUG("not enough to object\r\n");
		return;
	}
	cJSON* obj = cJSON_GetObjectItem(j_value, list_obj_conf[0]);
	if(obj && cJSON_IsNumber(obj))
	{
		if(strcmp(list_obj_conf[0], (char*)"status") == 0)
			device_data.lock = (uint8_t)obj->valueint;
		else
		{
			APP_DEBUG("object config not support\r\n");
			return;
		}
	}
	else
	{
		APP_DEBUG("value of object config format incorrect\r\n");
		return;
	}
	//set msg and send to mainboard
	Message_ShowInfo(&device_data);
	if(pfnMessage_SetEvent != NULL)
		pfnMessage_SetEvent(EVT_SET_LOCK_ON);

	char* msg_publish = server_packagedata(&device_data);
	AppConfig.MQTT_Config.publish(AppConfig.MQTT_Config.pubtopic, msg_publish);
	free(msg_publish);
	device_data.lock = 0;
}



/*
{
	"lock":{"status": $value},
}
*/

static char* server_packagedata(device_data_t* __msg)
{
	char* json_msg = (char*)calloc(SERVER_MSG_LEN, sizeof(char));	//free in function call it

	sprintf(json_msg, "{\"lock\": {\"status\",: %d}}",__msg->lock);
	APP_DEBUG("msg data send to server: \r\n");
	APP_DEBUG("%s\r\n", json_msg);
	return json_msg;
}












