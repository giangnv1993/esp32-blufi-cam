/*
 * UartTask.c
 *
 *  Created on: Mar 4, 2019
 *      Author: giangnv3
 */
#include "UartTask.h"
#include "../Interface/UserTimer.h"
#include "../app/app.h"

extern uart_port_t user_uart;


static void UartTask(void *pvParameters);



/******************************************************************************
 *
 ******************************************************************************/
void UartTask_Init(void)
{
//	int result;
//	printf("Creating uart task ....\r\n");
//	result = xTaskCreate(
//			UartTask,
//			( const char* ) "uart task", // @suppress("Type cannot be resolved")
//			512,
//			NULL,
//			tskIDLE_PRIORITY + 5,
//			NULL
//		);
//
//	if(result != pdPASS) { // @suppress("Symbol is not resolved")
//		printf("xTaskCreate failed");
//	}
	xTaskCreate(UartTask, "UartTask", 2*1024, NULL, 8, NULL);

}


/******************************************************************************
 *
 ******************************************************************************/
static void UartTask(void *pvParameters)
{
	uint8_t data[128] = {0};
//	uint8_t test[12] = "hello\r\n";
//	unsigned char Rx_len = 0;
//	vTaskDelay(1000);
//	printf("\r\n---- Uart_Task start with Baudrate = %d ......\r\n", UART_BAUD_4800);
//
//	UserUart_Config(user_uart, UART_BAUD_4800);


	while(1)
	{
//		if((Rx_len = UserUart_ReadData(data)) > 0)
//		{
//			UserUart_UartRxCallBack((unsigned char*)data, Rx_len, true);
//		}
//		vTaskDelay(1);
		if(bDelay_ms(DELAY_DEBUG2, 2000) != false)
			printf(".");
		vTaskDelay(5);

	}
}
/******************************************************************************
 *
 ******************************************************************************/


