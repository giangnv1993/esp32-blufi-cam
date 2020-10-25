/*
 * blufi.h
 *
 *  Created on: Sep 20, 2020
 *      Author: admin
 */

#ifndef MAIN_APP_SMARTCONFIG_BLUFI_H_
#define MAIN_APP_SMARTCONFIG_BLUFI_H_


#include <stdint.h>

typedef struct
{
	char UserName[32];
	char Password[64];
}wifi_info_t;


#pragma once

#define BLUFI_EXAMPLE_TAG "BLUFI_EXAMPLE"
#define BLUFI_INFO(fmt, ...)   ESP_LOGI(BLUFI_EXAMPLE_TAG, fmt, ##__VA_ARGS__)
#define BLUFI_ERROR(fmt, ...)  ESP_LOGE(BLUFI_EXAMPLE_TAG, fmt, ##__VA_ARGS__)

void blufi_dh_negotiate_data_handler(uint8_t *data, int len, uint8_t **output_data, int *output_len, bool *need_free);
int blufi_aes_encrypt(uint8_t iv8, uint8_t *crypt_data, int crypt_len);
int blufi_aes_decrypt(uint8_t iv8, uint8_t *crypt_data, int crypt_len);
uint16_t blufi_crc_checksum(uint8_t iv8, uint8_t *data, int len);

int blufi_security_init(void);
void blufi_security_deinit(void);

void blufi_wifi_start(char* ssid, char* pass);

void blufi_wifi_stop(void);

void blufi_wifi_deinit(void);

void blufi_wifi_init(char* ssid, char* password);

void blufi_register_status(void status_cb(uint32_t/*stt*/));

// void initialise_wifi(char* ssid, char* password);


#endif /* MAIN_APP_SMARTCONFIG_BLUFI_H_ */
