#include "HardButton.h"


#define APP_LOGD			printf



static uint8_t u32ButtonData[MAX_BTN_SUPPORT] = {0};
static uint32_t u32ButtonPressEvent;
static uint32_t u32ButtonReleaseEvent;
static uint32_t u32ButtonHoldEvent = 0;
static uint32_t u32ButtonHoldEventExec = 0;
static uint32_t u32ButtonOnHoldEvent = 0;
static uint8_t u8ButtonCount;
//static uint8_t u8ButtonStateUpdateFlag;
static uint32_t pu32ButtonHoldTimeCount[MAX_BTN_SUPPORT] = {0};
static uint32_t pu32ButtonOnHoldTimeCount[MAX_BTN_SUPPORT] = {0};
static uint32_t pu32ButtonLastPressTime[MAX_BTN_SUPPORT] = {0};
static uint8_t u8ButtonPressCount[MAX_BTN_SUPPORT] = {0};
static tsButtonHwConfig *hwParams;
static pHardButtonEventHandler tsCallbackTable[E_EVENT_HARD_MAX] = {0};
static void* pvCustomData[E_EVENT_HARD_MAX] = {0};



static void vHardButtonTask(void* params);

//static void vButtonStateUpdate(uint8_t u8intNo)
//{
//    u8ButtonStateUpdateFlag = 1;
//}

void vHardButtonInit(tsButtonHwConfig* params, uint8_t u8BtnCount)
{
    uint8_t i = 0;

    if (NULL == params)
    {
        APP_LOGD("Invalid init pointer");
        return;
    }

    if (MAX_BTN_SUPPORT < u8BtnCount)
    {
        APP_LOGD("Not support %d buttons", u8BtnCount);
        return;
    }

    hwParams = params;

    u8ButtonCount = u8BtnCount;

    for(i = 0; i < u8BtnCount; i ++)
    {
    	hwParams[i].gpio_cfg.intr_type = GPIO_PIN_INTR_DISABLE;
    	hwParams[i].gpio_cfg.pin_bit_mask = hwParams[i].u8BtnPin;
		//set as input mode
		hwParams[i].gpio_cfg.mode = GPIO_MODE_INPUT;
		//enable pull-up mode
		hwParams[i].gpio_cfg.pull_up_en = 1;
		gpio_config(&hwParams[i].gpio_cfg);
        hwParams[i].u8BtnLastState = hwParams[i].u32IdleLevel = 1;
        u32ButtonData[i] = hwParams[i].u32IdleLevel;
    }

    xTaskCreate(vHardButtonTask, "vHardButtonTask", 2048, NULL, 10, NULL);

}

void vHardButtonTask(void* params)
{
    uint8_t i = 0;

    if (NULL == hwParams)
    {
        APP_LOGD("Invalid init pointer");
        return;
    }

    if (MAX_BTN_SUPPORT < u8ButtonCount)
    {
        APP_LOGD("Not support %d buttons", u8ButtonCount);
        return;
    }

    while (1)
    {

        for(i = 0; i < u8ButtonCount; i ++)
        {
            u32ButtonData[i] = gpio_get_level(hwParams[i].u8BtnPin);
            // APP_LOGD("Stt %d %d", u32ButtonData[i], hwParams[i].u8BtnPin);
            if (u32ButtonData[i] ^ hwParams[i].u8BtnLastState)
            {
                if (u32ButtonData[i] != hwParams[i].u32IdleLevel)
                {
                    u32ButtonPressEvent |= 1 << i;
                    pu32ButtonHoldTimeCount[i] = xTaskGetTickCount();
                }
                else
                {
                   u32ButtonReleaseEvent |= 1 << i;
                   pu32ButtonHoldTimeCount[i] = 0;
                   pu32ButtonOnHoldTimeCount[i] = 0;
                }
            }
            hwParams[i].u8BtnLastState = u32ButtonData[i];
        }
//    }


		for(i = 0; i < u8ButtonCount; i ++)
		{
			if (u32ButtonData[i] != hwParams[i].u32IdleLevel)		//hold
			{
				if (xTaskGetTickCount() - pu32ButtonHoldTimeCount[i] >= BUTTON_HOLD_TIME)
				{
					if (((u32ButtonHoldEvent >> i) & 0x1))
					{
						if (xTaskGetTickCount() - pu32ButtonOnHoldTimeCount[i] >= BUTTON_ON_HOLD_TIME_FIRE_EVENT)
						{
							pu32ButtonOnHoldTimeCount[i] = xTaskGetTickCount();
							u32ButtonOnHoldEvent |= (1 << i);
						}
					}
					else
					{
						pu32ButtonOnHoldTimeCount[i] = xTaskGetTickCount();
						u32ButtonHoldEvent |= (1 << i);
						u32ButtonHoldEventExec &= ~(1 << i); // clear executed flag
					}
				}
			}

			if ((u32ButtonPressEvent >> i) & 0x01)
			{
				APP_LOGD("Hard button press event, button idx [%d]", i);
				if (pu32ButtonLastPressTime[i] == 0)
				{
					pu32ButtonLastPressTime[i] = xTaskGetTickCount();
				}

				if (xTaskGetTickCount() - pu32ButtonLastPressTime[i] >= BUTTON_DOUBLE_CLICK_TIME)
				{
					u8ButtonPressCount[i] = 0;
				}
				else
				{
					u8ButtonPressCount[i] += 1;

					uint32_t u32Event = E_EVENT_HARD_MAX;

					if (u8ButtonPressCount[i] == 1) // double click
					{
						u32Event = E_EVENT_HARD_BUTTON_DOUBLE_CLICK;
					}
					else if (u8ButtonPressCount[i] == 2)
					{
						u32Event = E_EVENT_HARD_BUTTON_TRIPLE_CLICK;
						u8ButtonPressCount[i] = 0;
					}

					if (u32Event != E_EVENT_HARD_MAX)
					{
						APP_LOGD("Hard button event[%d], button idx [%d]", u32Event, i);
						if (NULL != tsCallbackTable[u32Event])
						{
							tsCallbackTable[u32Event](i, u32Event, pvCustomData[u32Event]);
						}
					}
				}

				pu32ButtonLastPressTime[i] = xTaskGetTickCount();

				if (NULL != tsCallbackTable[E_EVENT_HARD_BUTTON_PRESS])
				{
					tsCallbackTable[E_EVENT_HARD_BUTTON_PRESS](i, E_EVENT_HARD_BUTTON_PRESS, pvCustomData[E_EVENT_HARD_BUTTON_PRESS]);
				}
				u32ButtonPressEvent &= ~(1 << i);
			}
			if ((u32ButtonReleaseEvent >> i) & 0x01)
			{
				APP_LOGD("Hard button release event, button idx [%d]", i);
				if (NULL != tsCallbackTable[E_EVENT_HARD_BUTTON_RELEASE])
				{
					tsCallbackTable[E_EVENT_HARD_BUTTON_RELEASE](i, E_EVENT_HARD_BUTTON_RELEASE, pvCustomData[E_EVENT_HARD_BUTTON_RELEASE]);
				}
				u32ButtonReleaseEvent &= ~(1 << i);
				u32ButtonHoldEvent &= ~(1 << i); // clear
			}

			if (!((u32ButtonHoldEventExec >> i) & 0x1) &&  ((u32ButtonHoldEvent >> i) & 0x1))
			{
				u32ButtonHoldEventExec |= (1 << i);
				APP_LOGD("E_EVENT_HARD_BUTTON_HOLD exec event %d %d", i, E_EVENT_HARD_BUTTON_HOLD);
				uint32_t u32HoldTime = xTaskGetTickCount() - pu32ButtonHoldTimeCount[i];
			   if(NULL != tsCallbackTable[E_EVENT_HARD_BUTTON_HOLD]) {
					tsCallbackTable[E_EVENT_HARD_BUTTON_HOLD](i, E_EVENT_HARD_BUTTON_HOLD, (void*)u32HoldTime);
			   }
			}

			if ((u32ButtonOnHoldEvent >> i) & 0x1)
			{
				 uint32_t u32HoldTime = xTaskGetTickCount() - pu32ButtonHoldTimeCount[i];
				APP_LOGD(" E_EVENT_HARD_BUTTON_ON_HOLD exec event %d %d", i, E_EVENT_HARD_BUTTON_ON_HOLD);
			   if(NULL != tsCallbackTable[E_EVENT_HARD_BUTTON_ON_HOLD]) {
					tsCallbackTable[E_EVENT_HARD_BUTTON_ON_HOLD](i, E_EVENT_HARD_BUTTON_ON_HOLD, (void*)u32HoldTime);
			   }
			   u32ButtonOnHoldEvent &= ~(1 << i);
			}
		}
    }
}


void vHardButtonSetCallback(eHardButtonEventType event, pHardButtonEventHandler cb, void* data)
{
    if(NULL != cb) {
        tsCallbackTable[event] = cb;
        pvCustomData[event] = data;
    }
}
