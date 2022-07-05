#include "String_Operation.h"

unsigned long StrCpy(char* tar, const char* sour)
{
	unsigned long i = 0;
	while (tar[i] = sour[i])
		++i;
	return i;
}

char* StrPCpy(char* tar, const char* sourS, const char* sourE)
{
	while (sourS < sourE)
	{
		*tar = *sourS;
		++tar;
		++sourS;
	}
	*tar = 0;
	return tar;
}

unsigned long StrLen(const char* str)
{
	unsigned long i = 0;
	while (str[i])
		++i;
	return i;
}

bool StrCmp(const char* str1, const char* str2)
{
	unsigned long i = 0;
	while (str1[i] && str2[i])
	{
		if (str1[i] != str2[i])
		{
			return false;
		}
	}
	if (str1[i] || str2[i])
	{
		return false;
	}

	return true;
}

char* FindLastChars(char* pFind, char c1, char c2)
{
	char* temp = pFind;
	long i = 0;
	while (pFind[i])
	{
		if (pFind[i] == c1 || pFind[i] == c2)
		{
			temp = &pFind[++i];
		}
		else
		{
			++i;
		}
	}
	return temp;
}

char* FindLastChar(char* pFind, char c)
{
	char* temp = pFind;
	long i = 0;
	while (pFind[i])
	{
		if (pFind[i] == c)
		{
			temp = &pFind[++i];
		}
		else
		{
			++i;
		}
	}
	return temp;
}

unsigned long AddString(char* target, char* source)
{
	unsigned long lLen = StrLen(target);
	unsigned long i = 0;
	while (target[lLen] = source[i])
	{
		++lLen;
		++i;
	}
	return lLen;
}

bool IsUnsignedNum(const char* str)
{
	unsigned long i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			return false;
		}
		++i;
	}
	return true;
}

bool IsUnsignedwNum(const wchar_t* str)
{
	unsigned long i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			return false;
		}
		++i;
	}
	return true;
}

