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
                            "Interface/UserTimer.c"
                            "Interface/UserUart.c"
                            "Interface/UserFile.c"
                        #     "Task/ACInf_Task.c"
                            "Task/smartconf_task.c"
                            "Task/UartTask.c"
                            "Task/ProcessTask.c"
#                            "Task/mqtt_task.c"
                            "app/app.c"
                            # "app/wifi/wifi.c"
                            "app/app_debug.c"
                            "app/app_delay.c"
                            "app/cli/app_cli.c"
                            "app/cli/shell.c"
                            "app/smartconfig/blufi.c"
                            "app/smartconfig/blufi_security.c"
                            "app/Button/HardButton.c"
                            "app/mqtt/MQTTClient.c"
                            "app/device_info/device_info.c"
                            "app/message/cJSON.c"
                            "app/message/server_message.c"
                            "app/message/message.c"
                    INCLUDE_DIRS ".")

