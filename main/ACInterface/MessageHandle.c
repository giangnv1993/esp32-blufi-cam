/*
 * Parse.c
 *
 *  Created on: Mar 6, 2019
 *      Author: thaonm1
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MessageHandle.h"



#define MESSAGE_CTRL_LENGHT				 25



#define MSG_HEADER_LENGHT			10
unsigned char MSG_HEADER[MSG_HEADER_LENGHT + 1] = {0xBB, 0x00, 0x06, 0x80, 0x00, 0x00, 0x0F, 0x00, 0x01, 0x01};

#define MSG_STATIC1_LENGHT			1
unsigned char MSG_STATIC1[MSG_STATIC1_LENGHT + 1] = {0x20};

#define MSG_STATIC2_LENGHT			3
unsigned char MSG_STATIC2[MSG_STATIC2_LENGHT + 1] = {0x20, 0x00, 0x00};

#define MSG_STATIC3_LENGHT			4
unsigned char MSG_STATIC3[MSG_STATIC3_LENGHT + 1] = {0x00, 0x10, 0x00, 0x00};




/******************************************************************************
 * Function		: unsigned char **MessageHandle_Parse( unsigned char *RXBuffer, unsigned char RXLenght, unsigned char *FieldNumber )
 * Description	: message parse
 * Param		:
 * 				+ RXBuffer - buffer data
 * 				+ RXLenght - lenght of buffer
 * 				+ FieldNumber - number of field data
 * return		: None
 * ***************************************************************************/
unsigned char **MessageHandle_Parse( unsigned char *RXBuffer, unsigned char RXLenght, unsigned char *FieldNumber )
{
	unsigned char **Array = calloc(RXLenght + 16, sizeof(char));
	unsigned char index = 0;

	unsigned char *ptr = (unsigned char *)strtok((char *)RXBuffer, ",!");
	while(ptr != NULL)
	{
		Array[index++] = ptr;
		ptr = (unsigned char *)strtok(NULL, ",!");
	}

	*FieldNumber = index;

	return Array;
}




/******************************************************************************
 * Function		: unsigned char *MessageHandle_UartParking( msgData_t *_msgData, unsigned char *msgLenght )
 * Description	: message parking for uart transmit
 * Param		: _msgData - message control struct
 * return		: message string
 * ***************************************************************************/
unsigned char *MessageHandle_UartParking( msgData_t *_msgData, unsigned char *msgLenght )
{
	unsigned char *msg = calloc(MSG_MAX_LENGHT + 1, sizeof(unsigned char));
	unsigned char Lenght = 0;
	signed long CheckSum = 0x000000;
	unsigned char ui = 0;

	//header
	for(ui = 0; ui < MSG_HEADER_LENGHT; ui++)
		msg[Lenght++] = MSG_HEADER[ui];

	//Temp and swing byte
	msg[Lenght++] = ((_msgData->Temp - 8) << 3) | (_msgData->Swing & 0x07);

	//static 1
	for(ui = 0; ui < MSG_STATIC1_LENGHT; ui++)
		msg[Lenght++] = MSG_STATIC1[ui];

	//mask byte
	msg[Lenght++] = _msgData->MaskByte;

	//fan speed
	msg[Lenght++] = _msgData->Fan_speed;

	//fan on/off
	msg[Lenght++] = _msgData->Fan_Mode;

	//static 2
	for(ui = 0; ui < MSG_STATIC2_LENGHT; ui++)
		msg[Lenght++] = MSG_STATIC2[ui];

	//power on off byte
	msg[Lenght++] = _msgData->PowerOnOff;

	//static 3
	for(ui = 0; ui < MSG_STATIC3_LENGHT; ui++)
		msg[Lenght++] = MSG_STATIC3[ui];

	//check sum
	unsigned int Cal = 0x0000;
	for(ui = 0; ui < Lenght;)
	{
		Cal = ((msg[ui++] & 0xff) << 8);
		if(ui < Lenght)
			Cal |= msg[ui++] & 0xff;
		CheckSum += Cal;
	}

	CheckSum = ((0xFFFF - (CheckSum & 0xFFFF)) & 0xFFFF) - ((CheckSum & 0xFF0000) >> 16);
	msg[Lenght++] = (CheckSum >> 8) & 0xff;
	msg[Lenght++] = CheckSum & 0xff;

	*msgLenght = Lenght;

	return msg;
}




/******************************************************************************
 * Function		: unsigned char *MessageHandle_NwkParking( msgData_t *_msgData, unsigned char *msgLenght )
 * Description	: message parking for nwk transmit
 * Param		: _msgData - message data struct
 * return		: message string
 * ***************************************************************************/
unsigned char *MessageHandle_NwkParking( msgData_t *_msgData, unsigned char *msgLenght )
{
	unsigned char *msg = calloc(MSG_MAX_LENGHT + 1, sizeof(unsigned char));
	unsigned char Lenght = 0;
	unsigned char ui = 0;

	//header
	for(ui = 0; ui < strlen(MESSAGE_TYPEPACKET_DATA); ui++)
		msg[Lenght++] = MESSAGE_TYPEPACKET_DATA[ui];
	msg[Lenght++] = ',';

	//ac mode
	for(ui = 0; ui < strlen(MESSAGE_FUNC_CTRL_POWER); ui++)
		msg[Lenght++] = MESSAGE_FUNC_CTRL_POWER[ui];
	msg[Lenght++] = ':';
	if(_msgData->PowerOnOff == power_on)
	{
		for(ui = 0; ui < strlen(MESSAGE_POWER_ON); ui++)
			msg[Lenght++] = MESSAGE_POWER_ON[ui];
	}
	else if(_msgData->PowerOnOff == power_off)
	{
		for(ui = 0; ui < strlen(MESSAGE_POWER_OFF); ui++)
			msg[Lenght++] = MESSAGE_POWER_OFF[ui];
	}
	msg[Lenght++] = ',';

	//ac mode
	for(ui = 0; ui < strlen(MESSAGE_FUNC_CTRL_MODE); ui++)
		msg[Lenght++] = MESSAGE_FUNC_CTRL_MODE[ui];
	msg[Lenght++] = ':';

	char c_mode[16] = {0};
	if(_msgData->Mode == Mode_Cold)
		sprintf(c_mode, "%s", MESSAGE_AC_MODE_COLD);
	else if(_msgData->Mode == Mode_Dry)
		sprintf(c_mode, "%s", MESSAGE_AC_MODE_DRY);
	else if(_msgData->Mode == Mode_Heat)
		sprintf(c_mode, "%s", MESSAGE_AC_MODE_HEAT);
	//if(_msgData->Mode == Mode_Auto)
	else
		sprintf(c_mode, "%s", MESSAGE_AC_MODE_AUTO);
	for(ui = 0; ui < strlen(c_mode); ui++)
		msg[Lenght++] = c_mode[ui];
	msg[Lenght++] = ',';

	//fan mode
	for(ui = 0; ui < strlen(MESSAGE_FUNC_CTRL_FAN_MODE); ui++)
		msg[Lenght++] = MESSAGE_FUNC_CTRL_FAN_MODE[ui];
	msg[Lenght++] = ':';

	if(_msgData->Fan_Mode == Fan_on)
	{
		for(ui = 0; ui < strlen(MESSAGE_FAN_MODE_ON); ui++)
			msg[Lenght++] = MESSAGE_FAN_MODE_ON[ui];
	}
	else
	{
		for(ui = 0; ui < strlen(MESSAGE_FAN_MODE_OFF); ui++)
			msg[Lenght++] = MESSAGE_FAN_MODE_OFF[ui];
	}
	msg[Lenght++] = ',';

	//fan speed
	for(ui = 0; ui < strlen(MESSAGE_FUNC_CTRL_FAN_SPEED); ui++)
		msg[Lenght++] = MESSAGE_FUNC_CTRL_FAN_SPEED[ui];
	msg[Lenght++] = ':';

	char cStr[16] = {0};
	if(_msgData->Fan_speed == f_speed_low)
		sprintf(cStr, "%s", MESSAGE_FAN_SPEED_LOW);
	else if(_msgData->Fan_speed == f_speed_medium)
		sprintf(cStr, "%s", MESSAGE_FAN_SPEED_MEDIUM);
	else if(_msgData->Fan_speed == f_speed_high)
		sprintf(cStr, "%s", MESSAGE_FAN_SPEED_HIGH);
	else if(_msgData->Fan_speed == f_speed_auto)
		sprintf(cStr, "%s", MESSAGE_FAN_SPEED_AUTO);
	for(ui = 0; ui < strlen(cStr); ui++)
		msg[Lenght++] = cStr[ui];
	msg[Lenght++] = ',';

	//temp
	for(ui = 0; ui < strlen(MESSAGE_FUNC_CTRL_TEMP); ui++)
		msg[Lenght++] = MESSAGE_FUNC_CTRL_TEMP[ui];
	msg[Lenght++] = ':';
	msg[Lenght++] = (_msgData->Temp / 10) | 0x30;
	msg[Lenght++] = (_msgData->Temp % 10) | 0x30;

	msg[Lenght++] = '!';

	*msgLenght = Lenght;
	return msg;
}



















