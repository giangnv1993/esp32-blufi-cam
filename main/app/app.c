/*
 * AppData.c
 *
 *  Created on: Apr 2, 2020
 *      Author: giang
 */


#include "main.h"
#include "app.h"


void GPIO_INIT(void);



#define GPIO_INPUT_IO_1     16

#define ESP_INTR_FLAG_DEFAULT 0



tsButtonHwConfig button_hw_cfg[] =
{
	{
			.u8BtnPin = (uint32_t)GPIO_INPUT_IO_1,
	}
};





static void app_loadConfig(AppConfig_t* __AppConfig);

static void app_loadConfigDefault(AppConfig_t* __AppConfig, type_config_t __type_cfg);




void App_pHardButton_Press_EventHandler(int idx, int evt_type, void* data)
{
	printf("\r\ntouch %d press\r\n", idx);
	ProcessTask_SetEvent(EVT_WIFI_AVAILABLE);
}

void app_init(AppConfig_t* __AppConfig)
{

	GPIO_INIT();
//	app_loadConfig(__AppConfig);

	//register status
//	Wifi_Register_StatusCallBack(ProcessTask_SetEvent);
//	MQTT_Register_StatusCallBack(ProcessTask_SetEvent);
//	message_register_setevent(ProcessTask_SetEvent);

	app_delay_init();		// delay
	UserTimer_Init();

//	Wifi_Init(WF_MODE_STA);
//	Wifi_Register_StatusCallBack(ProcessTask_SetEvent);

	blufi_register_status(ProcessTask_SetEvent);

	mqtt_register_status(ProcessTask_SetEvent);

	ProcessTask_Init();
//	UartTask_Init();

	vHardButtonInit(button_hw_cfg, 1);
	vHardButtonSetCallback(E_EVENT_HARD_BUTTON_PRESS , App_pHardButton_Press_EventHandler, 0);


//	blufi_wifi_start();

//	vsm_cli_start();
}




static void app_loadConfigDefault(AppConfig_t* __AppConfig, type_config_t __type_cfg)
{
#if 0
	if(__type_cfg == mqtt)
	{
		strcpy(__AppConfig->MQTT_Config.broker, (char*)MQTT_BROKER_DEFAULT);
		__AppConfig->MQTT_Config.port = MQTT_PORT_DEFAULT;
		strcpy(__AppConfig->MQTT_Config.username, (char*)MQTT_USER_DEFAULT);
		strcpy(__AppConfig->MQTT_Config.password, (char*)MQTT_PASS_DEFAULT);
		__AppConfig->MQTT_Config.keepalive_s = MQTT_KEEP_ALIVE_DEFAULT;
		__AppConfig->MQTT_Config.mode = MQTT_MODE_DEFAULT;
	}
	else if(__type_cfg == wifi)
	{
		strcpy(__AppConfig->Wifi_STA_Config.ssid, (char*)WIFI_SSID_DEFAULT);
		strcpy(__AppConfig->Wifi_STA_Config.pass, (char*)WIFI_PASS_DEFAULT);
		__AppConfig->Wifi_STA_Config.wifi_security =  (unsigned long)WIFI_SECURITY_DEFAULT;
#ifdef SERVER_DEMO_ENABLE
		strcpy(__AppConfig->ID_Device, "UfZ94sHo09bApu1QZBff");	//for test
#endif
	}
#endif
}




static void app_loadConfig(AppConfig_t* __AppConfig)
{
#if 0
	char* Mqtt_Config = (char*)calloc(SIZE_MQTT_CONFIG, sizeof(char));
	char* Wifi_Sta_Config = (char*)calloc(SIZE_WIFI_CONFIG, sizeof(char));

#ifdef SERVER_DEMO_ENABLE
	app_loadConfigDefault(__AppConfig, wifi);
#else
	/*1. Load Wifi Station configuration*/
	Flash_Read(&xFlash, (uint32_t)ADDR_WIFI_CONF, SIZE_WIFI_CONFIG, (uint8_t*)Wifi_Sta_Config);

	if(strlen(Wifi_Sta_Config) > 0)		//has data
	{
		//load config ssid,pass
		VSM_DEBUG("load wifi station config from flash\r\n");
		const char cut[2] = ",";
		char* pData;
		uint8_t index = 0;
		memset(&__AppConfig->Wifi_STA_Config, 0, sizeof(__AppConfig->Wifi_STA_Config));
		pData = strtok(Wifi_Sta_Config, cut);
		while( pData != NULL )
		{
			index++;
			if(index == 1)		//check header
			{
				if(strcmp(pData, HEADER_FLASH) != 0)
					goto wifi_default;
			}
			else if(index == 2)
			{
				strcpy(__AppConfig->Wifi_STA_Config.ssid, pData);
				VSM_DEBUG("load ssid: %s\r\n", __AppConfig->Wifi_STA_Config.ssid);

			}
			else if(index == 3)
			{
				strcpy(__AppConfig->Wifi_STA_Config.pass, pData);
				VSM_DEBUG("load pass: %s\r\n", __AppConfig->Wifi_STA_Config.pass);
			}
			else if(index == 4)
			{
				__AppConfig->Wifi_STA_Config.wifi_security = atol(pData);
				VSM_DEBUG("load pass: %ld\r\n", __AppConfig->Wifi_STA_Config.pass);
			}
			else
				goto wifi_default;	//load wifi backup if failed, coding later

			pData = strtok(NULL, cut);
		}
	}
	else
	{
		wifi_default:
		{
			VSM_DEBUG("load default wifi config....\r\n");
//			app_loadConfigDefault(__AppConfig, wifi);			// ko enable cai nay len
		}
	}
#endif
	/*2. Load mqtt configuration
	 * broker,
	 * user,
	 * pass,
	 * port,
	 * mode: 1= ssl, 0 = local
	 *
	 * */
#ifdef SERVER_DEMO_ENABLE
	app_loadConfigDefault(__AppConfig, mqtt);
#else
	Flash_Read(&xFlash, ADDR_MQTT_CONF, SIZE_MQTT_CONFIG, (uint8_t*)Mqtt_Config);

	if(strlen((char*)Mqtt_Config) > 0)		//has data
	{
		//load config
		const char cut[2] = ",";
		char* pData;
		uint8_t index = 0;
		memset(&__AppConfig->MQTT_Config, 0, sizeof(__AppConfig->MQTT_Config));

		pData = strtok(Mqtt_Config, cut);
		while( pData != NULL )
		{
			index++;
			if(index == 1)		//check header
			{
				if(strcmp(pData, HEADER_FLASH) != 0)
					goto mqtt_default;
			}
			else if(index == 2)
			{
				strcpy(__AppConfig->MQTT_Config.broker, pData);
				VSM_DEBUG("Load MQTT broker: %s\r\n", __AppConfig->MQTT_Config.broker);
			}
			else if(index == 3)
			{
				strcpy(__AppConfig->MQTT_Config.username, pData);
				VSM_DEBUG("Load MQTT user: %s\r\n", __AppConfig->MQTT_Config.username);
			}
			else if(index == 4)
			{
				strcpy(__AppConfig->MQTT_Config.password, pData);
				VSM_DEBUG("Load MQTT password: %s\r\n", __AppConfig->MQTT_Config.password);
			}
			else if(index == 5)
			{
				__AppConfig->MQTT_Config.port = (int)atoi(pData);
				VSM_DEBUG("Load MQTT port: %d\r\n", __AppConfig->MQTT_Config.port);
			}
			else if(index == 6)
			{
				__AppConfig->MQTT_Config.mode = (int)atoi(pData);
				VSM_DEBUG("Load MQTT mode: %s\r\n", __AppConfig->MQTT_Config.mode == 1? "ssl":"local" );
			}
			else
				goto mqtt_default;
				//load broker backup if failed, coding later
			pData = strtok(NULL, cut);
		}
	}
	else	//no data, load config default
	{
		mqtt_default:
		{
			VSM_DEBUG("load default mqtt config....\r\n");
//			app_loadConfigDefault(__AppConfig, mqtt);
		}
	}

#endif
	free(Mqtt_Config);
	free(Wifi_Sta_Config);
#endif
}


/*2. Load mqtt configuration
 * broker,
 * user,
 * pass,
 * port,
 * mode: 1= ssl, 0 = local
 *
 * */

void app_saveConfig(AppConfig_t* __AppConfig)
{
#if 0
	//sau khi wifi connect done, save config wifi va mqtt
	char* Mqtt_Config = (char*)calloc(SIZE_MQTT_CONFIG, sizeof(char));
	char* Wifi_Sta_Config = (char*)calloc(SIZE_WIFI_CONFIG, sizeof(char));

	sprintf(Mqtt_Config, "%s,%s,%s,%s,%d,%d", HEADER_FLASH				\
										, __AppConfig->MQTT_Config.broker\
										, __AppConfig->MQTT_Config.username\
										,__AppConfig->MQTT_Config.password\
										,__AppConfig->MQTT_Config.port\
										,__AppConfig->MQTT_Config.mode);

	Flash_EraseSector(&xFlash, ADDR_MQTT_CONF);
	vTaskDelay(5);
	Flash_Write(&xFlash, (uint32_t)ADDR_MQTT_CONF, strlen(Mqtt_Config), (uint8_t*)Mqtt_Config);

	sprintf(Wifi_Sta_Config, "%s,%s,%s,%ld", HEADER_FLASH				\
										, __AppConfig->Wifi_STA_Config.ssid\
										, __AppConfig->Wifi_STA_Config.pass\
										,__AppConfig->Wifi_STA_Config.wifi_security);

	Flash_EraseSector(&xFlash, ADDR_WIFI_CONF);
	vTaskDelay(5);
	Flash_Write(&xFlash, (uint32_t)ADDR_WIFI_CONF, strlen(Wifi_Sta_Config), (uint8_t*)Wifi_Sta_Config);
	vTaskDelay(5);
#endif
}


void app_memsetConfig(AppConfig_t* __AppConfig)
{
#if 0
	//wifi
	memset(__AppConfig->Wifi_STA_Config.ssid, 0x00, sizeof(__AppConfig->Wifi_STA_Config.ssid));
	memset(__AppConfig->Wifi_STA_Config.pass, 0x00, sizeof(__AppConfig->Wifi_STA_Config.pass));
	__AppConfig->Wifi_STA_Config.wifi_security = RTW_SECURITY_WPA2_AES_PSK;


	//mqtt
	memset(__AppConfig->MQTT_Config.broker, 0x00, sizeof(__AppConfig->MQTT_Config.broker));
	memset(__AppConfig->MQTT_Config.username, 0x00, sizeof(__AppConfig->MQTT_Config.username));
	memset(__AppConfig->MQTT_Config.password, 0x00, sizeof(__AppConfig->MQTT_Config.password));
	__AppConfig->MQTT_Config.mode = MQTT_MODE_DEFAULT;
#endif
}



void GPIO_INIT(void)
{
	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

}

void GPIO_Set_Pin_High(gpio_num_t gpio)
{
	gpio_set_level(gpio, 1);
}

void GPIO_Set_Pin_Low(gpio_num_t gpio)
{
	gpio_set_level(gpio, 0);
}





























