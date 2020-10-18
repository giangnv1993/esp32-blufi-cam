/*
 * FileAccess.h
 *
 *  Created on: Apr 4, 2018
 *      Author: ThaoHuyen
 */

#ifndef MAIN_INTERFACE_FILEACCESS_H_
#define MAIN_INTERFACE_FILEACCESS_H_

#define macroFILE_ACCESS_LINE_MAX_LENGHT		64
#define macroFILE_ACCESS_MAX_LENGHT				512


typedef enum
{
	eFileFail,
	eFileOK,
}Enum_FileState;



/*******************************************************************************
 * Function		: FILE *xFileAccess_Open(char *PathFile, char *Mode)
 * Description	: open file
 * Param		:
 * 				+ PathFile: path file to open
 * 				+ Mode: read, write,...
 * Return		: File description
 ******************************************************************************/
FILE *xFileAccess_Open(char *PathFile, char *Mode);




/*******************************************************************************
 * Function		: void vFileAccess_CreateFile(char *PathFile)
 * Description	: create file
 * Param		: PathFile: path file to open
 * Return		: None
 ******************************************************************************/
void vFileAccess_CreateFile(char *PathFile);




/*******************************************************************************
 * Function		: Enum_FileState eFileAccess_CheckFile_Existed(char *PathFile)
 * Description	: check file existed
 * Param		: PathFile: path file to open
 * Return		:
 * 				+ eFileFail: not existed
 * 				+ eFileOK: existed
 ******************************************************************************/
Enum_FileState eFileAccess_CheckFile_Existed(char *PathFile);




/*******************************************************************************
 * Function		: void vFileAccess_DeleteFile(char *PathFile)
 * Description	: delete file
 * Param		: PathFile: path file to open
 * Return		: None
 ******************************************************************************/
void vFileAccess_DeleteFile(char *PathFile);




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
Enum_FileState eFileAccess_RenameFile(char *PathFileSRC, char *PathFileDST);




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
Enum_FileState eFileAccess_ReadLine(char *PathFile, char *Line);




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
Enum_FileState eFileAccess_ReadAll(char *PathFile, char *Buffer);





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
Enum_FileState eFileAccess_WriteFile_Append(char *PathFile, char *Data);




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
Enum_FileState eFileAccess_WriteFile_Empty(char *PathFile, char *Data);




#endif /* MAIN_INTERFACE_FILEACCESS_H_ */













