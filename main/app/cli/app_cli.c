/******************************************************************************
 * @file:    vsm_cli.c
 * @brief:
 * @version: V0.0.0
 * @date:    2019/11/12
 * @author:
 * @email:
 *
 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". VINSMART
 * JSC MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
 * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. VINSMART JSC
 * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
 * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
 * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
 *
 * VINSMART JSC SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
 * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
 * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
 * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
 * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
 * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
 *
 * (C)Copyright VINSMART JSC 2019 All rights reserved
 ******************************************************************************/



#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>


#include "app_cli.h"
#include "shell.h"

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "../../Interface/UserUart.h"

#define ATCONN					"ATCONN"
#define ATSET					"ATSET"
#define ATRST					"ATRST"
#define ATSN					"ATSN"
#define ATMQTT					"ATMQTT"
#define ATOTA					"ATOTA"
#define ATINFO					"ATINFO"
#define ATRSSI					"ATRSSI"


#define OBJ_MAX					(13)

#define BT_MAC_EFUSE_OFFSET                        0x190
#define WF_MAC_EFUSE_OFFSET                        0x11A
#define XCAP_EFUSE_OFFSET                          0x0C9


#define MAC_HEX_SIZE                               6





//static serial_t                                 app_cli_serial_instance;

static void     vsm_cli_main_loop               (void* pvParameters);

static void cli_command_config_handle(char* obj, char* val);
//static int32_t  cli_test_sendtoMainboard(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")
//
//static int32_t  cli_test_RevFromMainboard(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_wifi_reset(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")
static int32_t  cli_wifi_conn(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_send_config(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_mqtt_inf(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_ble_start(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_wifi_ota(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_setmac(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_getmac(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_get_info(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static int32_t  cli_getrssi(p_shell_context_t context, int32_t argc, char **argv); // @suppress("Type cannot be resolved")

static void do_restart_wifi(void);

int wf_get_mac_address(uint8_t *mac);

static const shell_command_context_t cli_command_table[] = {
    {"ATRST",     		"\"ATRST\": wifi reset\r\n",                                        	cli_wifi_reset,          0},
    {"ATCONN",      	"\"ATCONN\": connect to AP. Usage: ATCONN <SSID> <PASS>\r\n",           cli_wifi_conn,          2},
	{"ATBLE",      		"\"ATSN\": start ble peripheral: ATBLE start/stop \r\n",           		cli_ble_start,          0},
	{"ATMQTT",      	"\"ATMQTT\": get info MQTT\r\n",           								cli_mqtt_inf,          0},
	{"ATINFO",      	"\"ATINFO\": get info device\r\n",           							cli_get_info,          0},
	{"ATSETMAC",      	"\"ATSETMAC\": setmac info MQTT\r\n",           						cli_setmac,          	0},
	{"ATGETMAC",      	"\"ATGETMAC\": getmac info MQTT\r\n",           						cli_getmac,          	0},
	{"ATRSSI",      	"\"ATRSSI\": get rssi wifi\r\n",           								cli_getrssi,          	0},
	{"ATOTA",     		"\"ATOTA\": wifi ota\r\n",                                        		cli_wifi_ota,          0},
	{"ATSET",        	"\"ATSET\": simulator command control from server. Usage: ATSET <Object> <value>\r\n\
						ATSET\tPWR\t0/1\r\n\
						ATSET\tMODE\t1/2/4\r\n\
						ATSET\tFAN\t1/2/4/8/16\r\n\
						ATSET\tHUMI\t0/1/2/4/8/16\r\n\
						ATSET\tANION\t0/1\n\
						ATSET\tLOCK\t0/1\n\
						ATSET\tTIMON\t1-12\n\
						ATSET\tTIMOFF\t1-12\n\
						ATSET\tTIMNONE\t0\n\
						ATSET\tMUTE\t0/1\n\
						ATSET\tSMF\t0/1\n\
						ATSET\tUV\t0/1\r\n",													cli_send_config,   2},

};




static uint8_t command_handle_pwr(char* val);
static uint8_t command_handle_mode(char* val);
static uint8_t command_handle_spd(char* val);
static uint8_t command_handle_humier(char* val);
static uint8_t command_handle_anion(char* val);
static uint8_t command_handle_lock(char* val);
static uint8_t command_handle_timer_on(char* val);
static uint8_t command_handle_timer_off(char* val);
static uint8_t command_handle_timer_none(char* val);
static uint8_t command_handle_mute(char* val);
static uint8_t command_handle_uv(char* val);

static uint8_t command_handle_smf(char* val);

typedef uint8_t (*cmd_handle)(char* val);

typedef struct
{
	char obj[8];
	cmd_handle		command_handle;
}type_command_t;

type_command_t tbCommand[OBJ_MAX] =
{
	{"PWR"	 		,command_handle_pwr},
	{"MODE"	 		,command_handle_mode},
	{"FAN"	 		,command_handle_spd},
	{"HUMI" 		,command_handle_humier},
	{"ANION"	 	,command_handle_anion},
	{"LOCK"	 		,command_handle_lock},
	{"TIMON"	 	,command_handle_timer_on},
	{"TIMOFF"	 	,command_handle_timer_off},
	{"TIMNONE"	 	,command_handle_timer_none},
	{"MUTE"			,command_handle_mute},
	{"SMF"			,command_handle_smf},
	{"UV"			,command_handle_uv}
};


shell_context_struct user_context;
//static xTaskHandle      xCliTask;


void user_put_char(uint8_t *buf, uint32_t len)
{
//    serial_putc(&app_cli_serial_instance, *buf);
//	putchar(*buf);
	uart_write_bytes( UART_NUM_0, (const char*)buf, (size_t)len);
}

void user_get_char(uint8_t *buf, uint32_t len)
{

	uart_read_bytes(UART_NUM_0, buf, len, 5);

    // xSerialGetChar ((char*)buf, portMAX_DELAY) ;

//    *buf = serial_getc(&app_cli_serial_instance);
//	*buf = getchar();
}

void vsm_cli_start(void)
{
	uart_config_t uart_config = {
		        .baud_rate = 115200,
		        .data_bits = UART_DATA_8_BITS,
		        .parity    = UART_PARITY_EVEN,
		        .stop_bits = UART_STOP_BITS_1,
		        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
		    };
	uart_param_config(UART_NUM_0, &uart_config);
	uart_set_pin(UART_NUM_0, UART0_TX_PIN, UART0_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_driver_install(UART_NUM_0, BUFF_SIZE * 2, 0, 0, NULL, 0);
}

/*! \brief  VSM CLI Task
*/
void vsm_cli_main_loop(void* pvParameters)
{
    SHELL_Init(&user_context,
            user_put_char,
            user_get_char,
            printf,
            ">");

    /* Register CLI commands */
    for(int i = 0; i < sizeof(cli_command_table)/sizeof(shell_command_context_t); i ++) {
        SHELL_RegisterCommand(&cli_command_table[i]);
    }

    /* Run CLI task */
    SHELL_Main(&user_context);
}









int32_t cli_wifi_reset(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
//	if(argc > 1)
//	{
//		VSM_DEBUG("command wrong \r\n");
//		return 0;
//	}
//	else if(strcmp(argv[0], ATRST) != 0)
//	{
//		VSM_DEBUG("command wrong \r\n");
//		return 0;
//	}
//	else
//		vsm_sys_reset();
    return 0;
}

int32_t  cli_wifi_conn(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
//    if(argc > 3)
//    	VSM_DEBUG("command wrong \r\n");
//    else
//    {
//    	if(strcmp(argv[0], ATCONN) != 0)
//    	{
//    		VSM_DEBUG("command wrong \r\n");
//    		return 0;
//    	}
//    	else
//    		Wifi_Connect_toAP((uint8_t*)argv[1], (uint8_t*)argv[2], RTW_SECURITY_WPA2_AES_PSK);
//    }

    return 0;
}


static int32_t  cli_send_config(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
//	VSM_DEBUG("argc = %d\r\n", argc);
//	if(argc > 3)
//		VSM_DEBUG("command wrong \r\n");
//	else
//	{
//		if(strcmp(argv[0], ATSET) != 0)
//		{
//			VSM_DEBUG("command wrong \r\n");
//			return 0;
//		}
//		else
//			cli_command_config_handle(argv[1], argv[2]);
//	}

	return 0;

}



static void cli_command_config_handle(char* obj, char* val)
{
#if 0
	for(uint8_t ui = 0; ui < OBJ_MAX; ui ++)
	{
		if(strcmp(tbCommand[ui].obj, obj) == 0)
		{
			if(tbCommand[ui].command_handle(val) == 1)
				message_sendconfig(&message_control, mainboard);
			return;
		}
	}
	VSM_DEBUG("not found object control \r\n");
#endif
}




static uint8_t command_handle_pwr(char* val)
{
#if 0
	if(strcmp(val, "0") == 0)
		message_control.data.power = turnoff; // @suppress("Field cannot be resolved")
	else if(strcmp(val, "1") == 0)
		message_control.data.power = turnon;
	else
	{
		VSM_DEBUG("command wrong \r\n");
		return 0;
	}
	VSM_DEBUG_RAW("---- power : %s\r\n", message_control.data.power == turnon? "on": "off");
#endif
	return 1;
}

static uint8_t command_handle_mode(char* val)
{
	return 1;
}


static uint8_t command_handle_spd(char* val)
{
	return 1;

}



static uint8_t command_handle_humier(char* val)
{
	return 1;

}

static uint8_t command_handle_anion(char* val)
{
	return 1;

}
static uint8_t command_handle_lock(char* val)
{
	return 1;
}

static uint8_t command_handle_timer_on(char* val)
{
	return 0;
}
static uint8_t command_handle_timer_off(char* val)
{
	return 0;
}
static uint8_t command_handle_timer_none(char* val)
{
	return 1;
}

static uint8_t command_handle_mute(char* val)
{
	return 1;
}

static uint8_t command_handle_uv(char* val)
{
	return 1;
}


extern void Message_response_smartcfg(bool ret);
static uint8_t command_handle_smf(char* val)
{
//	if(strcmp(val, "0") == 0)
//		Message_response_smartcfg(false);
//	else if(strcmp(val, "1") == 0)
//	{
////		Message_response_smartcfg(true);
//		extern void ProcessTask_SetEvent(uint32_t event);
//		ProcessTask_SetEvent(EVT_BT_CONFIG_START);
//	}
//	else
//	{
//		VSM_DEBUG("command wrong \r\n");
//		return 0;
//	}
//	VSM_DEBUG_RAW("---- smf_reps \r\n");
	return 1;
}

static int32_t  cli_mqtt_inf(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
#if 0
	if(argc > 1)
	{
		VSM_DEBUG("command wrong \r\n");
		return 0;
	}
	else if(strcmp(argv[0], ATMQTT) != 0)
	{
		VSM_DEBUG("command wrong \r\n");
		return 0;
	}
	else
	{
		VSM_DEBUG("broker: %s\r\n", AppConfig.MQTT_Config.broker);
		VSM_DEBUG("port: %d\r\n", AppConfig.MQTT_Config.port);
		VSM_DEBUG("user: %s\r\n", AppConfig.MQTT_Config.username);
		VSM_DEBUG("pass: %s\r\n", AppConfig.MQTT_Config.password);
		VSM_DEBUG("mode: %d\r\n", AppConfig.MQTT_Config.mode);

		VSM_DEBUG("wifi: %s\r\n", AppConfig.Wifi_STA_Config.ssid);
		VSM_DEBUG("pass: %s\r\n", AppConfig.Wifi_STA_Config.pass);
		return 0;
	}
#endif
	return 0;
}


static int32_t  cli_get_info(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
#if 0
	if(argc > 1)
	{
		VSM_DEBUG("command wrong \r\n");
		return 0;
	}
	else if(strcmp(argv[0], ATINFO) != 0)
	{
		VSM_DEBUG("command wrong \r\n");
		return 0;
	}
	else
	{
		deviceinfo_show();
		return 0;
	}

	return 0;
#endif

	return 0;
}


static int32_t  cli_wifi_ota(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
#if 0
	if(argc > 1)
	{
		VSM_DEBUG("command wrong \r\n");
		return 0;
	}
	else if(strcmp(argv[0], ATOTA) != 0)
	{
		VSM_DEBUG("command wrong \r\n");
		return 0;
	}
	else
	{
//		ota_info_t tsOTA_Info = {0};
		extern ota_info_t tsOTA_Info;

//		strcpy((char*)tsOTA_Info.host, (char*)"minio.uat-smh.vsmarthome.net");
//		strcpy((char*)tsOTA_Info.resource, (char*)"public/WF_AP_BA400_200917_0103_003.bin");

		strcpy((char*)tsOTA_Info.host, (char*)"media-smh.vsmart.net");
		strcpy((char*)tsOTA_Info.resource, (char*)"/airpurifier/WF_AP_BA400_200917_0103_003");
		tsOTA_Info.port = 80;


		Ota_Start();
		return 0;
	}
#endif
	return 0;

}

static int32_t  cli_ble_start(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
	return 0;
}


static int32_t  cli_setmac(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
	return 0;
}

static int32_t  cli_getmac(p_shell_context_t context, int32_t argc, char **argv) // @suppress("Type cannot be resolved")
{
	return 0;
}



void do_restart_wifi(void)
{
}

int bt_set_mac_address(uint8_t *mac) {
    return 0;
}





int wf_set_mac_address(uint8_t *mac) {
    return 0;
}



int wf_get_mac_address(uint8_t *mac)
{
    return 0;
}



int wf_set_xcap(uint32_t xcap) {

    return 0;
}



int wf_get_xcap(uint32_t *xcap) {
    return 0;
}


static int32_t  cli_getrssi(p_shell_context_t context, int32_t argc, char **argv)
{
	return 0;
}





















