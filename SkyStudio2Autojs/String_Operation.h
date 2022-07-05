#pragma once

unsigned long StrCpy(char* tar, const char* sour);

char* StrPCpy(char* tar, const char* sourS, const char* sourE);

unsigned long StrLen(const char* str);

bool StrCmp(const char* str1, const char* str2);

char* FindLastChars(char* pFind, char c1, char c2);

char* FindLastChar(char* pFind, char c);

unsigned long AddString(char* target, char* source);

bool IsUnsignedNum(const char* str);

bool IsUnsignedwNum(const wchar_t* str);
