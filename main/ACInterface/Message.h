/*
 * Control.h
 *
 *  Created on: Mar 6, 2019
 *      Author: thaonm1
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_


/******************************************************************************
 * Function		: unsigned char **Message_Process( unsigned char *RXBuffer, unsigned char RXLenght, bool fromUart )
 * Description	: message process
 * Param		:
 * 				+ RXBuffer - buffer data
 * 				+ RXLenght - lenght of buffer
 * 				+ fromUart - data from uart port
 * return		: None
 * ***************************************************************************/
bool Message_Process( unsigned char *RXBuffer, unsigned char RXLenght, bool fromUart );



#endif /* MESSAGE_H_ */
