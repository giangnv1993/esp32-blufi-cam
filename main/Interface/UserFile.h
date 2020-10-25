/*
 * UserFile.h
 *
 *  Created on: Apr 4, 2018
 *      Author: giangnv
 */

#ifndef MAIN_INTERFACE_USERFILE_H_
#define MAIN_INTERFACE_USERFILE_H_

bool bUserFile_Read(char* file_name, char* r_content);

bool bUserFile_CreateFile(char* file_name);

bool bUserFile_Write_Append(char* file_name, char* w_content, int w_len);

void UserFile_InitSpiffs(void);

void UserFile_DeInitSpiffs(void);

void vUserFile_DeleteFile(char *file_name);

bool bUserFile_Check_Existed(char *file_name);

bool bUserFile_Write_EmptyFile(char *file_name, char* w_content);

#endif /* MAIN_INTERFACE_USERFILE_H_ */











