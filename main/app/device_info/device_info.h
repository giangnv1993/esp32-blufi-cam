/*
 * device_info.h
 *
 *  Created on: Aug 25, 2020
 *      Author: admin
 */

#ifndef SOURCE_APPS_DEVICE_INFO_DEVICE_INFO_H_
#define SOURCE_APPS_DEVICE_INFO_DEVICE_INFO_H_

#include "device_data.h"




//===================define for info wifi=====================================//


//=======device info=========//
// #define NAME_DEVICE_SHOW		"BluBox"
#define NAME_DEVICE				"BluBox"
#define MODEL_DEVICE			"BB01"		//BA400
#define DATE_RELEASE			"20201025"
#define FW_VERSION				"V001"
// #define THINGTYPE				"vsmart_airpurifier_ba400"
//======end================//







//======================= define feature =====================================//

#define LEN_DEVICE_INFO					(32)

#define WATCHDOG_ENABLE					(0)

#define COMMAND_CLI_ENABLE				(0)

#define TIME_REFRESH_WTD				(15000)	//14s

#define OTA_ENABLE						(0)




void deviceinfo_get_firmware_version(char* fw_version);

void deviceinfo_show(void);

void device_info_get_mac(char* mac);

#endif /* SOURCE_APPS_DEVICE_INFO_DEVICE_INFO_H_ */


















