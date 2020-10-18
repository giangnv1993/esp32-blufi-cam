/*
 * FileAccess.c
 *
 *  Created on: Apr 4, 2018
 *      Author: ThaoHuyen
 */


#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "FileAccess.h"
#include "../main.h"


#define macroFILE_ACCESS_MODE_READ_ONLY			"r"		//read content file, file must exist
#define macroFILE_ACCESS_MODE_WRITE_ONLY		"w"		//create file empty for write
#define macroFILE_ACCESS_MODE_WRITE_APPEND		"a"		//write append file
#define macroFILE_ACCESS_MODE_READ_WRITE		"r+"	//open file for read and write. file must exist
#define macroFILE_ACCESS_MODE_CREATE_WR			"w+"	//create file for read and write
#define macroFILE_ACCESS_MODE_READ_APPEND		"r"		//open file for reading and appending




/*******************************************************************************
 * Function		: FILE *xFileAccess_Open(char *PathFile, char *Mode)
 * Description	: open file
 * Param		:
 * 				+ PathFile: path file to open
 * 				+ Mode: read, write,...
 * Return		: File description
 ******************************************************************************/
FILE *xFileAccess_Open(char *PathFile, char *Mode)
{
	FILE *File = fopen(PathFile, Mode);
	return File;
}



/*******************************************************************************
 * Function		: void vFileAccess_CreateFile(char *PathFile)
 * Description	: create file
 * Param		: PathFile: path file to open
 * Return		: None
 ******************************************************************************/
void vFileAccess_CreateFile(char *PathFile)
{
	FILE *file = xFileAccess_Open(PathFile, macroFILE_ACCESS_MODE_WRITE_ONLY);

	if(file == NULL)
	{
		printf("--- FileAccess: CreateFile failture\r\n");
		return;
	}
	fclose(file);
}





/*******************************************************************************
 * Function		: Enum_FileState eFileAccess_CheckFile_Existed(char *PathFile)
 * Description	: check file existed
 * Param		: PathFile: path file to open
 * Return		:
 * 				+ eFileFail: not existed
 * 				+ eFileOK: existed
 ******************************************************************************/
Enum_FileState eFileAccess_CheckFile_Existed(char *PathFile)
{
	struct stat st;

	if(stat(PathFile, &st) == 0)
		return eFileOK;
	else
		return eFileFail;
}




/*******************************************************************************
 * Function		: void vFileAccess_DeleteFile(char *PathFile)
 * Description	: delete file
 * Param		: PathFile: path file to open
 * Return		: None
 ******************************************************************************/
void vFileAccess_DeleteFile(char *PathFile)
{
	unlink(PathFile);
}




/*******************************************************************************
 * Function		: Enum_FileState eFileAccess_RenameFile(char *PathFileSRC, char *PathFileDST)
 * Description	: rename file
 * Param		:
 * 				+ PathFileSRC: file source
 * 				+ PathFileDST: file destination
 * Return		:
 * 				+ eFileFail: rename fail
 * 				+ eFileOK: rename ok
 ******************************************************************************/
Enum_FileState eFileAccess_RenameFile(char *PathFileSRC, char *PathFileDST)
{
	if (rename(PathFileSRC, PathFileDST) != 0)
		return eFileFail;
	else
		return eFileOK;
}




/*******************************************************************************
 * Function		: Enum_FileState eFileAccess_ReadLine(char *PathFile, char *Line)
 * Description	: read line file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Line: buffer to read
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
Enum_FileState eFileAccess_ReadLine(char *PathFile, char *Line)
{
	uint16_t ui = 0;
	FILE *file = xFileAccess_Open(PathFile, macroFILE_ACCESS_MODE_READ_ONLY);

	if(file == NULL)
	{
		printf("--- FileAccess: open file to Read line is failture\r\n");
		return eFileFail;
	}

	for( ; !feof(file); )
	{
		char c = fgetc(file);
		if(c == '\n')
			break;
		else if(c == '\r')
			continue;
		else
			Line[ui++] = c;

		if(ui > macroFILE_ACCESS_LINE_MAX_LENGHT)
			break;
	}

	fclose(file);

	return eFileOK;
}




/*******************************************************************************
 * Function		: Enum_FileState eFileAccess_ReadAll(char *PathFile, char *Buffer)
 * Description	: read all file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Buffer: buffer to read
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
Enum_FileState eFileAccess_ReadAll(char *PathFile, char *Buffer)
{
	uint16_t ui = 0;
	FILE *file = xFileAccess_Open(PathFile, macroFILE_ACCESS_MODE_READ_ONLY);

	if(file == NULL)
	{
		printf("--- FileAccess: open file to Read all is failture\r\n");
		return eFileFail;
	}

	memset(Buffer, 0, strlen(Buffer));

	for(;!feof(file);)
	{
		char c = fgetc(file);
		Buffer[ui++] = c;

		if(ui > macroFILE_ACCESS_MAX_LENGHT)
			break;
	}

	fclose(file);

	return eFileOK;
}





/*******************************************************************************
 * Function		: Enum_FileState eFileAccess_WriteFile_Append(char *PathFile, char *Data)
 * Description	: write append file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Data: data to write
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
Enum_FileState eFileAccess_WriteFile_Append(char *PathFile, char *Data)
{
	FILE *file = xFileAccess_Open(PathFile, macroFILE_ACCESS_MODE_WRITE_APPEND);
	if(file == NULL)
	{
		printf("--- FileAccess: open file to write append is failture\r\n");
		return eFileFail;
	}

	Data[strlen(Data)] = '\n';
	fprintf(file, Data);
	fclose(file);

	return eFileOK;
}





/*******************************************************************************
 * Function		: Enum_FileState eFileAccess_WriteFile_Empty(char *PathFile, char *Data)
 * Description	: write empty file
 * Param		:
 * 				+ PathFile: file read
 * 				+ Data: data to write
 * Return		:
 * 				+ eFileFail: fail
 * 				+ eFileOK: ok
 ******************************************************************************/
Enum_FileState eFileAccess_WriteFile_Empty(char *PathFile, char *Data)
{
	FILE *file = xFileAccess_Open(PathFile, macroFILE_ACCESS_MODE_WRITE_ONLY);
	if(file == NULL)
	{
		printf("--- FileAccess: open file to write empty is failture\r\n");
		return eFileFail;
	}

	fprintf(file, Data);
	fclose(file);

	return eFileOK;
}



















