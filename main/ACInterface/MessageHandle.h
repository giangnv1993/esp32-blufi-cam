/*
 * Parse.h
 *
 *  Created on: Mar 6, 2019
 *      Author: thaonm1
 */

#ifndef MESSAGEHANDLE_H_
#define MESSAGEHANDLE_H_


#define MSG_MAX_LENGHT					128


//type packet
#define MESSAGE_TYPEPACKET_CTRL			"ctrl"
#define MESSAGE_TYPEPACKET_DATA			"data"

//Function mode
#define MESSAGE_FUNC_CTRL_MODE			"mode"
#define MESSAGE_FUNC_CTRL_TEMP			"temp"
#define MESSAGE_FUNC_CTRL_FAN_MODE		"f_mode"
#define MESSAGE_FUNC_CTRL_FAN_SPEED		"f_speed"
#define MESSAGE_FUNC_CTRL_POWER			"power"


/****************************** Function mode *******************************/
//ac mode
#define MESSAGE_AC_MODE_COLD			"cold"
#define MESSAGE_AC_MODE_HEAT			"heat"
#define MESSAGE_AC_MODE_DRY				"dry"
#define MESSAGE_AC_MODE_AUTO			"auto"

enum
{
	Mode_Cold	= 0x20,
	Mode_Heat	= 0x60,
	Mode_Dry	= 0x40,
	Mode_Auto	= 0xC0
}Func_Mode;
/****************************************************************************/


/******************************** power mode ********************************/
//ac mode
#define MESSAGE_POWER_ON				"on"
#define MESSAGE_POWER_OFF				"off"

enum
{
	power_on	= 0x20,
	power_off	= 0x00,
}Power_Mode;
/****************************************************************************/


/**************************** Fan mode and speed ****************************/
//Fan mode
#define MESSAGE_FAN_MODE_ON				"on"
#define MESSAGE_FAN_MODE_OFF			"off"

//Fan speed
#define MESSAGE_FAN_SPEED_LOW			"low"
#define MESSAGE_FAN_SPEED_MEDIUM		"medium"
#define MESSAGE_FAN_SPEED_HIGH			"hight"
#define MESSAGE_FAN_SPEED_AUTO			"auto"

enum
{
	Fan_on	= 0x00,
	Fan_off	= 0x80,
}Fan_Mode_Value;


enum
{
	f_speed_auto 	= 0xA0,
	f_speed_low		= 0x60,
	f_speed_medium 	= 0x40,
	f_speed_high 	= 0x20,
}Fan_Speed_Value;
/****************************************************************************/


/************************************ Swing *********************************/
//Swing mode
#define MESSAGE_SWING_MODE_ON			"on"
#define MESSAGE_SWING_MODE_Off			"off"

enum
{
	swing_on	= 0x07,
	swing_off	= 0x00,
}Swing_Mode_Value;
/****************************************************************************/



typedef struct
{
	unsigned char	Mode;
	unsigned char	Temp;
	unsigned char	Swing;
	unsigned char	MaskByte;
	unsigned char	Fan_Mode;
	unsigned char	Fan_speed;
	unsigned char	PowerOnOff;
	unsigned char	CheckSum[2];
}msgData_t;




/******************************************************************************
 * Function		: unsigned char **MessageHandle_Parse( unsigned char *RXBuffer, unsigned char RXLenght, unsigned char *FieldNumber )
 * Description	: message parse
 * Param		:
 * 				+ RXBuffer - buffer data
 * 				+ RXLenght - lenght of buffer
 * 				+ FieldNumber - number of field data
 * return		: None
 * ***************************************************************************/
unsigned char **MessageHandle_Parse( unsigned char *RXBuffer, unsigned char RXLenght, unsigned char *FieldNumber );



/******************************************************************************
 * Function		: unsigned char *MessageHandle_UartParking( msgNwkCtrl_t *_msgNwkCtrl, unsigned char *msgLenght )
 * Description	: message parking for uart transmit
 * Param		: _msgNwkCtrl - message control struct
 * return		: message string
 * ***************************************************************************/
unsigned char *MessageHandle_UartParking( msgData_t *_msgNwkCtrl, unsigned char *msgLenght );



/******************************************************************************
 * Function		: unsigned char *MessageHandle_NwkParking( msgData_t *_msgData, unsigned char *msgLenght )
 * Description	: message parking for nwk transmit
 * Param		: _msgData - message data struct
 * return		: message string
 * ***************************************************************************/
unsigned char *MessageHandle_NwkParking( msgData_t *_msgData, unsigned char *msgLenght );


#endif /* MESSAGEHANDLE_H_ */










