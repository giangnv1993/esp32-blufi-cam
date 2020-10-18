/*
 * ProcessTask.c
 *
 *  Created on: Sep 18, 2020
 *      Author: admin
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>


#include "../main.h"

#include "ProcessTask.h"
#include "../app/app.h"


static void ProcessTask(void *pvParameters);



static void ProcessTask_EventHandler(void);

static void ProcessTask_Wifi_EventHandler(void);

static void ProcessTask_MQTT_EventHandler(void);

static void ProcessTask_DataHandler(void);

static void ProcessTask_SmartConfig(void);

static void ProcessTask_OTA_Handler(void);

void ProcessTask(void *pvParameters);


uint32_t ProcessEvent = (uint32_t)EVT_PROCESS_IDLE;

extern Flag_t xFlag;


extern wifi_info_t _WifiInfo;


void ProcessTask_Init(void)
{
	xTaskCreate(ProcessTask, "ProcessTask", 6*1024, NULL, 10, NULL);
}

static void ProcessTask(void *pvParameters)
{
	vTaskDelay(10);

	while(1)
	{
		ProcessTask_EventHandler();
		if(bDelay_ms(DELAY_DEBUG1, 1000) != false)
			APP_DEBUG("running....\r\n");

		if( (ProcessEvent & EVT_SET_LIGHT_ON) == EVT_SET_LIGHT_ON )
		{
			GPIO_Set_Pin_High(GPIO_OUTPUT_LIGHT);
			vTaskDelay(300);
			GPIO_Set_Pin_Low(GPIO_OUTPUT_LIGHT);
			printf("set pin light high\r\n");
			ProcessEvent ^= EVT_SET_LIGHT_ON;
		}

		if( (ProcessEvent & EVT_SET_LOCK_ON) == EVT_SET_LOCK_ON )
		{
			GPIO_Set_Pin_High(GPIO_OUTPUT_LOCK);
			if(bDelay_ms(GPIO_LIGH, 60) != false)
			{
				printf("set pin lock back down\r\n");
				GPIO_Set_Pin_Low(GPIO_OUTPUT_LOCK);
				ProcessEvent ^= EVT_SET_LOCK_ON;
			}
		}

		vTaskDelay(5);
	}
}




void ProcessTask_SetEvent(uint32_t event)
{
	ProcessEvent |= event;
}


static void ProcessTask_EventHandler(void)
{
	ProcessTask_Wifi_EventHandler();
	ProcessTask_MQTT_EventHandler();
	ProcessTask_SmartConfig();
}

//for development
static void ProcessTask_OTA_Handler(void)
{
//	if( (ProcessEvent & EVT_OTA_START) == EVT_OTA_START )
//	{
//		xFlag.wifi_connected = false;
//		xFlag.mqtt_conn = false;
//		xFlag.do_smartcfg = true;	//inform to process task about doing smart config
//		Ota_Start();
//		ProcessEvent ^= EVT_OTA_START;
//	}
}

int mqtt_running = 0;

static void ProcessTask_Wifi_EventHandler(void)
{
	if( (ProcessEvent & EVT_PROCESS_IDLE) == EVT_PROCESS_IDLE )
	{
		ProcessEvent ^= EVT_PROCESS_IDLE;
	}

//	if( (ProcessEvent & EVT_WIFI_START) == EVT_WIFI_START )
//	{
//		VSM_DEBUG("wifi try connect to %s\r\n", AppConfig.Wifi_STA_Config.ssid);
//		if(Wifi_Connect_toAP((uint8_t*)AppConfig.Wifi_STA_Config.ssid, (uint8_t*)AppConfig.Wifi_STA_Config.pass,
//						AppConfig.Wifi_STA_Config.wifi_security) == true)
//			ProcessTask_SetEvent(EVT_WIFI_CONNECTED);
//		else
//			ProcessTask_SetEvent(EVT_WIFI_CHECK_CON);
//		ProcessEvent ^= EVT_WIFI_START;
//	}

	if( (ProcessEvent & EVT_WIFI_CONNECTED) == EVT_WIFI_CONNECTED )
	{
		APP_DEBUG("wifi connected......\r\n");
		xFlag.wifi_connected = true;
		blufi_wifi_stop();
		ProcessTask_SetEvent(EVT_MQTT_START);
		ProcessEvent ^= EVT_WIFI_CONNECTED;
	}

	if( (ProcessEvent & EVT_WIFI_DISCONNECTED) == EVT_WIFI_DISCONNECTED )
	{
		APP_DEBUG("wifi disconnected......\r\n");
		xFlag.wifi_connected = false;
		xFlag.mqtt_conn = false;
		xFlag.send_status = 0;
		xFlag.scan_broker = true;
		ProcessEvent ^= EVT_WIFI_DISCONNECTED;
	}

	if( (ProcessEvent & EVT_WIFI_AVAILABLE) == EVT_WIFI_AVAILABLE )
	{
			if( (_WifiInfo.UserName[0] == 0) && (_WifiInfo.Password[0] == 0) )
			{
				APP_DEBUG("enter smart config\r\n");
				ProcessEvent |= EVT_BLUFI_START;
				ProcessEvent ^= EVT_WIFI_AVAILABLE;
			}
			else
				ProcessEvent ^= EVT_WIFI_AVAILABLE;
	}
}


static void ProcessTask_MQTT_EventHandler(void)
{
	if( (ProcessEvent & EVT_MQTT_START) == EVT_MQTT_START )
	{
		if(mqtt_running == 0)
		{
			mqtt_app_start();
			mqtt_running = 1;
		}
		ProcessEvent ^= EVT_MQTT_START;
	}


//	if( (ProcessEvent & EVT_MQTT_DISCONNECTED) == EVT_MQTT_DISCONNECTED )
//	{
//		VSM_DEBUG("Mqtt disconnect, check wifi....\r\n");
//		xFlag.scan_broker = true;
//
//		if(xFlag.wifi_connected == false)		//disconnect cause wifi
//			ProcessTask_SetEvent(EVT_MQTT_STOP);
//		else
//			ProcessTask_SetEvent(EVT_WIFI_CHECK_CON);		//check lai wifi conn
//		ProcessEvent ^= EVT_MQTT_DISCONNECTED;
//	}
//
//	if( (ProcessEvent & EVT_MQTT_STOP) == EVT_MQTT_STOP )
//	{
////		MQTT_Stop();
//		ProcessEvent ^= EVT_MQTT_STOP;
//	}
//
//	if( (ProcessEvent & EVT_MQTT_SCAN_BROKER) == EVT_MQTT_SCAN_BROKER )
//	{
//		memset(IP_Broker_local, 0x00, sizeof(IP_Broker_local));
//		broadcast_start();
//		ProcessTask_SetEvent(EVT_GET_IP_GATEWAY);
//		ProcessEvent ^= EVT_MQTT_SCAN_BROKER;
//	}

}



static void ProcessTask_DataHandler(void)
{
//	if( (ProcessEvent & EVT_DATA_MAINBOARD) == EVT_DATA_MAINBOARD )
//	{
//		if(len_data_mb > 0)
//		{
//			UART_CMD.ReceivedCB(data_mb, len_data_mb);
//			memset(data_mb, 0x00, sizeof(data_mb));
//			len_data_mb = 0;
//		}
//		ProcessEvent ^= EVT_DATA_MAINBOARD;
//	}


}

extern int state_wifi_init;
static void ProcessTask_SmartConfig(void)
{
	if( (ProcessEvent & EVT_BLUFI_START) == EVT_BLUFI_START )
	{
		APP_DEBUG("disable wifi, ble and mqtt....\r\n");
		xFlag.wifi_connected = false;
//		blufi_wifi_deinit();
		blufi_wifi_start();
		state_wifi_init = 1;
		//coding led blink
		ProcessEvent ^= EVT_BLUFI_START;
	}

//	if( (ProcessEvent & EVT_SMF_CHECK_TIMEOUT) == EVT_SMF_CHECK_TIMEOUT )
//	{
//		if( delay_ms_partical(DELAY_CHECK_TIMEOUT_SMF, 1000 ) != false)
//		{
//			timeout_smf_cnt ++;
//		}
//
//		if( (xFlag.do_smartcfg == false) || (timeout_smf_cnt >= TIMEOUT_SMF) )
//		{
//			timeout_smf_cnt = 0;
//			xFlag.smartcfg_timeout = true;
//			ProcessTask_SetEvent(EVT_BT_CONFIG_STOP);
//			ProcessEvent ^= EVT_SMF_CHECK_TIMEOUT;
//		}
//	}
//
//	if( (ProcessEvent & EVT_BT_CONFIG_RESULT) == EVT_BT_CONFIG_RESULT )
//	{
//		xFlag.wifi_connected = true;
//
//		VSM_DEBUG("Data after config: \r\n\r\n");
//		VSM_DEBUG("broker: %s\r\n", AppConfig.MQTT_Config.broker);
//		VSM_DEBUG("port: %d\r\n", AppConfig.MQTT_Config.port);
//		VSM_DEBUG("user: %s\r\n", AppConfig.MQTT_Config.username);
//		VSM_DEBUG("pass: %s\r\n", AppConfig.MQTT_Config.password);
//		VSM_DEBUG("mode: %d\r\n", AppConfig.MQTT_Config.mode);
//
//
//		//mode sw
//		if( (AppConfig.MQTT_Config.mode == MQTT_CONN_SW_SSL) ||(AppConfig.MQTT_Config.mode == MQTT_CONN_SW_NO_SSL) )
//		{
//			extern void MQTT_SetUp_SSL(void);
//			MQTT_SetUp_SSL();
//			xFlag.bt_config = true;
//			ProcessTask_SetEvent(EVT_MQTT_START);
//			ProcessTask_SetEvent(EVT_SAVE_DATA_CFG);
//		}
//		else		//local
//		{
//			VSM_DEBUG("find ip gw.....\r\n");
//			memset(IP_Broker_local, 0x00, sizeof(IP_Broker_local));
//			broadcast_start();
//			ProcessTask_SetEvent(EVT_GET_IP_GATEWAY);
//		}
//		ProcessEvent ^= EVT_BT_CONFIG_RESULT;
//	}
//
//	if( (ProcessEvent & EVT_GET_IP_GATEWAY) == EVT_GET_IP_GATEWAY )
//	{
//		if(xFlag.do_smartcfg == false)	// for MQTT reconnect
//		{
//			if(delay_ms_partical(DELAY_CHECK_FIND_GW, 2000 ) != false)
//			{
//				if(IP_Broker_local[0] != 0)
//				{
//					VSM_DEBUG("scan ip gw done.......\r\n");
//
//					extern void MQTT_SetUp_SSL(void);
//					MQTT_SetUp_SSL();
//
//					if(strcmp(AppConfig.MQTT_Config.broker, IP_Broker_local) != 0)	//neu khac IP ban dau thi save lai
//						ProcessTask_SetEvent(EVT_SAVE_DATA_CFG);
//
//					xFlag.scan_broker = false;	//ket thuc scan
//					broadcast_stop();
//					ProcessEvent ^= EVT_GET_IP_GATEWAY;
//				}
//			}
//
//			if(xFlag.wifi_connected == false)
//				ProcessEvent ^= EVT_GET_IP_GATEWAY;
//		}
//		else	//for smart config
//		{
//			if(delay_ms_partical(DELAY_CHECK_FIND_GW, 2000 ) != false)
//			{
//				if(IP_Broker_local[0] != 0)
//				{
//					VSM_DEBUG("smart config get ip gw done.......\r\n");
//					memset(AppConfig.MQTT_Config.broker, 0x00, sizeof(AppConfig.MQTT_Config.broker));
//					strcpy(AppConfig.MQTT_Config.broker, IP_Broker_local);
//					xFlag.bt_config = true;
//					xFlag.scan_broker = false;	//tranh truong hop phai rescan lai sau khi smart config
//					ProcessTask_SetEvent(EVT_SAVE_DATA_CFG);
//					ProcessTask_SetEvent(EVT_MQTT_START);
//					ProcessEvent ^= EVT_GET_IP_GATEWAY;
//				}
//			}
//
//			//ko scan duoc ip, timeout
//			if(xFlag.smartcfg_timeout == true)
//				ProcessEvent ^= EVT_GET_IP_GATEWAY;
//		}
//
//	}
//
//	if( (ProcessEvent & EVT_SMART_CONFIG) == EVT_SMART_CONFIG )		//smart config using wifi, coding here
//	{
//		ProcessEvent ^= EVT_SMART_CONFIG;
//	}
//
//	if( (ProcessEvent & EVT_BT_CONFIG_STOP) == EVT_BT_CONFIG_STOP )
//	{
//		VSM_DEBUG(" stop bt_config......\r\n ");
//
//		//if mode local
//		if( (AppConfig.MQTT_Config.mode == MQTT_CONN_LOCAL_SSL) ||(AppConfig.MQTT_Config.mode == MQTT_CONN_LOCAL_NO_SSL) )
//			broadcast_stop();
//
//		xFlag.do_smartcfg = false;
//		ble_app_stop(bt_config);
//		Message_response_smartcfg(xFlag.bt_config);	//send ket qua smartconfig
//		ProcessEvent ^= EVT_BT_CONFIG_STOP;
//	}
//
//
//	if( (ProcessEvent & EVT_SAVE_DATA_CFG) == EVT_SAVE_DATA_CFG )	//co the set event nay sau khi connect mqtt thanh cong
//	{
//		app_saveConfig(&AppConfig);
//		ProcessEvent ^= EVT_SAVE_DATA_CFG;
//		ProcessTask_SetEvent(EVT_BT_CONFIG_STOP);
//	}
}












