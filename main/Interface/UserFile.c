/*
 * UserFile.c
 *
 *  Created on: Apr 4, 2018
 *      Author: giangnv
 */
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <esp_err.h>
#include <esp_spiffs.h>

#include "UserFile.h"

#if 1
esp_vfs_spiffs_conf_t conf = 
{
	.base_path = "/spiffs",
	.partition_label = NULL,
	.max_files = 5,
	.format_if_mount_failed = true
};

/*******************************************************************************
 * Function		: UserFile_InitSpiffs(void)
 * Description	: write append file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Data: data to write
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
void UserFile_InitSpiffs(void)
{
	esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            printf("---- Failed to mount or format filesystem\r\n");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            printf("---- Failed to find SPIFFS partition\r\n");
        } else {
            printf("---- Failed to initialize SPIFFS (%s)\r\n", esp_err_to_name(ret));
        }
        return;
    }
    
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        printf("---- Failed to get SPIFFS partition information (%s)\r\n", esp_err_to_name(ret));
    } else {
        printf("---- Partition size: total: %d, used: %d \r\n", total, used);
    }
}



/*******************************************************************************
 * Function		: UserFile_DeInitSpiffs(void)
 * Description	: write append file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Data: data to write
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
void UserFile_DeInitSpiffs(void)
{
    esp_vfs_spiffs_unregister(conf.partition_label);
}



/*******************************************************************************
 * Function		: bUserFile_CreateFile(char* file_name)
 * Description	: CreateFile to write
 * Param		:
 * 				+ file_name: file read
 * Return		:
 * 				+ false: fail
 * 				+ true: ok
 ******************************************************************************/
bool bUserFile_CreateFile(char* file_name)
{
    printf("---- create file......\r\n");
    FILE* f = fopen(file_name, "w");
    if (f == NULL) {
        printf("---- Failed to open file for writing\r\n");
        return false;
    }

    printf("---- create filename done.....\r\n");
    fclose(f);
    return true;
}







/*******************************************************************************
 * Function		: bUserFile_Read(char* file_name, char* r_content)
 * Description	: write append file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Data: data to write
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
bool bUserFile_Read(char* file_name, char* r_content)
{
    printf("---- Reading file\r\n");
    FILE* f = fopen(file_name, "r");
    if (f == NULL) {
        printf("---- Failed to open file for reading\r\n");
        return false;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '!');
    if (pos) {
        *pos = '\0';
    }
    printf("---- Read from file: '%s'", line);

    strcpy(r_content, line);

    return true;
}

/*******************************************************************************
 * Function		: Enum_FileState bUserFile_Write_Empty(char *PathFile, char *Data)
 * Description	: write empty file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Data: data to write
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
bool bUserFile_Write_EmptyFile(char *file_name, char* w_content)
{
	 FILE* file = fopen(file_name, "w");
	if(file == NULL)
	{
		printf("---- bUserFile_Write_EmptyFile: open file to write empty is failed\r\n");
		return false;
	}

	fprintf(file, w_content);
	fclose(file);

	return true;
}


/*******************************************************************************
 * Function		: bUserFile_Write_Append
 * Description	: write append file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Data: data to write
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
bool bUserFile_Write_Append(char* file_name, char* w_content, int w_len)
{
    FILE* f = fopen(file_name, "a");
    if (f == NULL) {
        printf("---- Failed to open file for writing\r\n");
        return false;
    }
    fprintf(f, w_content);
    fclose(f);
    printf("---- File written: %s, len = %d\r\n", w_content, w_len);
    return true;
}

/*******************************************************************************
 * Function		: Enum_FileState eFileAccess_CheckFile_Existed(char *PathFile)
 * Description	: check file existed
 * Param		: PathFile: path file to open
 * Return		:
 * 				+ false: not existed
 * 				+ true: existed
 ******************************************************************************/
bool bUserFile_Check_Existed(char *file_name)
{
	struct stat st;

	if(stat(file_name, &st) == 0)
		return true;
	else
		return false;
}



/*******************************************************************************
 * Function		: void vFileAccess_DeleteFile(char *PathFile)
 * Description	: delete file
 * Param		: PathFile: path file to open
 * Return		: None
 ******************************************************************************/
void vUserFile_DeleteFile(char *file_name)
{
	unlink(file_name);
}

#endif







