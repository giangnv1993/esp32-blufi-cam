/*
 * APP_DATA.h
 *
 *  Created on: Mar 30, 2019
 *      Author: giangnv3
 */

#ifndef SOURCE_APPDATA_H_
#define SOURCE_APPDATA_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "mqtt/MQTTClient.h"

#include "../Task/ProcessTask.h"
#include "../Task/UartTask.h"
//#include "mqtt/mqtt_task.h"
//#include "../app/wifi/Wifi.h"
#include "../app/smartconfig/blufi.h"

#include "../Interface/UserTimer.h"

#include "app_debug.h"
#include "app_delay.h"
#include "cli/app_cli.h"
#include "Button/HardButton.h"

//#include "../Interface/Spiffs/esp_spiffs.h"


#define USE_MAC_12BYTE			(1)

#if USE_MAC_12BYTE
	#define SIZE_ID_DEVICE						(64)
#else
	#define SIZE_ID_DEVICE						(16)
#endif

#define GPIO_OUTPUT_LIGHT    0			//GPIO0
#define GPIO_OUTPUT_LOCK     14			// GPIO14
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_LIGHT) | (1ULL<<GPIO_OUTPUT_LOCK))


#define SIZE_NAME				(64)

#define SIZE_BLE_NAME			(32)

#define LEN_CMD					(128)

#define MAIN_BOARD_DATA_LEN		UART_LEN_MAX

#define SERVER_MSG_LEN			(1024)

#define SIZE_ID_Ed				(16)

/******************************************************************************/



#define WIFI_SSID_PASS_SIZE		(32)
/******************************************************************************
 * Define area for flash, total size = 4096 Byte = 1 sector
 * ****************************************************************************/


/*define addr flash*/
#define SIZE_FLASH_ACTIVE					(2)
#define SIZE_WIFI_CONFIG					(WIFI_SSID_PASS_SIZE * 2 + 16)
#define SIZE_MQTT_CONFIG					(SIZE_BROKER_MQTT + SIZE_USER_PASS_MQTT *3)
#define NUMBER_SECTOR						(16)
#define SIZE_SECTOR							(4096)



//#define 	ADDR_FLASH_BASE								FLASH_APP_BASE				//(0x200000 - 0xA9000)
#define 	ADDR_FLASH_BASE								0x185000				//(0x200000 - 0xA9000)
#define 	ADDR_WIFI_CONF								(ADDR_FLASH_BASE - 0x1000)
#define 	ADDR_MQTT_CONF								(ADDR_FLASH_BASE - 0x2000)

//define fw1 start 10000 = 0x200000 - 1F0000
//sizefw1 = BB000 = 766kb + addr_fw1 = CB000 = 200000 - 135000
//fw2 start CB000 = 200000 - 135000
//fw2 size (end) = BB000 + CB000 = 186000 = 200000 - 7A000

typedef enum
{
	eOK = 1,
	eNOK = 0
}Result_t;



typedef enum
{
	EVT_WIFI_START			= 0x00000001,
	EVT_WIFI_CONNECTED		= 0x00000002,
	EVT_WIFI_DISCONNECTED	= 0x00000004,
	EVT_WIFI_AVAILABLE		= 0x00000008,
	EVT_WIFI_CHECK_CON		= 0x00000010,
	EVT_WIFI_STOP			= 0x00000020,

	/*Event MQTT*/
	EVT_MQTT_START			= 0x00000040,
	EVT_MQTT_STOP			= 0x00000080,
	EVT_MQTT_DISCONNECTED   = 0x00000100,
	EVT_MQTT_RUNNING		= 0x00000200,
	EVT_MQTT_SCAN_BROKER	= 0x00000400,		//in mode local
	EVT_MQTT_CHECK_CONN		= 0x00000800,

	/*event process Data*/
	EVT_DATA_MAINBOARD		= 0x00001000,
	EVT_DATA_SERVER			= 0x00002000,

	/*bt_config*/
	EVT_BLUFI_START			= 0x00004000,
	EVT_BT_CONFIG_RESULT	= 0x00008000,
	EVT_SMART_CONFIG		= 0x00010000,
	EVT_BT_CONFIG_STOP		= 0x00020000,
	EVT_SAVE_DATA_CFG		= 0x00040000,
	EVT_SMF_CHECK_TIMEOUT	= 0x00080000,

	EVT_SET_LOCK_ON			= 0x00100000,

	EVT_SET_LIGHT_ON		= 0x00200000,

	EVT_PROCESS_IDLE         = 0x00000000,
}eProcessEvent;




typedef struct
{
	char ssid[WIFI_SSID_PASS_SIZE + 1];
	char pass[WIFI_SSID_PASS_SIZE  + 1];
	unsigned long wifi_security;
}Wifi_STA_conf_t;


typedef struct
{
	bool wifi_connected;
	bool ble_connected;
	bool bt_config;
	bool mqtt_conn;
	bool scan_broker;
	bool do_smartcfg;
	bool msg_refresh;
	bool smartcfg_timeout;
	uint8_t send_status;
	bool sent_reqs;
}Flag_t;




typedef struct
{
//	MQTTClient_Config_t MQTT_Config;
	Wifi_STA_conf_t Wifi_STA_Config;
	char ID_Device[SIZE_ID_DEVICE + 1];
}AppConfig_t;


typedef enum
{
	mqtt = 1,
	wifi = 0
}type_config_t;



//============================================================================//



void app_init(AppConfig_t* __AppConfig);



void app_saveConfig(AppConfig_t* __AppConfig);


void app_memsetConfig(AppConfig_t* __AppConfig);

void GPIO_Set_Pin_Low(gpio_num_t gpio);

void GPIO_Set_Pin_High(gpio_num_t gpio);


#endif /* SOURCE_APPDATA_H_ */








