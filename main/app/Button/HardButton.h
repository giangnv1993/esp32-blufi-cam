#ifndef _VSM_HARDBUTTON_H_
#define _VSM_HARDBUTTON_H_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"


#define MAX_BTN_SUPPORT                     (16u)
#define PERIOD_SCAN_IN_MS                   (20)
#define BUTTON_HOLD_TIME                    (2000)
#define BUTTON_ON_HOLD_TIME_FIRE_EVENT      (400)
#define IDLE_TIME_COUNT_IN_MS               (30000)
#define BUTTON_DOUBLE_CLICK_TIME            (500)

/**
 * Enum to define the event type of hard button
 */
typedef enum
{
    E_EVENT_HARD_BUTTON_PRESS = 0,
    E_EVENT_HARD_BUTTON_RELEASE,
    E_EVENT_HARD_BUTTON_HOLD,
    E_EVENT_HARD_BUTTON_ON_HOLD,
    E_EVENT_HARD_BUTTON_DOUBLE_CLICK,
    E_EVENT_HARD_BUTTON_TRIPLE_CLICK,
    E_EVENT_HARD_ILDE,
    E_EVENT_HARD_ILDE_BREAK,
    E_EVENT_HARD_MAX
} eHardButtonEventType;


typedef struct
{
	gpio_config_t           	gpio_cfg;
    uint32_t                    u8BtnPin;
    uint8_t                     u8BtnLastState;
    uint32_t                    u32IntNo;
    uint32_t                    u32IdleLevel;
} tsButtonHwConfig;

/**
 * Event callback function type
 * button idx, event type, custom data
 */
typedef void (*pHardButtonEventHandler)(int, int, void*);

void vHardButtonInit(tsButtonHwConfig* params, uint8_t u8BtnCount);

void vHardButtonSetCallback(eHardButtonEventType event, pHardButtonEventHandler cb, void* data);

#endif
