/* MQTT over SSL Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"

#include "MQTTClient.h"
#include "../app.h"
#include "../message/message.h"
#include "../app_debug.h"

static const char *TAG = "MQTTS_EXAMPLE";


static void mqtt_revdata(char* data, uint16_t len, char* topic);

static void mqtt_parser(char* data, uint16_t len);

static void mqtt_publish(char* topic, char* data);

static void mqtt_init(void);

static bool MQTT_Register_SubTopic(char* topic, void msg_rx_cb(char*, uint16_t, uint8_t));

esp_mqtt_client_handle_t client;

#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipse_org_pem_start[]  = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipse_org_pem_start[]   asm("_binary_mqtt_eclipse_org_pem_start");
#endif
extern const uint8_t mqtt_eclipse_org_pem_end[]   asm("_binary_mqtt_eclipse_org_pem_end");


extern AppConfig_t AppConfig;

static uint8_t num_topic = 0;


char topic_rev_data[SIZE_TOPIC_MQTT] = {0};
char data_buff[MQTT_DATA_LENGTH_MAX] = {0};

//
// Note: this function is for testing purposes only publishing the entire active partition
//       (to be checked against the original binary)
//

void (*mqtt_set_status)(uint32_t);

void mqtt_register_status(void status_cb(uint32_t/*stt*/))
{
	if(status_cb != NULL)
		mqtt_set_status = status_cb;
}



static void send_binary(esp_mqtt_client_handle_t client)
{
    spi_flash_mmap_handle_t out_handle;
    const void *binary_address;
    const esp_partition_t* partition = esp_ota_get_running_partition();
    esp_partition_mmap(partition, 0, partition->size, SPI_FLASH_MMAP_DATA, &binary_address, &out_handle);
    int msg_id = esp_mqtt_client_publish(client, "/topic/binary", binary_address, partition->size, 0, 0);
    ESP_LOGI(TAG, "binary sent with msg_id=%d", msg_id);
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
	client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, AppConfig.MQTT_Config.list_topic[INDEX_SUB_TOPIC_LOCK_FORMAT].subtopic, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, AppConfig.MQTT_Config.list_topic[INDEX_SUB_TOPIC_UV_FORMAT].subtopic, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            // msg_id = esp_mqtt_client_subscribe(client, "0987654abd/TIMER", 0);
            // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

//            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
//            ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, AppConfig.MQTT_Config.pubtopic, "online", 0, 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

			memcpy(topic_rev_data, event->topic, event->topic_len);
			memcpy(data_buff, event->data, event->data_len);
            mqtt_revdata(data_buff, event->data_len, topic_rev_data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_ESP_TLS) {
                ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
                ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
                ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
            } else {
                ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}





void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
//        .uri = CONFIG_BROKER_URI,
//        .cert_pem = (const char *)mqtt_eclipse_org_pem_start,
    		.host			= "node02.myqtthub.com",
			.port			= 1883,
			// .client_id		= "987654321",
            .client_id		= AppConfig.ID_Device,
			// .username		= "Y9g5SHe8s - MO8nq",
            .username		= AppConfig.MQTT_Config.username,
			.password		= "",
			.keepalive		= 60,
//			.event_handle	= xMQTTClient_Event_Handler,
    };

    mqtt_init();

    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}





static void mqtt_publish(char* topic, char* data)
{
	esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
}


bool mqtt_msg_received = false;

static void mqtt_revdata(char* data, uint16_t len, char* topic)
{
	#if 0
	if( mqtt_msg_received == true )
		{
			memset((char*)data->message->payload, 0x00, data->message->payloadlen);
			return;
		}

		if(data->message->payloadlen > LEN_MQTT_MAX)
		{
			memset((char*)data->message->payload, 0x00, data->message->payloadlen);
			return;
		}

	mqtt_msg_received = true;
//		VSM_DEBUG("Message arrived on topic %s: %s\n", data->topicName->lenstring.data, (char *)data->message->payload);
	AppConfig.MQTT_Config.received((char *)data->message->payload, data->message->payloadlen, INDEX_SUB_TOPIC_CMD);
	mqtt_msg_received = false;
	memset((char*)data->message->payload, 0x00, data->message->payloadlen);

	#endif

	APP_DEBUG("has msg from topic : %s......\r\n", topic);
	APP_DEBUG("has msg from data : %s......\r\n", data);
	APP_DEBUG("has msg from len : %d......\r\n", len);
	if(strcmp(topic, AppConfig.MQTT_Config.list_topic[INDEX_SUB_TOPIC_LOCK_FORMAT].subtopic) == 0)
	{
		printf("data from lock topic: %s, data = %s\r\n", topic, data);
		AppConfig.MQTT_Config.list_topic[INDEX_SUB_TOPIC_LOCK_FORMAT].mqtt_rev_message(data, len, INDEX_SUB_TOPIC_LOCK_FORMAT);
	}
	else if(strcmp(topic, AppConfig.MQTT_Config.list_topic[INDEX_SUB_TOPIC_UV_FORMAT].subtopic) == 0)
	{
		printf("data from lock topic: %s, data = %s\r\n", topic, data);
		AppConfig.MQTT_Config.list_topic[INDEX_SUB_TOPIC_UV_FORMAT].mqtt_rev_message(data, len, INDEX_SUB_TOPIC_UV_FORMAT);
	}

	memset(topic_rev_data, 0x00, sizeof(topic_rev_data));
	memset(data_buff, 0x00, sizeof(data_buff));

	// printf("data from topic: %s, data = %s\r\n", topic, data);
	// // mqtt_parser(data, len);
    // AppConfig.MQTT_Config.list_topic[INDEX_SUB_TOPIC_LOCK_FORMAT].mqtt_rev_message()
}


#if 0
static void mqtt_parser(char* data, uint16_t len)
{
	if(len >=32)
		return;

	uint8_t ind = 0;
	uint8_t cnt = 0;
	uint8_t object[32] = {0};
	uint8_t value[4] = {0};
	uint8_t buff[32] = {0};

	uint8_t data_status[32] = {0};

	uint16_t ui = 0;

	for( ui = 0; ui < len; ui ++)
	{
		if(data[ui] != '\"')
		{
			if(data[ui] == ':')
				ind = cnt;
			buff[cnt ++] = data[ui];
		}
	}

	memcpy(object, buff, ind);

	memcpy(value, &buff[ind + 1], strlen((char*)buff) - ind + 1);

	printf("---- object = %s\r\n", object);

	printf("---- value = %s\r\n", value);

	if(strcmp((char*)object, (char*)"Light") == 0)
	{
		if(atoi((char*)value) == 1)
		{
			printf("---- turn on light\r\n");
			mqtt_set_status(EVT_SET_LIGHT_ON);
		}
		else if(atoi((char*)value) == 0)
			printf("---- turn off light\r\n");
	}
	else if(strcmp((char*)object, (char*)"Lock") == 0)
	{
		if(atoi((char*)value) == 1)
		{
			printf("turn on lock\r\n");
			sprintf((char*)data_status, "\"Lock\":\"%d\"", 1);
			mqtt_set_status(EVT_SET_LOCK_ON);
		}
		else
			sprintf((char*)data_status, "\"Lock\":\"%d\"", 0);

		mqtt_publish((char*)data_status, 0);

	}


}
#endif



static void mqtt_init(void)
{
	//topic sub
	char* sub_light_topic = (char*)calloc(SIZE_NAME + 1, sizeof(char));
	char* sub_lock_topic = (char*)calloc(SIZE_NAME + 1, sizeof(char));
	snprintf(sub_lock_topic, SIZE_NAME, MQTT_SUB_TOPIC_LOCK_FORMAT, AppConfig.ID_Device);
	snprintf(sub_light_topic, SIZE_NAME, MQTT_SUB_TOPIC_UV_FORMAT, AppConfig.ID_Device);


	//add topic to list_topic_sub
	MQTT_Register_SubTopic(sub_lock_topic, message_received_fromServer);
	MQTT_Register_SubTopic(sub_light_topic, message_received_fromServer);


	//add topic publish
	snprintf((char*)AppConfig.MQTT_Config.pubtopic, sizeof(AppConfig.MQTT_Config.pubtopic), MQTT_PUB_TOPIC_RESP_STATUS, AppConfig.ID_Device);
	// snprintf((char*)pub_topic[INDEX_PUB_TOPIC_KEEP_ALIVE], sizeof(pub_topic[INDEX_PUB_TOPIC_KEEP_ALIVE]), MQTT_PUB_TOPIC_KEEP_ALIVE_FOMAT, AppConfig.ID_Device);

	//set api publish
	AppConfig.MQTT_Config.publish = mqtt_publish;
	// MQTT_Register_RxCallBack(message_received_fromServer);


	
	APP_DEBUG("sub_lock_topic_0 = %s\r\n", sub_lock_topic);
	APP_DEBUG("sub_light_topic_1 = %s\r\n", sub_light_topic);
	APP_DEBUG("MQTT_PUB_TOPIC_RESP_STATUS = %s\r\n", AppConfig.MQTT_Config.pubtopic);
    
	free(sub_light_topic);
	free(sub_lock_topic);
}




static bool MQTT_Register_SubTopic(char* topic, void msg_rx_cb(char*, uint16_t, uint8_t))
{
	if( (num_topic >= subtopic_max) || (topic == NULL) || (msg_rx_cb == NULL) )
	{
		APP_DEBUG("regiter sub topic failed!\r\n");
		return false;
	}

	strcpy((char*)AppConfig.MQTT_Config.list_topic[num_topic].subtopic, topic);
	AppConfig.MQTT_Config.list_topic[num_topic].mqtt_rev_message = msg_rx_cb;
	num_topic++;
	APP_DEBUG("register topic %s success\r\n", topic);
	return true;
}
