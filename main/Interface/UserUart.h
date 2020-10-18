/*
 * User_Uart.h
 *
 *  Created on: Mar 4, 2019
 *      Author: giangnv3
 */

#ifndef INTERFACE_USERUART_H_
#define INTERFACE_USERUART_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"


#define UART_PORT        		(UART_NUM_1)
#define BUFF_SIZE         		(512)
#define UART0_RX_PIN     		(3)
#define UART0_TX_PIN     		(1)

#define UART1_RX_PIN     		(9)
#define UART1_TX_PIN     		(10)

#define UART2_RX_PIN     		(16)
#define UART2_TX_PIN     		(17)

#define UART_BAUD_4800 			(4800)
#define UART_BAUD_115200 		(115200)



void UserUart_Config(uart_port_t uart_port, uint32_t baudrate);

int UserUart_ReadData(unsigned char* buff);

void UserUart_WriteData(unsigned char* buf, unsigned char size);

void (*UserUart_UartRxCallBack)(unsigned char*/*data_rx*/, unsigned char/*len_data*/, bool/*uart_des*/);

//void (*UserUart_UartTxCallBack)(unsigned char*/*data_tx*/, unsigned char/*len_data*/);


/*******************************************************************************
for user
 ******************************************************************************/
void UserUart_UartRx(unsigned char* data_rev, unsigned char len, bool from_uart);

void UserUart_UartTX(unsigned char* data_tx, unsigned char len);

#endif /* INTERFACE_USERUART_H_ */
