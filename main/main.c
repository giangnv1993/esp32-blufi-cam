
/* Simple WiFi Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include "main.h"

#include "app/app.h"



extern wifi_info_t _WifiInfo;

Flag_t xFlag = {false};


/*****************************************************************************
 *
 * ****************************************************************************/

void app_main()
{
    printf("\r\n--- APP_MAIN: BLUFI PROJECT %s......\r\n", __DATE__);
    printf("\r\n--- APP_MAIN: Free memory: %d bytes\r\n", esp_get_free_heap_size());
    //Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
	  ESP_ERROR_CHECK(nvs_flash_erase());
	  ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	//GPIO
//	GPIO_Init();



//	esp_vfs_spiffs_conf_t conf = {
//	  .base_path = "/spiffs",
//	  .partition_label = NULL,
//	  .max_files = 5,
//	  .format_if_mount_failed = true
//	};
//
//	esp_vfs_spiffs_register(&conf);
//	vUserFile_LoadInfo_WifiStation(&_WifiInfo);
//	printf("--- WifiTask: Station Username = %s, Passwork = %s\r\n", _WifiInfo.UserName, _WifiInfo.Password);

	/*1. Wifi start*/
	/*2. Timer init and start*/
	// UserTimer_Init();

	app_init(NULL);
	

}












