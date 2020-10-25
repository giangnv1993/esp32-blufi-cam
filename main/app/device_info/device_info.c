/*
 * define_info.c
 *
 *  Created on: Aug 14, 2020
 *      Author: giangnv
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "device_info.h"


//depend platform
#include "esp_wifi.h"



//WF_Namedevice_model_yymmdd_snfw_fwversion = max 27 bytes


void deviceinfo_get_firmware_version(char* fw_version)
{
	char* fw_vs = calloc(LEN_DEVICE_INFO, sizeof(char*));
	sprintf(fw_vs, "WF_%s_%s_%s_%s",\
			NAME_DEVICE,\
			MODEL_DEVICE,\
			DATE_RELEASE,\
			FW_VERSION);
	strcpy(fw_version, fw_vs);

	free(fw_vs);
}


void deviceinfo_show(void)
{

	printf(
	"\r\n===============================================================\n"
		"Device name :%s\n"
		"Model: %s\n"
		"Date release: %s\n"
		"Firmware version: %s\n"
		"===============================================================\r\n",
		NAME_DEVICE, MODEL_DEVICE, DATE_RELEASE, FW_VERSION);

}


//call when wifi on

void device_info_get_mac(char* mac)
{
	char* __mac = (char*)calloc(16, sizeof(char));
	uint8_t buff_mac[8] = {0};
 	// esp_wifi_get_mac(ESP_IF_WIFI_STA, buff_mac);
	 esp_efuse_mac_get_default(buff_mac);

	sprintf(__mac, "%2X%2X%2X%2X%2X%2X", buff_mac[0], buff_mac[1], buff_mac[2], buff_mac[3], buff_mac[4], buff_mac[5]);

	printf("---- device_info_get_mac = %s\r\n", __mac);

	strcpy(mac, __mac);
	free(__mac);
}









