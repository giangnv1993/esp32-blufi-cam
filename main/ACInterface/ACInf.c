/*
 * ACInf.c
 *
 *  Created on: Mar 4, 2019
 *      Author: thaonm1
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ACInf.h"
#include "Message.h"


//smart config start
#define AC_SMART_CONFIG_LENGHT					11
unsigned char AC_Smart_Config[] = {0xBB, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x38, 0xFF};

//ac check wifi connection
#define AC_CHECK_CONNECTION_LENGHT				10
unsigned char AC_Check_Connection[AC_CHECK_CONNECTION_LENGHT + 1] = {0xBB, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0xFF};

//wifi feedback connection
#define WIFI_FEEDBACK_CONNECTION_WAIT_TIME		85
#define WIFI_FEEDBACK_CONNECTION_LENGHT			18
unsigned char WIFI_Feedback_Connection[WIFI_FEEDBACK_CONNECTION_LENGHT + 1] = \
					{0xBB, 0x00, 0x01, 0x80, 0x01, 0x00, 0x08, 0x00, 0x24, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0xDB};

//wifi check ac info
#define WIFI_GET_AC_INFO_WAIT_TIME				1100
#define WIFI_GET_AC_INFO_LENGHT					12
unsigned char WIFI_Get_AC_Info[WIFI_GET_AC_INFO_LENGHT + 1] = \
					{0xBB, 0x00, 0x06, 0x80, 0x00, 0x00, 0x02, 0x00, 0x21, 0x01, 0x1B, 0x7E};

//wifi get ac status
#define WIFI_GET_AC_STATUS_WAIT_TIME			250
#define WIFI_GET_AC_STATUS_LENGHT				12
unsigned char WIFI_Get_AC_Status[WIFI_GET_AC_STATUS_LENGHT + 1] = \
					{0xBB, 0x00, 0x06, 0x80, 0x00, 0x00, 0x02, 0x00, 0x11, 0x01, 0x2B, 0x7E};

//ac feedback status
#define AC_FEEDBACK_INFO_HEADER_LENGHT			7
unsigned char AC_Feedback_Info_Header[AC_FEEDBACK_INFO_HEADER_LENGHT + 1] = {0xBB, 0x00, 0x07, 0x00, 0x00, 0x00, 0x18};

//ac feedback info
#define AC_FEEDBACK_STATUS_HEADER_LENGHT		7
unsigned char AC_Feedback_Status_Header[AC_FEEDBACK_STATUS_HEADER_LENGHT + 1] = {0xBB, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0F};

//wifi received data fro network
#define AC_RECV_DATA_NWK_HEADER_LENGHT			7
unsigned char WIFI_RecvData_Nwk_Header[AC_RECV_DATA_NWK_HEADER_LENGHT + 1] = {0xBB, 0x00, 0x06, 0x80, 0x00, 0x00, 0x0F};

#define AC_CONTROL_WAIT_TIMEOUT					200


unsigned char RX_Buffer[MAX_BUFFER_LENGHT] = {0};
unsigned char RX_Lenght = 0;

unsigned int AC_EVENT = EVENT_IDLE;
unsigned long msCount = 0;
unsigned long msCountOld = 0;

//led status
unsigned int LedCount = 0;
unsigned int LedOn = 0;
unsigned int LedOff = 0;
#define LED_ON_WIFI_CONNECTED				1000
#define LED_OFF_WIFI_CONNECTED				1000
#define LED_ON_MQTT_CONNECTED				300
#define LED_OFF_MQTT_CONNECTED				300
#define LED_ON_WIFI_DISCONNECTED			50
#define LED_OFF_WIFI_DISCONNECTED			2000



static bool ACInf_Check_Message( unsigned char *StaticData, unsigned char sDataLenght, unsigned char *Message, unsigned char msgLenght );
static bool Wait_forMe( unsigned long ms );
char *ConvertHex(char *Ptr, int Lenght);




/******************************************************************************
 * Function		: void ACInf_MessageHandle( void )
 * Description	: message handle
 * Param		: None
 * return		: None
 * ***************************************************************************/
static void ACInf_MessageHandle( void )
{
	if(RX_Lenght == 0)
		return;

	//ac check connection
	if( ACInf_Check_Message( AC_Smart_Config, AC_SMART_CONFIG_LENGHT, RX_Buffer, RX_Lenght ) == true )
	{
		AC_INFO_DEBUG("--- ACInf: smart config start\r\n");
		AC_Smart_Cfg();
	}
	//ac check connection
	else if( ACInf_Check_Message( AC_Check_Connection, AC_CHECK_CONNECTION_LENGHT, RX_Buffer, RX_Lenght ) == true )
	{
		AC_INFO_DEBUG("--- ACInf: ac check status\r\n");
		AC_EVENT |= EVENT_AC_CHECK_CONNECTION;
	}
	//ac info feedback
	else if( ACInf_Check_Message( RX_Buffer, RX_Lenght, AC_Feedback_Info_Header, AC_FEEDBACK_INFO_HEADER_LENGHT ) == true )
	{
		AC_INFO_DEBUG("--- ACInf: ac info feedback\r\n");
		AC_EVENT |= EVENT_WIFI_GET_AC_STATUS;
	}
	//ac status feedback
	else if( ACInf_Check_Message( RX_Buffer, RX_Lenght, AC_Feedback_Status_Header, AC_FEEDBACK_STATUS_HEADER_LENGHT ) == true )
	{
		AC_INFO_DEBUG("--- ACInf: ac feedback status\r\n");
		Message_Process( RX_Buffer, RX_Lenght, true );
	}
	//received data from network
	else //if( ACInf_Check_Message( RX_Buffer, RX_Lenght, WIFI_RecvData_Nwk_Header, AC_RECV_DATA_NWK_HEADER_LENGHT ) == true )
	{
		AC_INFO_DEBUG("--- ACInf: received data from network\r\n");
//		AC_EVENT |= EVENT_CONTROL_WAIT_TIMEOUT;
		Message_Process( RX_Buffer, RX_Lenght, false );
	}
	memset(RX_Buffer, 0, RX_Lenght + 1);
	RX_Lenght = 0;
}




/******************************************************************************
 * Function		: void ACInf_EventProcess( void )
 * Description	: process event
 * Param		: None
 * return		: None
 * ***************************************************************************/
static void ACInf_EventProcess( void )
{
	//feed back ac check connection
	if( (AC_EVENT & EVENT_AC_CHECK_CONNECTION) == EVENT_AC_CHECK_CONNECTION )
	{
		if( Wait_forMe( WIFI_FEEDBACK_CONNECTION_WAIT_TIME ) == true )
		{
			AC_INFO_DEBUG("--- ACInf: ac send 15DB\r\n");
			ACInf_UartTransmit( WIFI_Feedback_Connection, WIFI_FEEDBACK_CONNECTION_LENGHT );
			AC_EVENT ^= EVENT_AC_CHECK_CONNECTION;
			AC_EVENT |= EVENT_WIFI_GET_AC_INFO;
			msCount = 0;
		}
	}
	//get ac info
	else if( (AC_EVENT & EVENT_WIFI_GET_AC_INFO) == EVENT_WIFI_GET_AC_INFO )
	{
		if( Wait_forMe( WIFI_GET_AC_INFO_WAIT_TIME ) == true )
		{
			AC_INFO_DEBUG("--- ACInf: ac send AC Info\r\n");
			ACInf_UartTransmit( WIFI_Get_AC_Info, WIFI_GET_AC_INFO_LENGHT );
			AC_EVENT ^= EVENT_WIFI_GET_AC_INFO;
			msCount = 0;
		}
	}
	else if( (AC_EVENT & EVENT_WIFI_GET_AC_STATUS) == EVENT_WIFI_GET_AC_STATUS )
	{
		if( Wait_forMe( WIFI_GET_AC_STATUS_WAIT_TIME ) == true )
		{
			AC_INFO_DEBUG("--- ACInf: ac send AC status\r\n");
			ACInf_UartTransmit( WIFI_Get_AC_Status, WIFI_GET_AC_STATUS_LENGHT );
			AC_EVENT ^= EVENT_WIFI_GET_AC_STATUS;
			msCount = 0;
		}
	}
//	else if( (AC_EVENT & EVENT_CONTROL_WAIT_TIMEOUT) == EVENT_CONTROL_WAIT_TIMEOUT )
//	{
//		if( Wait_forMe( AC_CONTROL_WAIT_TIMEOUT ) == true )
//		{
//			ACInf_UartTransmit( WIFI_Get_AC_Status, WIFI_GET_AC_STATUS_LENGHT );
//			AC_EVENT ^= EVENT_WIFI_GET_AC_STATUS;
//			msCount = 0;
//		}
//	}
}



/******************************************************************************
 * Function		: void ACInf_Process( void )
 * Description	: ac interface process
 * Param		: None
 * return		: None
 * ***************************************************************************/
void ACInf_Process( void )
{
	ACInf_MessageHandle();
	ACInf_EventProcess();
}




/******************************************************************************
 * Function		: void ACInf_Receive_CB( unsigned char *RXBuffer, int RXLenght, bool fromUart )
 * Description	: data received callback
 * Param		:
 * 				+ RXBuffer - buffer data
 * 				+ RXLenght - lenght of buffer
 * 				+ fromUart - data received from uart, false - from network
 * return		: None
 * ***************************************************************************/
void ACInf_Receive_CB( unsigned char *RXBuffer, unsigned char RXLenght, bool fromUart )
{
	while(RX_Lenght > 0)
	{
		ACInf_Delay_ms(1);
	}

	if(RXLenght > 0)
	{
		for(unsigned char ui = 0; ui < RXLenght; ui++)
			RX_Buffer[ui] = RXBuffer[ui];
		RX_Lenght = RXLenght;
		if(fromUart == false)
			printf("nwk data received = %s, lenght = %d\r\n", RXBuffer, RXLenght);
		else
		{
			char *msg = ConvertHex((char *)RXBuffer, RXLenght);
			printf("uart data received = %s, lenght = %d\r\n", msg, RXLenght);
		}
	}
}




/******************************************************************************
 * Function		: bool ACData_Check_Message( unsigned char *StaticData, unsigned char sDataLenght, unsigned char *Message, unsigned char msgLenght )
 * Description	: check message is available
 * Param		:
 * 				+ StaticData - static message
 * 				+ sDataLenght - static message lenght
 * 				+ Message - message received
 * 				+ msgLenght - message lenght
 * return		:
 * 				+ true - message ok
 * 				+ false - not ok
 * ***************************************************************************/
static bool ACInf_Check_Message( unsigned char *StaticData, unsigned char sDataLenght, unsigned char *Message, unsigned char msgLenght )
{
	for(unsigned char ui = 0; ui < msgLenght; ui++)
	{
		if(StaticData[ui] != Message[ui])
			return false;
		if(sDataLenght <= ui)
			return false;
	}
	return true;
}



/******************************************************************************
 * Function		: bool Wait_forMe( unsigned long ms )
 * Description	: wait timeout to send data
 * Param		: ms - milisecond
 * return		:
 * 				+ true - timeout
 * 				+ false - waiting
 * ***************************************************************************/
static bool Wait_forMe( unsigned long ms )
{
	if(msCountOld != msCount)
	{
		msCountOld = msCount;
		if(msCount >= ms)
			return true;
	}
	return false;
}


//timer call back
void ACInf_Timer_Counter( void )
{
	msCount++;
	LedCount++;

	if(LedCount < LedOn)
		LedStatus_On();
	else if(LedCount <= (LedOn + LedOff))
		LedStatus_Off();
	else
		LedCount = 0;
}



char Hex[17] = "0123456789ABCDEF";


char *ConvertHex(char *Ptr, int Lenght)
{
	char *Buffer = (char *)calloc(128 * 2 + 16, sizeof(char));
	int ui = 0;
	int Index = 0;

	for (ui = 0; ui < Lenght; ui++)
	{
		if (*Ptr == 0)
		{
			Buffer[Index++] = '0';
			Buffer[Index++] = '0';
		}
		else
		{
			unsigned char p = *Ptr;
			Buffer[Index++] = Hex[p / 16];
			Buffer[Index++] = Hex[p % 16];
		}
		Ptr++;
	}

	return Buffer;
}





void ACInf_WifiStatus( bool Status )
{
	//wifi disconnected
	if(Status == false)
	{
		LedOn 	= LED_ON_WIFI_DISCONNECTED;
		LedOff 	= LED_OFF_WIFI_DISCONNECTED;
	}
	//wifi connected
	else
	{
		LedOn	= LED_ON_WIFI_CONNECTED;
		LedOff	= LED_OFF_WIFI_CONNECTED;
	}
}



void ACInf_MqttStatus( bool Status )
{
	if(Status == true)
	{
		LedOn	= LED_ON_MQTT_CONNECTED;
		LedOff	= LED_OFF_MQTT_CONNECTED;
	}
}



















