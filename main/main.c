
/* Simple WiFi Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include "main.h"

#include "app/app.h"

#include "app/device_info/device_info.h"

AppConfig_t AppConfig;

Flag_t xFlag = {false};

device_data_t device_data = {0};

/*****************************************************************************
 *
 * ****************************************************************************/

void app_main()
{

	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	deviceinfo_show();

	device_info_get_mac(AppConfig.ID_Device);

	app_init(&AppConfig);

}










