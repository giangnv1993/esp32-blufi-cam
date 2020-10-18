/*
 * device_info.h
 *
 *  Created on: Aug 25, 2020
 *      Author: admin
 */

#ifndef SOURCE_APPS_DEVICE_INFO_DEVICE_INFO_H_
#define SOURCE_APPS_DEVICE_INFO_DEVICE_INFO_H_






//===================define for info wifi=====================================//


//=======device info=========//
#define NAME_DEVICE_SHOW		"Air Purifier"
#define NAME_DEVICE				"SmartLock"
#define MODEL_DEVICE			"SM01"		//BA400
#define DATE_RELEASE			"20200922"
#define FW_VERSION				"001"
#define THINGTYPE				"vsmart_airpurifier_ba400"
//======end================//







//======================= define feature =====================================//

#define LEN_DEVICE_INFO					(32)

#define WATCHDOG_ENABLE					(1)

#define COMMAND_CLI_ENABLE				(1)

#define TIME_REFRESH_WTD				(15000)	//14s

#define OTA_ENABLE						(1)

#define CONVET_TO_LOWCASE				(0)

#define GET_INFO_ENABLE					(1)

//#define SERVER_DEMO_ENABLE
//=============================================================================//


//WF_AP_BA400_200810_0102_111;  WF_Namedevice_model_yymmdd_snfw_fwversion = max 27 bytes

//typedef struct
//{
//	char name_device[3];
//	char model[6];
//	char date_release;
//	char fw_sn[5];
//	uint16_t fw_version;
//}device_info_t;


void deviceinfo_get_firmware_version(char* fw_version);

void deviceinfo_show(void);

void device_info_get_mac(char* mac);

#endif /* SOURCE_APPS_DEVICE_INFO_DEVICE_INFO_H_ */


















