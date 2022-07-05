#pragma once

typedef long _BOOL;

typedef struct Config
{
	_BOOL bUseWindow = 0;// /w

	_BOOL bCoverFile = 0;// /c

	_BOOL bUseRandomMove = 0;// /r
	long ulRandomMove = 0;

	_BOOL bUseStartSleep = 0;// /s
	long ulStartSleep = 0;

	_BOOL bUseFolderPath = 0;// /f
	char* pFolderPath = 0;
}CONFIG, * PCONFIG;

#include <Windows.h>

bool Read1Byte(HANDLE hFile, char* cRead);

bool Read2Byte(HANDLE hFile, wchar_t* cRead);

bool WriteNByte(HANDLE hFile, LPCVOID lpData, DWORD dwSize);

bool GetFilePos(HANDLE hFile, PLARGE_INTEGER piPos);

LPVOID GetResourceData(LPCWSTR lpName, LPCWSTR lpType, LPDWORD lpSize);

bool ResourceToFile(LPCWSTR lpName, LPCWSTR lpType, LPCWSTR lpFileName);

bool HandleRead2Str(HANDLE f, const char* str);

bool HandleReadUntilChar(HANDLE f, char c, char* str, int size);

bool HandleRead2wStr(HANDLE f, const wchar_t* str);

bool HandleReadUntilwChar(HANDLE f, wchar_t c, wchar_t* str, int size);

bool ReadKeyData(HANDLE hFile, const char* key, long* data);

bool WriteDefaultConfig(HANDLE hFile);

bool ReadConfig(HANDLE hFile, PCONFIG pConfig);

HANDLE CreateJsFileAtSpecifiedLocation(LPSTR lpOriginalName, LPSTR lpFolderPath, bool bCover);

HANDLE CreateFileInPath(LPSTR lpPath, LPCSTR lpName);