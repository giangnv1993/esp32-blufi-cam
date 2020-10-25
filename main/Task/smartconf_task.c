/*
 * smartConf_Task.c
 *
 *  Created on: Mar 8, 2019
 *      Author: giangnv3
 */

#include "smartconf_task.h"
#include "esp_wpa2.h"
#include "esp_system.h"
#include "tcpip_adapter.h"
#include "esp_smartconfig.h"
// #include "../Interface/UserFile.h"

#if 0

extern EventGroupHandle_t wifi_event_group;

//extern int WIFI_CONNECTED_BIT; // @suppress("Symbol is not resolved")
static const int ESPTOUCH_DONE_BIT = BIT1; // @suppress("Symbol is not resolved")

extern WifiStation_Info _WifiInfo;

bool bSmart_Conf_start = false;
uint16_t Smc_TimeOut = 0;

/******************************************************************************
 *
 * ****************************************************************************/
static void sc_callback(smartconfig_status_t status, void *pdata)
{
    switch (status) {
        case SC_STATUS_WAIT:
            printf("\r\n--- SC_STATUS_WAIT");
            break;
        case SC_STATUS_FIND_CHANNEL:
            printf("\r\n--- SC_STATUS_FINDING_CHANNEL");
            break;
        case SC_STATUS_GETTING_SSID_PSWD:
        	Smc_TimeOut = 0;
            printf("\r\n--- SC_STATUS_GETTING_SSID_PSWD");
            break;
        case SC_STATUS_LINK:
        	Smc_TimeOut = 0;
            printf("\r\n--- SC_STATUS_LINK");
            wifi_config_t *wifi_config = pdata;

            printf("\r\n--- SSID:%s", wifi_config->sta.ssid);
            printf("\r\n--- PASSWORD:%s", wifi_config->sta.password);
            strcpy(_WifiInfo.UserName, (char *)wifi_config->sta.ssid);
            strcpy(_WifiInfo.Password, (char *)wifi_config->sta.password);
            vUserFile_SaveInfo_WifiStation( &_WifiInfo );

            ESP_ERROR_CHECK( esp_wifi_disconnect() );
            ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, wifi_config) );
            ESP_ERROR_CHECK(esp_wifi_connect());
            printf("\r\n---- Wifi smart config done ......\r\n");
            break;
        case SC_STATUS_LINK_OVER:
            printf("\r\n--- SC_STATUS_LINK_OVER");
            if (pdata != NULL) {
                uint8_t phone_ip[4] = { 0 };
                memcpy(phone_ip, (uint8_t* )pdata, 4);
                printf("\r\n--- Phone ip: %d.%d.%d.%d\n", phone_ip[0], phone_ip[1], phone_ip[2], phone_ip[3]);
            }
            SmartConf_stop();
            xEventGroupSetBits(wifi_event_group, ESPTOUCH_DONE_BIT);
            break;
        default:
            break;
    }
}


/******************************************************************************
 *
 * ****************************************************************************/
void SmartConf_Task(void *pvParameters)
{
	vTaskDelay(10000);
	printf("\r\n---- SmartConf_Task: SmartConf_Task start......\r\n");
	//EventBits_t uxBits;
	bSmart_Conf_start = false;

	while (1)
	{
		if(bSmart_Conf_start == true)
		{
			Smc_TimeOut++;
			if(Smc_TimeOut >= 20) //time out after 20s
			{
				SmartConf_stop();
				bSmart_Conf_start = 0;
				Smc_TimeOut = 0;
				printf("--- SmartConf: time out...\r\n");
			}
		}

//		uxBits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY); // @suppress("Symbol is not resolved")
//
//		if(uxBits & WIFI_CONNECTED_BIT)
//		{ // @suppress("Symbol is not resolved")
//			printf("\r\n--- WiFi Connected to ap.....\r\n");
////			SmartConf_stop();
//		}
//
//		if(uxBits & ESPTOUCH_DONE_BIT)
//		{
//			printf("\r\n---- Smartconfig over");
//			esp_smartconfig_stop();
//		}
		printf("--- SmartConf: Smc_TimeOut++\r\n");
		vTaskDelay(1000);
	}
}



/******************************************************************************
 *
 * ****************************************************************************/
void SmartConf_start(void)
{
	ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
	ESP_ERROR_CHECK( esp_smartconfig_start(sc_callback) );
}
/******************************************************************************
 *
 * ****************************************************************************/
void SmartConf_stop(void)
{
	esp_smartconfig_stop();
	printf("\r\nSmart config stop\r\n");
	SmartConf_finish();
}



void SmartConf_EventStart( void )
{
	SmartConf_start();
	bSmart_Conf_start = true;
}
#endif
