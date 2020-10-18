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
		NAME_DEVICE_SHOW, MODEL_DEVICE, DATE_RELEASE, FW_VERSION);
}




void device_info_get_mac(char* mac)
{

}









