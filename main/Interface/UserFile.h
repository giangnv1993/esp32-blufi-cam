/*
 * UserFile.h
 *
 *  Created on: Apr 4, 2018
 *      Author: ThaoHuyen
 */

#ifndef MAIN_INTERFACE_USERFILE_H_
#define MAIN_INTERFACE_USERFILE_H_


typedef struct
{
	char UserName[32];
	char Password[64];
}WifiStation_Info;




/*******************************************************************************
 * Function		: void vUserFile_SaveInfo_WifiStation( WifiStation_Info *xWifiStation_Info )
 * Description	: save wifi station infor
 * Param		: xWifiStation_Info - wifi station info
 * Return		: None
 ******************************************************************************/
void vUserFile_SaveInfo_WifiStation( WifiStation_Info *xWifiStation_Info );




/*******************************************************************************
 * Function		: void vUserFile_LoadInfo_WifiStation( WifiStation_Info *xWifiStation_Info )
 * Description	: Load wifi station infor
 * Param		: xWifiStation_Info - wifi station info
 * Return		: None
 ******************************************************************************/
void vUserFile_LoadInfo_WifiStation( WifiStation_Info *xWifiStation_Info );



#endif /* MAIN_INTERFACE_USERFILE_H_ */











