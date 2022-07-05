#include "String_Operation.h"
#include "File_Operation.h"

#include <Windows.h>
#include <stdio.h>
#include <time.h>


#define TEMPSIZE 32
#define DEFAULT_RANDOMMOVE	"UseWindow=true\n"\
							"CoverFile=true\n\n"\
							"UseRandomMove=true\n"\
							"RandomMove=18\n\n"\
							"UseStartSleep=false\n"\
							"StartSleep=500\n\n"\
							"UseFolderPath=false\n"\
							"FolderPath="


bool Read1Byte(HANDLE hFile, char* cRead)
{
	DWORD dwReadSize;
	return ReadFile(hFile, cRead, 1, &dwReadSize, NULL) && dwReadSize == 1;
}

bool Read2Byte(HANDLE hFile, wchar_t* cRead)
{
	DWORD dwReadSize;
	return ReadFile(hFile, cRead, 2, &dwReadSize, NULL) && dwReadSize == 2;
}

bool WriteNByte(HANDLE hFile, LPCVOID lpData, DWORD dwSize)
{
	DWORD dwWriteSize;
	return WriteFile(hFile, lpData, dwSize, &dwWriteSize, NULL) && dwWriteSize == dwSize;
}

bool GetFilePos(HANDLE hFile, PLARGE_INTEGER piPos)
{
	return SetFilePointerEx(hFile, LARGE_INTEGER{ 0 }, piPos, FILE_CURRENT);
}

LPVOID GetResourceData(LPCWSTR lpName, LPCWSTR lpType, LPDWORD lpSize)
{
	HRSRC src = FindResource(GetModuleHandle(NULL), lpName, lpType);
	if (lpSize)
		*lpSize = SizeofResource(GetModuleHandle(NULL), src);
	HGLOBAL hglo = LoadResource(GetModuleHandle(NULL), src);
	return LockResource(hglo);
}

bool ResourceToFile(LPCWSTR lpName, LPCWSTR lpType, LPCWSTR lpFileName)
{
	DWORD dwResourceSize, dwWriteSize;
	VOID* pResource = GetResourceData(lpName, lpType, &dwResourceSize);

	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	return WriteFile(hFile, pResource, dwResourceSize, &dwWriteSize, NULL) && dwResourceSize == dwWriteSize;
}

bool HandleRead2Str(HANDLE f, const char* str)
{
	int i;
	char c;
	bool suc = true;

	do
	{
		i = 0;
		while (str[i] && (suc = Read1Byte(f, &c)) && c != str[i])
			continue;

		while (str[++i] && (suc = Read1Byte(f, &c)) && c == str[i])
			continue;

		if (!suc && str[i])
			return false;
	} while (str[i]);

	return true;
}

bool HandleReadUntilChar(HANDLE f, char c, char* str, int size)
{
	int i;
	bool suc = true;

	for (i = 0; i < size && (suc = Read1Byte(f, &str[i])) && str[i] != c; ++i)
		continue;

	if (!suc || i == size && str[size - 1] != c || str[i] != c)
		return false;

	if (str[i] == c)
	{
		str[i] = 0;
		return true;
	}

	return false;
}

bool HandleRead2wStr(HANDLE f, const wchar_t* str)
{
	int i;
	wchar_t c;
	bool suc = true;

	do
	{
		i = 0;
		while (str[i] && (suc = Read2Byte(f, &c)) && c != str[i])
			continue;

		while (str[++i] && (suc = Read2Byte(f, &c)) && c == str[i])
			continue;

		if (!suc && str[i])
			return false;
	} while (str[i]);

	return true;
}

bool HandleReadUntilwChar(HANDLE f, wchar_t c, wchar_t* str, int size)
{
	int i;
	bool suc = true;

	for (i = 0; i < size && (suc = Read2Byte(f, &str[i])) && str[i] != c; ++i)
		continue;

	if (!suc || i == size && str[size - 1] != c || str[i] != c)
		return false;

	if (str[i] == c)
	{
		str[i] = 0;
		return true;
	}

	return false;
}

bool ReadKeyData(HANDLE hFile, const char* key, long* data)
{
	char cTemp[TEMPSIZE];
	bool b;

	b = HandleRead2Str(hFile, key);
	b = b && HandleReadUntilChar(hFile, '\n', cTemp, TEMPSIZE);

	if (tolower(cTemp[0]) == 't' || tolower(cTemp[0]) == 'f')
		*data = tolower(cTemp[0]) == 't';
	else if (IsUnsignedNum(cTemp))
		*data = atol(cTemp);
	else
		b = false;

	return b;
}

bool WriteDefaultConfig(HANDLE hFile)
{
	bool b;
	DWORD dwWriteSize;

	//定位到文件开头
	b = SetFilePointerEx(hFile, LARGE_INTEGER{ 0 }, NULL, FILE_BEGIN);
	//写入配置
	b = b && WriteFile(hFile, DEFAULT_RANDOMMOVE, sizeof(DEFAULT_RANDOMMOVE) - 1, &dwWriteSize, NULL);
	//截断文件
	b = b && SetEndOfFile(hFile);
	return b;
}

bool ReadConfig(HANDLE hFile, PCONFIG pConfig)
{
	bool b;
	//定位到文件开头
	b = SetFilePointerEx(hFile, LARGE_INTEGER{ 0 }, NULL, FILE_BEGIN);
	//读取配置
	b = b && ReadKeyData(hFile, "UseWindow=", &pConfig->bUseWindow);
	b = b && ReadKeyData(hFile, "CoverFile=", &pConfig->bCoverFile);

	b = b && ReadKeyData(hFile, "UseRandomMove=", &pConfig->bUseRandomMove);
	b = b && ReadKeyData(hFile, "RandomMove=", &pConfig->ulRandomMove);

	b = b && ReadKeyData(hFile, "UseStartSleep=", &pConfig->bUseStartSleep);
	b = b && ReadKeyData(hFile, "StartSleep=", &pConfig->ulStartSleep);

	b = b && ReadKeyData(hFile, "UseFolderPath=", &pConfig->bUseFolderPath);
	b = b && HandleRead2Str(hFile, "FolderPath=");

	//读取配置路径
	LARGE_INTEGER liReadSize = { 0 };
	LARGE_INTEGER liTemp = { 0 };
	b = b && GetFileSizeEx(hFile, &liReadSize);
	b = b && GetFilePos(hFile, &liTemp);

	if (!(liReadSize.QuadPart -= liTemp.QuadPart))
	{
		pConfig->pFolderPath = NULL;
		return b;
	}
	pConfig->pFolderPath = (char*)malloc((liReadSize.QuadPart + 1) * sizeof(char));
	if (!pConfig->pFolderPath)
		return false;

	DWORD dwReadSize;
	b = b && ReadFile(hFile, pConfig->pFolderPath, liReadSize.QuadPart, &dwReadSize, NULL);
	b = b && dwReadSize == liReadSize.QuadPart;
	pConfig->pFolderPath[liReadSize.QuadPart] = 0;

	return b;
}

HANDLE CreateJsFileAtSpecifiedLocation(LPSTR lpOriginalName, LPSTR lpFolderPath, bool bCover)
{
	char pTimeTemp[TEMPSIZE];
	char* pPathTemp = NULL;
	char* pPath = NULL;
	long lFileNameSize = 0;

	if (lpFolderPath)
	{
		pPathTemp = FindLastChars(lpOriginalName, '/', '\\');
		lFileNameSize += StrLen(lpFolderPath);
		lFileNameSize += StrLen(pPathTemp);
	}
	else
	{
		pPathTemp = lpOriginalName;
		lFileNameSize += StrLen(lpOriginalName);
	}
	if (!bCover)
	{
		ltoa(time(NULL), pTimeTemp, 10);
		lFileNameSize += StrLen(pTimeTemp);
	}
	lFileNameSize += 1;

	pPath = (char*)malloc(lFileNameSize * sizeof(char));
	if (!pPath)
	{
		return INVALID_HANDLE_VALUE;
	}
	pPath[0] = '\0';


	if (lpFolderPath)
	{
		StrCpy(pPath, lpFolderPath);
	}

	AddString(pPath, pPathTemp);
	pPathTemp = FindLastChar(pPath, '.') - 1;

	if (!bCover)
	{
		pPathTemp += StrCpy(pPathTemp, pTimeTemp);
	}
	pPathTemp[0] = '.';
	pPathTemp[1] = 'j';
	pPathTemp[2] = 's';
	pPathTemp[3] = '\0';

	HANDLE hFile = CreateFileA(pPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	fprintf(stdout, "%s\n", pPath);
	free(pPath);
	return hFile;
}

HANDLE CreateFileInPath(LPSTR lpPath, LPCSTR lpName)
{
	unsigned long ulStrSize;
	LPSTR lpTemp = FindLastChars(lpPath, '/', '\\');
	ulStrSize = lpTemp - lpPath;
	ulStrSize += StrLen(lpName) + 1;

	LPSTR lpStr = (LPSTR)malloc(ulStrSize * sizeof(CHAR));
	if(!lpStr)
	{
		return INVALID_HANDLE_VALUE;
	}

	lpTemp = StrPCpy(lpStr, lpPath, lpTemp);
	StrCpy(lpTemp, lpName);
	HANDLE hFile = CreateFileA(lpStr, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	free(lpStr);
	return hFile;
}