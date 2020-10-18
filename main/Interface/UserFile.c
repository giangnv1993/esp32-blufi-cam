/*
 * UserFile.c
 *
 *  Created on: Apr 4, 2018
 *      Author: ThaoHuyen
 */
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <esp_err.h>
#include <esp_spiffs.h>

#include "UserFile.h"
#include "../main.h"
#include "FileAccess.h"
#include "Spiffs/spiffs_vfs.h"



#define macroUSER_FILE_SPIFFS_PATH					SPIFFS_BASE_PATH

//wifi info
#define macroUSER_FILE_STATION_INFO					"/stationinfo.conf"

#define macroUSER_FILE_STA_USERNAME_KEY				"sta_username"
#define macroUSER_FILE_STA_PASSWORD_KEY				"sta_password"






/*******************************************************************************
 * Function		: void vUserFile_SaveInfo_WifiStation( WifiStation_Info *xWifiStation_Info )
 * Description	: save wifi station infor
 * Param		: xWifiStation_Info - wifi station info
 * Return		: None
 ******************************************************************************/
void vUserFile_SaveInfo_WifiStation( WifiStation_Info *xWifiStation_Info )
{
	char STA_InfoFile[32] = {0};
	char Buffer[macroFILE_ACCESS_MAX_LENGHT] = {0};

	printf("\r\n--- UserFile: Saving file wifi station info\r\n");

	//check wifi staion info file existed
	sprintf(STA_InfoFile, "%s%s", macroUSER_FILE_SPIFFS_PATH, macroUSER_FILE_STATION_INFO);

	if(eFileAccess_CheckFile_Existed(STA_InfoFile) == eFileFail)
	{
		printf("--- UserFile: wifi staion save info file not existed. Creating file\r\n");
		vFileAccess_CreateFile(STA_InfoFile);
	}

	sprintf(Buffer, "%s=%s\n%s=%s\n", 	macroUSER_FILE_STA_USERNAME_KEY, xWifiStation_Info->UserName, \
										macroUSER_FILE_STA_PASSWORD_KEY, xWifiStation_Info->Password \
			);
	if(eFileAccess_WriteFile_Empty(STA_InfoFile, Buffer) == eFileFail)
	{
		printf("--- UserFile: save wifi staion info failture\r\n");
	}
	else
	{
		printf("--- UserFile: save wifi staion info successfully\r\n");
	}
}





/*******************************************************************************
 * Function		: void vUserFile_LoadInfo_WifiStation( WifiStation_Info *xWifiStation_Info )
 * Description	: Load wifi station infor
 * Param		: xWifiStation_Info - wifi station info
 * Return		: None
 ******************************************************************************/
void vUserFile_LoadInfo_WifiStation( WifiStation_Info *xWifiStation_Info )
{
	char STA_InfoFile[32] = {0};
	char Buffer[macroFILE_ACCESS_MAX_LENGHT] = {0};
	char Array[4][macroFILE_ACCESS_LINE_MAX_LENGHT] = {0};
	uint16_t ui = 0, uj = 0, uk = 0;

	printf("\r\n--- UserFile: loading file wifi station info\r\n");

	//check wifi staion info file existed
	sprintf(STA_InfoFile, "%s%s", macroUSER_FILE_SPIFFS_PATH, macroUSER_FILE_STATION_INFO);
	if(eFileAccess_CheckFile_Existed(STA_InfoFile) == eFileFail)
	{
		printf("--- UserFile: wifi staion load info file not existed\r\n");
		return;
	}

	eFileAccess_ReadAll(STA_InfoFile, Buffer);

	for( ; Buffer[ui] > 0; )
	{
		if(Buffer[ui] == '\r')
			continue;
		else if( (Buffer[ui] == '=') || (Buffer[ui] == '\n') )
		{
			uj++;
			uk = 0;
		}
		else
			Array[uj][uk++] = Buffer[ui];

		ui++;
	}

	printf("--- UserFile: load wifi info successfully\r\n");

	if(strcmp(Array[0], macroUSER_FILE_STA_USERNAME_KEY) == 0)
		strcpy(xWifiStation_Info->UserName, strdup(Array[1]));
	if(strcmp(Array[2], macroUSER_FILE_STA_PASSWORD_KEY) == 0)
		strcpy(xWifiStation_Info->Password, strdup(Array[3]));
}























