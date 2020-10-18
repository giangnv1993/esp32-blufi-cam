/*
 * ACInf.h
 *
 *  Created on: Mar 4, 2019
 *      Author: thaonm1
 */

#ifndef ACINF_H_
#define ACINF_H_

#include <stdio.h>

#define AC_DEBUG

#ifdef AC_DEBUG
#define AC_INFO_DEBUG					printf
#else
#define AC_INFO_DEBUG
#endif


#define MAX_BUFFER_LENGHT				128



#define EVENT_IDLE						0x0000
#define EVENT_AC_CHECK_CONNECTION		0x0001
#define EVENT_WIFI_GET_AC_INFO			0x0002
#define EVENT_WIFI_GET_AC_STATUS		0x0004
#define EVENT_CONTROL_WAIT_TIMEOUT		0x0008




void (*ACInf_Delay_ms)( const unsigned int /*milisecond*/ );
void (*ACInf_UartTransmit)( unsigned char */*TXBuffer*/, unsigned char /*TXLenght*/ );
void (*ACInf_NWKTransmit)( unsigned char */*TXBuffer*/, unsigned char /*TXLenght*/ );
void (*AC_Smart_Cfg)( void );

void (*LedStatus_On)( void );
void (*LedStatus_Off)( void );


/******************************************************************************
 * Function		: void ACInf_Receive_CB( unsigned char *RXBuffer, int RXLenght, bool fromUart )
 * Description	: data received callback
 * Param		:
 * 				+ RXBuffer - buffer data
 * 				+ RXLenght - lenght of buffer
 * 				+ fromUart - data received from uart, false - from network
 * return		: None
 * ***************************************************************************/
void ACInf_Receive_CB( unsigned char *RXBuffer, unsigned char RXLenght, bool fromUart );



void ACInf_Timer_Counter( void );



void ACInf_WifiStatus( bool Status );

void ACInf_MqttStatus( bool Status );




/******************************************************************************
 * Function		: void ACInf_Process( void )
 * Description	: ac interface process
 * Param		: None
 * return		: None
 * ***************************************************************************/
void ACInf_Process( void );



#endif /* ACINF_H_ */








