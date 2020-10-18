/*
 * User_Uart.c
 *
 *  Created on: Mar 4, 2019
 *      Author: giangnv3
 */

#include "UserUart.h"


uart_port_t user_uart = UART_NUM_2;
/******************************************************************************
 *
 ******************************************************************************/
void UserUart_Config(uart_port_t uart_port, uint32_t baudrate)
{
	uart_config_t uart_config = {
	        .baud_rate = baudrate,
	        .data_bits = UART_DATA_8_BITS,
	        .parity    = UART_PARITY_EVEN,
	        .stop_bits = UART_STOP_BITS_1,
	        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	    };
	    uart_param_config(uart_port, &uart_config);
	    uart_set_pin(uart_port, UART2_TX_PIN, UART2_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	    uart_driver_install(uart_port, BUFF_SIZE * 2, 0, 0, NULL, 0);
}
/******************************************************************************
 *
 ******************************************************************************/
int UserUart_ReadData(unsigned char* buff)
{
	int len = 0;
	len = uart_read_bytes(user_uart, buff, BUFF_SIZE, 20/portTICK_RATE_MS); // @suppress("Symbol is not resolved")
	return len;
}
/******************************************************************************
 *
 ******************************************************************************/
void UserUart_WriteData(unsigned char* buf, unsigned char size)
{
	char Hex[17] = "0123456789ABCDEF";
	printf("\r\n--- UserUart_WriteData: Sending data = ");
	for(unsigned char ui = 0; ui < size; ui++)
	{
		unsigned char p = buf[ui];
		printf("%c%c", Hex[p / 16], Hex[p % 16]);
	}
	printf("\r\n");
	uart_write_bytes( user_uart, (const char*)buf, (size_t)size);
}
/******************************************************************************
 *
 ******************************************************************************/
void UserUart_UartRx(unsigned char* data_rev, unsigned char len, bool from_uart)
{
	printf("\r\n--- UserUart_UartRx: data = %s, len = %d\r\n", data_rev, len);
	memset(data_rev, 0x00, len);
}
/******************************************************************************
 *
 ******************************************************************************/
void UserUart_UartTX(unsigned char* data_tx, unsigned char len)
{
	printf("\r\n--- UserUart_UartTXCB: Sending data....\r\n");
	UserUart_WriteData(data_tx, len);
}







