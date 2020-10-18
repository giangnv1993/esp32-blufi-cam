/*
 * Wifi.c
 *
 *  Created on: Apr 2, 2019
 *      Author: giangnv3
 */

#include <stdbool.h>
#include "Wifi.h"

#include "../app.h"
#include "../../main.h"



/******************************************************************************
 *variable
******************************************************************************/

EventGroupHandle_t wifi_event_group;
int WIFI_CONNECTED_BIT = BIT0; // @suppress("Symbol is not resolved")

//wifi_info_t _WifiInfo = {
//		.UserName = "giangnv",
//		.Password = "Giang1993",
//};

/******************************************************************************
 *function
******************************************************************************/



static void wifi_init_sta(char* ssid, char* password);



void (*Wifi_Status)(uint32_t /*Status*/);



/******************************************************************************
  * @brief  WifiTask.
  * @param  None
  * @retval None
******************************************************************************/
static esp_err_t event_handler_wifi(void *ctx, system_event_t *event)
{
	switch(event->event_id)
	{
	    case SYSTEM_EVENT_STA_START:
	    	APP_DEBUG("Wifi start.........\r\n");
	        esp_wifi_connect();
	        break;

	    case SYSTEM_EVENT_STA_GOT_IP:
	    	APP_DEBUG("got ip:%s",\
	              ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip)); // @suppress("Field cannot be resolved")
	        break;

	    case SYSTEM_EVENT_STA_CONNECTED:
	    	APP_DEBUG("wifi connected....\r\n");
	    	xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
	    	if(Wifi_Status != NULL)
	    		Wifi_Status(EVT_WIFI_CONNECTED);
	    	break;

	    case SYSTEM_EVENT_STA_DISCONNECTED:
	    	if(Wifi_Status != NULL)
	    		Wifi_Status(EVT_WIFI_DISCONNECTED);
	    	else
	    	{};
			esp_wifi_connect();
	    	xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
	        break;

	    default:
	        break;
	    }
	    return ESP_OK;
}
/******************************************************************************
  * @brief  Wifi_Init.
  * @param  None
  * @retval None
******************************************************************************/
void Wifi_Init(Wifi_Mode_t Mode)
{
	if(Mode == WF_MODE_STA)
		wifi_init_sta(_WifiInfo.UserName, _WifiInfo.Password);
	else
	{
		//run other mode
	}
}


/******************************************************************************
  * @brief  Wifi_DeInit.
  * @param  None
  * @retval None
******************************************************************************/
void Wifi_DeInit(void)
{
	esp_wifi_deinit();
}

/******************************************************************************
  * @brief  Wifi_Connect_toAP.
  * @param  RTW_SECURITY_WPA2_AES_PSK
  * @retval None
******************************************************************************/
static void wifi_init_sta(char* ssid, char* password)
{
	wifi_event_group = xEventGroupCreate();

	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler_wifi, NULL) ); // @suppress("Symbol is not resolved")

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); // @suppress("Symbol is not resolved")
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	wifi_config_t xWifi_Config =
	{
		.sta =
		{
			.scan_method = WIFI_FAST_SCAN,
			.sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
			.threshold.rssi = -127,
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,
		},
	};

	strcpy((char *)xWifi_Config.sta.ssid, ssid);
	strcpy((char *)xWifi_Config.sta.password, password);

//	strcpy((char *)xWifi_Config.sta.ssid, _WifiInfo.UserName);
//	strcpy((char *)xWifi_Config.sta.password, _WifiInfo.Password);

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &xWifi_Config) );
	ESP_ERROR_CHECK(esp_wifi_start() );
}






/******************************************************************************
  * @brief  Main program.
  * @param  None
  * @retval None
******************************************************************************/
void Wifi_Register_StatusCallBack(void stt_cb(uint32_t))
{
	if(stt_cb != NULL)
		Wifi_Status = stt_cb;
}

















