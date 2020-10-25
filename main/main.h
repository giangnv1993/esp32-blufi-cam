/*
 * ESP32_AC.h
 *
 *  Created on: Mar 4, 2019
 *      Author: giangnv3
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <sys/unistd.h>
#include <sys/stat.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_event_legacy.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "tcpip_adapter.h"
#include "driver/gpio.h"

#include "esp_partition.h"

#include "esp_spiffs.h"

//#include "os/os_cputime.h"


//===================


//typedef enum
//{
//	eIdle = 0,
//	eConnected,
//	eDisconnect
//}eStatus;



#endif /* MAIN_MAIN_H_ */







