#set(COMPONENT_SRCS
#    app_main.c
#    app_camera.c
#    app_httpserver.c
#    app_speech_recsrc.c
#    app_speech_wakeup.c
#    app_wifi.c
#    )
#
#set(COMPONENT_ADD_INCLUDEDIRS include)
#
#
#register_component()

idf_component_register(SRCS "main.c"
#                            "MQTTClient.c"
                            "ACInterface/ACInf.c"
                            "ACInterface/Message.c"
                            "ACInterface/MessageHandle.c"
                            "Interface/Spiffs/esp_spiffs.c"
                            "Interface/Spiffs/list.c"
                            "Interface/Spiffs/mutex.c"
                            "Interface/Spiffs/spiffs_cache.c"
                            "Interface/Spiffs/spiffs_check.c"
                            "Interface/Spiffs/spiffs_gc.c"
                            "Interface/Spiffs/spiffs_hydrogen.c"
                            "Interface/Spiffs/spiffs_nucleus.c"
                            "Interface/Spiffs/spiffs_vfs.c"
                            "Interface/FileAccess.c"
                            "Interface/UserFile.c"
                            "Interface/UserTimer.c"
                            "Interface/UserUart.c"
#                            "MQttLib/mqtt_client.c"
#                            "MQttLib/mqtt_msg.c"
#                            "MQttLib/mqtt_outbox.c"
#                            "MQttLib/platform_esp32_idf.c"
#                            "MQttLib/transport_ssl.c"
#                            "MQttLib/transport_tcp.c"
#                            "MQttLib/transport.c"
                            "Task/ACInf_Task.c"
                            "Task/smartconf_task.c"
                            "Task/UartTask.c"
                            "Task/ProcessTask.c"
#                            "Task/mqtt_task.c"
                            "app/app.c"
#                           "app/wifi/wifi.c"
                            "app/app_debug.c"
                            "app/app_delay.c"
                            "app/cli/app_cli.c"
                            "app/cli/shell.c"
                            "app/smartconfig/blufi.c"
                            "app/smartconfig/blufi_security.c"
                            "app/Button/HardButton.c"
                            "app/mqtt/MQTTClient.c"
#                            "app/mqtt/mqtt_task.c"
#                            "app/mqtt/MQttLib/mqtt_client.c"
#                            "app/mqtt/MQttLib/mqtt_msg.c"
#                            "app/mqtt/MQttLib/mqtt_outbox.c"
#                            "app/mqtt/MQttLib/platform_esp32_idf.c"
#                            "app/mqtt/MQttLib/transport_ssl.c"
#                            "app/mqtt/MQttLib/transport_tcp.c"
#                            "app/mqtt/MQttLib/transport.c"
                            "app/device_info/device_info.c"
                    INCLUDE_DIRS ".")

