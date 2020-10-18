/*
 * Control.c
 *
 *  Created on: Mar 6, 2019
 *      Author: thaonm1
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "MessageHandle.h"


//data position
#define MESSAGE_DATA_TEMP_SWING_POS		10
#define MESSAGE_DATA_MASK_POS			12
#define MESSAGE_DATA_FAN_SPEED_POS		13
#define MESSAGE_DATA_FAN_MODE_POS		14
#define MESSAGE_DATA_POWER_POS			18
#define MESSAGE_DATA_MODE_POS			15



extern void (*ACInf_UartTransmit)( unsigned char */*TXBuffer*/, int /*TXLenght*/ );
extern void (*ACInf_NWKTransmit)( unsigned char */*TXBuffer*/, int /*TXLenght*/ );


static void Message_Control( unsigned char *Func, unsigned char *Value );
static void Message_Data( unsigned char *Buffer, unsigned char Lenght );


/*
 * 	unsigned char	Mode;
	unsigned char	Temp;
	unsigned char	Swing;
	unsigned char	MaskByte;
	unsigned char	Fan_Mode;
	unsigned char	Fan_speed;
	unsigned char	PowerOnOff;
	unsigned char	CheckSum[2];*/
msgData_t _msgData =
{
	.Mode		= Mode_Cold,
	.Temp		= 25,
	.Swing		= swing_on,
	.MaskByte	= 0x00,
	.Fan_Mode	= Fan_on,
	.Fan_speed	= f_speed_auto,
	.PowerOnOff = power_on,
};





/******************************************************************************
 * Function		: unsigned char **Message_Process( unsigned char *RXBuffer, unsigned char RXLenght, bool fromUart )
 * Description	: message process
 * Param		:
 * 				+ RXBuffer - buffer data
 * 				+ RXLenght - lenght of buffer
 * 				+ fromUart - data from uart port
 * return		: None
 * ***************************************************************************/
bool Message_Process( unsigned char *RXBuffer, unsigned char RXLenght, bool fromUart )
{
	if(fromUart == true)
	{
		Message_Data( RXBuffer, RXLenght );
	}
	else
	{
		unsigned char NumF = 0;
		unsigned char **Array = MessageHandle_Parse(RXBuffer, RXLenght, &NumF);
		if(NumF < 2)
			return false;

		//lenh dieu khien
		if(strcmp((char *)Array[0], MESSAGE_TYPEPACKET_CTRL) == 0)
		{
			Message_Control( Array[1], Array[2] );
		}
	}
	return true;
}



/******************************************************************************
 * Function		: void Message_Control( char **Array )
 * Description	: control
 * Param		: None
 * return		: None
 * ***************************************************************************/
static void Message_Data( unsigned char *Buffer, unsigned char Lenght )
{
	unsigned char TXLenght = 0;
	_msgData.Temp 		= ((Buffer[MESSAGE_DATA_TEMP_SWING_POS] >> 3) & 0x1F) + 8;
	_msgData.Swing 		= Buffer[MESSAGE_DATA_TEMP_SWING_POS] & 0x07;
	_msgData.MaskByte 	= Buffer[MESSAGE_DATA_MASK_POS] & 0xFF;
	_msgData.Fan_speed 	= Buffer[MESSAGE_DATA_FAN_SPEED_POS] & 0xF0;
	_msgData.Fan_Mode 	= Buffer[MESSAGE_DATA_FAN_MODE_POS] & 0x20;
	_msgData.PowerOnOff	= Buffer[MESSAGE_DATA_POWER_POS] & 0x20;
	_msgData.Mode		= Buffer[MESSAGE_DATA_MODE_POS] & 0xE0;

	unsigned char *msg = MessageHandle_NwkParking( &_msgData, &TXLenght );
	ACInf_NWKTransmit( msg, TXLenght );
}




/******************************************************************************
 * Function		: void Message_Control( unsigned char *Func, unsigned char *Value )
 * Description	: control
 * Param		: None
 * return		: None
 * ***************************************************************************/
static void Message_Control( unsigned char *Func, unsigned char *Value )
{
	if(strcmp((char *)Func, MESSAGE_FUNC_CTRL_MODE) == 0)
	{
		if(strcmp((char *)Value, MESSAGE_AC_MODE_AUTO) == 0)
			_msgData.Mode = Mode_Auto;
		else if(strcmp((char *)Value, MESSAGE_AC_MODE_COLD) == 0)
			_msgData.Mode = Mode_Cold;
		else if(strcmp((char *)Value, MESSAGE_AC_MODE_DRY) == 0)
			_msgData.Mode = Mode_Dry;
		else if(strcmp((char *)Value, MESSAGE_AC_MODE_HEAT) == 0)
			_msgData.Mode = Mode_Heat;
	}
	else if(strcmp((char *)Func, MESSAGE_FUNC_CTRL_TEMP) == 0)
	{
		_msgData.Temp = atoi((char *)Value);
	}
	else if(strcmp((char *)Func, MESSAGE_FUNC_CTRL_FAN_MODE) == 0)
	{
		if(strcmp((char *)Value, MESSAGE_FAN_MODE_ON) == 0)
			_msgData.Fan_Mode = Fan_on;
		else if(strcmp((char *)Value, MESSAGE_FAN_MODE_OFF) == 0)
			_msgData.Fan_Mode = Fan_off;
	}
	else if(strcmp((char *)Func, MESSAGE_FUNC_CTRL_FAN_SPEED) == 0)
	{
		if(strcmp((char *)Value, MESSAGE_FAN_SPEED_LOW) == 0)
			_msgData.Fan_speed = f_speed_low;
		else if(strcmp((char *)Value, MESSAGE_FAN_SPEED_MEDIUM) == 0)
			_msgData.Fan_speed = f_speed_medium;
		else if(strcmp((char *)Value, MESSAGE_FAN_SPEED_HIGH) == 0)
			_msgData.Fan_speed = f_speed_high;
		else if(strcmp((char *)Value, MESSAGE_FAN_SPEED_AUTO) == 0)
			_msgData.Fan_speed = f_speed_auto;
	}
	else if(strcmp((char *)Func, MESSAGE_FUNC_CTRL_POWER) == 0)
	{
		if(strcmp((char *)Value, MESSAGE_POWER_ON) == 0)
			_msgData.PowerOnOff = power_on;
		else if(strcmp((char *)Value, MESSAGE_POWER_OFF) == 0)
			_msgData.PowerOnOff = power_off;
	}

	unsigned char msgLenght = 0;
	unsigned char *msg = MessageHandle_UartParking( &_msgData, &msgLenght );
	ACInf_UartTransmit( msg, msgLenght );
}


















