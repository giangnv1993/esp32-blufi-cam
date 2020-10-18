/* esp_timer (high resolution timer) example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "UserTimer.h"


#define TIMER_INTERVAL			2000 /*1ms*/

static void Timer_msec_handler(void* arg);

void (*pfnTimer_mSec)(void);

int cnt_ms = 0;

esp_timer_handle_t oneshot_timer;

void UserTimer_Init()
{

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &Timer_msec_handler,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    /* The timer has been created but is not running yet */


    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, TIMER_INTERVAL));

//    /* Clean up and finish the example */
//    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer));
//    ESP_ERROR_CHECK(esp_timer_delete(periodic_timer));
}



void Timer_RegisterCallBack(void timer_cb(void))
{
	if(timer_cb != NULL)
		pfnTimer_mSec = timer_cb;
}



static void Timer_msec_handler(void* arg)
{
//	cnt_ms ++;
//	if(cnt_ms >= 1000)
//	{
//		cnt_ms = 0;
//		printf("enter timer msec\r\n");
//	}
	pfnTimer_mSec();
}










