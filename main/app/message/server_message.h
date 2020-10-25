/*
 * message.h
 *
 *  Created on: Mar 26, 2020
 *      Author: giangnv
 */

#ifndef APP_MESSAGE_MESSAGE_H_
#define APP_MESSAGE_MESSAGE_H_
#include <stdlib.h>
#include <stdint.h>
//#include <device_info.h>



#define JSON_KEY_DEVICE_TOKEN			"deviceToken"

//define key msg json
#define JSON_KEY_OBJ_LIGHT                      "Light"
#define JSON_KEY_OBJ_LOCK                       "Lock"


#define JSON_CMD_NAME_DATA					"data"
#define JSON_CMD_NAME_CONF					"conf"
#define JSON_CMD_NAME_REQS					"reqs"
#define JSON_CMD_NAME_REFRESH				"refresh"
#define JSON_CMD_NAME_OTA					"ota"
#define JSON_CMD_NAME_GET_INFO				"info"





//define  key in data field msg config

#define JSON_CONF_NUM_OBJ					(2)
#define JSON_CONF_SIZE_OBJ					(16)



//define msg OTA

#define JSON_OTA_NUM_OBJ					(3)
#define JSON_OTA_SIZE_OBJ					(16)




void Server_Message_Parser(char* data_rx, uint16_t len_rx, uint8_t __fromTopic);


// char* server_packagedata(message_t* __msg);




#endif /* APP_MESSAGE_MESSAGE_H_ */





















