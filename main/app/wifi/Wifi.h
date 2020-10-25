/*
 * Wifi.h
 *
 *  Created on: Apr 2, 2019
 *      Author: giangnv3
 */

#ifndef SOURCE_WIFI_WIFI_H_
#define SOURCE_WIFI_WIFI_H_

#include "../../main.h"
#include "wifi.h"

#define NET_IF_NUM ((CONFIG_ETHERNET) + (CONFIG_WLAN) + 1)


// typedef struct
// {
// 	char UserName[32];
// 	char Password[64];
// }wifi_info_t;


typedef enum {
	WF_MODE_NONE = 0,
	WF_MODE_STA,
	WF_MODE_AP,
}Wifi_Mode_t;

void Wifi_Init(Wifi_Mode_t mode);

void Wifi_DeInit(void);


//void Wifi_DeInit(void);
/******************************************************************************
 *
 * ****************************************************************************/
void Wifi_Register_StatusCallBack(void stt_cb(uint32_t));


#endif /* SOURCE_WIFI_WIFI_H_ */
























