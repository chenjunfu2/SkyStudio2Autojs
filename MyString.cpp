#include "MyString.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
类似String库，但是只提供简单的基础功能
同时附加提供从文件流中读写的功能
*/

MyString::MyString(const char *_cStr)//构造函数重复操作封装成函数
{
	if (_cStr == NULL)//防止用户传入空指针
	{
		_cStr = "";
	}

	//初始化
	MallocMemAndCopyStr(_cStr, strlen(_cStr));
}

MyString::MyString(const MyString &_String)
{
	//初始化
	MallocMemAndCopyStr(_String.cStr, _String.szcStrLen);
}

MyString::MyString(const char *_cStr, const size_t _szcStrLen)
{
	if (_cStr == NULL)//防止用户传入空指针
	{
		_cStr = "";
	}

	//初始化
	MallocMemAndCopyStr(_cStr, _szcStrLen);
}

MyString::MyString(MyString &&_String) noexcept
{
	//移动
	szcStrLen = _String.szcStrLen;
	szMemSize = _String.szMemSize;
	cStr = _String.cStr;

	//清除
	_String.szcStrLen = 0;
	_String.szMemSize = 0;
	_String.cStr = NULL;
}


MyString::MyString(const char *_cStr, const size_t szBegin, const size_t szEnd)
{
	if (_cStr == NULL)//防止用户传入空指针
	{
		_cStr = "";
	}

	//初始化
	MallocMemAndCopyStr(&_cStr[szBegin], szEnd - szBegin);
}


MyString::MyString(const MyString &_String, const size_t szBegin, const size_t szEnd)
{
	//初始化
	MallocMemAndCopyStr(&_String.cStr[szBegin], szEnd - szBegin);
}


MyString::~MyString() noexcept
{
	//清除并释放
	szcStrLen = 0;
	szMemSize = 0;
	SafeFree(cStr);
}


void MyString::Clear(void) noexcept
{
	szcStrLen = 0;
	cStr[0] = '\0';
}

void MyString::Free(void)
{
	RellocStrMem(1);//缩小内存至1个字符长度
	szcStrLen = 0;
	cStr[0] = '\0';
}

const char *MyString::C_Str(void) const noexcept
{
	return cStr;
}


size_t MyString::Length(void) const noexcept
{
	return szcStrLen;
}

size_t MyString::Size(void) const noexcept
{
	return StrLen2MemSize(szcStrLen);
}

void MyString::ResizeLength(size_t _NewLen, char _FillChar)
{
	if (_NewLen == szcStrLen)//不变
	{
		return;
	}
	else if (_NewLen > szcStrLen)//填充
	{
		//扩容(如果需要)
		Capacity(_NewLen);
		//计算要填充的字符数并填充
		size_t szFillSize = (_NewLen - szcStrLen) * sizeof(*cStr);
		memset(&cStr[szcStrLen], _FillChar, szFillSize);
	}

	//如果_NewLen < szcStrLen就直接执行下面的代码进行字符串截断，不需要额外操作

	szcStrLen = _NewLen;//设置新的长度
	cStr[_NewLen] = '\0';
	return;
}

bool MyString::IsEmpty(void) const noexcept
{
	return (cStr[0] == '\0') && (szcStrLen == 0);//一致性要求,如果不一致则会出现问题以便被发现
}

void MyString::Capacity(size_t _NewLen)
{
	size_t _NewSize = StrLen2MemSize(_NewLen);
	if (_NewSize <= szMemSize)
	{
		return;
	}

	//重分配
	RellocStrMem(_NewSize);
}

size_t MyString::GetReserve(void) const noexcept
{
	return szMemSize;
}

void MyString::Swap(MyString &_String) noexcept
{
	//保存外部string类成员
	size_t szNewcStrLen = _String.szcStrLen;
	size_t szNewMemSize = _String.szMemSize;
	char *cNewStr = _String.cStr;

	//将元素交由外部string管理
	_String.szcStrLen = szcStrLen;
	_String.szMemSize = szMemSize;
	_String.cStr = cStr;

	//将先前保存的外部string元素归为自身管理
	szcStrLen = szNewcStrLen;
	szMemSize = szNewMemSize;
	cStr = cNewStr;
}

size_t MyString::FindFirstChar(const char cFindChar) const noexcept//查找某个字符第一次出现的位置(未找到返回尾后位置)
{
	cStr[szcStrLen] = cFindChar;//设置哨兵字符

	char *cIterator = cStr;
	while (*cIterator != cFindChar)
	{
		++cIterator;
	}

	cStr[szcStrLen] = '\0';//恢复末尾空字符


	return cIterator - cStr;//返回迭代器位置
}

size_t MyString::FindLastChar(const char cFindChar) const noexcept//查找某个字符最后一次出现的位置(未找到返回尾后位置)
{
	char cTempSave = cStr[0];//保存开头字符
	cStr[0] = cFindChar;//设置哨兵字符

	char *cIterator = &cStr[szcStrLen - 1];//从\0前一个字符开始反向查找，找到的第一个字符就是这个字符最后一次出现的位置
	while (*cIterator != cFindChar)
	{
		--cIterator;
	}

	cStr[0] = cTempSave;//恢复原来的字符

	if (cTempSave != cFindChar && cIterator <= cStr)//开头原先的字符不是要查找的 并且 查找到了开头的位置
	{
		return szcStrLen;//返回尾后位置
	}

	return cIterator - cStr;//返回迭代器位置
}

size_t MyString::EndIterator(void) const noexcept
{
	return szcStrLen;
}

int MyString::compare(const char *_cStr) const noexcept
{
	return strcmp(cStr, _cStr);
}

int MyString::compare(const MyString &_String) const noexcept
{
	return strcmp(cStr, _String.cStr);
}


bool MyString::operator==(const char *_cStr) const noexcept
{
	return strcmp(cStr, _cStr) == 0;
}

bool MyString::operator==(const MyString &_String) const noexcept
{
	return strcmp(cStr, _String.cStr) == 0;
}


MyString &MyString::operator=(const char *_cStr)
{
	if (_cStr == NULL)//防止用户传入空指针
	{
		_cStr = "";
	}

	this->MyString::~MyString();//释放自身
	MallocMemAndCopyStr(_cStr, strlen(_cStr));//构造
	//this->MyString::MyString(_cStr);//调用默认构造

	return *this;
}

MyString &MyString::operator=(const MyString &_String)
{
	this->MyString::~MyString();//释放自身
	MallocMemAndCopyStr(_String.cStr, _String.szcStrLen);//构造
	//this->MyString::MyString(_String);//调用默认构造

	return *this;
}


MyString MyString::operator+(const char *_cStr) const
{
	if (_cStr == NULL)//防止用户传入空指针
	{
		_cStr = "";
	}
	size_t sz_cStrLen = strlen(_cStr);

	size_t szAddcStrLen = szcStrLen + sz_cStrLen;
	size_t szAddMemSize = StrLen2MemSize(szAddcStrLen);
	char *cAddStr = (char *)malloc(szAddMemSize);
	if (cAddStr == NULL)
	{
		throw("No Memory!");
		return NULL;
	}

	memcpy(cAddStr, cStr, szcStrLen * sizeof(*cStr));//不拷贝'\0'
	memcpy(&cAddStr[szcStrLen], _cStr, StrLen2MemSize(sz_cStrLen));//拷贝'\0'

	return MyString(szAddcStrLen, szAddMemSize, cAddStr);//构造对象并返回
}

MyString MyString::operator+(const MyString &_String) const
{
	size_t szAddcStrLen = szcStrLen + _String.szcStrLen;
	size_t szAddMemSize = StrLen2MemSize(szAddcStrLen);
	char *cAddStr = (char *)malloc(szAddMemSize);
	if (cAddStr == NULL)
	{
		throw("No Memory!");
		return NULL;
	}

	memcpy(cAddStr, cStr, szcStrLen * sizeof(*cStr));//不拷贝'\0'
	memcpy(&cAddStr[szcStrLen], _String.cStr, StrLen2MemSize(_String.szcStrLen));//拷贝'\0'

	return MyString(szAddcStrLen, szAddMemSize, cAddStr);//构造对象并返回
}


MyString &MyString::operator+=(const char *_cStr)
{
	if (_cStr == NULL)//防止用户传入空指针
	{
		_cStr = "";
	}

	size_t sz_cStrLen = strlen(_cStr);
	size_t szAddcStrLen = szcStrLen + sz_cStrLen;
	Capacity(szAddcStrLen);//扩容(如果需要)失败则cStr指向的内容不会被修改

	memcpy(&cStr[szcStrLen], _cStr, StrLen2MemSize(sz_cStrLen));//拷贝'\0'
	szcStrLen = szAddcStrLen;

	return *this;
}

MyString &MyString::operator+=(const MyString &_String)
{
	size_t szAddcStrLen = szcStrLen + _String.szcStrLen;
	Capacity(szAddcStrLen);//扩容(如果需要)失败则cStr指向的内容不会被修改

	memcpy(&cStr[szcStrLen], _String.cStr, StrLen2MemSize(_String.szcStrLen));//拷贝'\0'
	szcStrLen = szAddcStrLen;

	return *this;
}


char &MyString::operator[](size_t _szIndex) const noexcept
{
	return cStr[_szIndex];
}


bool MyString::GetLine(FILE *_fInput, const char _cEndLineChar)
{
	if (feof(_fInput))
	{
		return false;
	}

	szcStrLen = 0;//从头开始获得字符
	while(true)
	{
		if (StrLen2MemSize(szcStrLen) > szMemSize)
		{
			RellocStrMem(szMemSize * 2);//二倍扩容(重分配)
		}

		cStr[szcStrLen] = fgetc(_fInput);

		if (cStr[szcStrLen] == _cEndLineChar || feof(_fInput))
		{
			cStr[szcStrLen] = '\0';
			return true;
		}
		else
		{
			++szcStrLen;
		}
	}
}

size_t MyString::GetChars(FILE *_fInput, const size_t _szInputCharCount)
{
	if (feof(_fInput))
	{
		return 0;
	}

	Capacity(_szInputCharCount);//扩容(如果需要)

	size_t szReadSize = fread(cStr, sizeof(*cStr), _szInputCharCount, _fInput);
	szcStrLen = _szInputCharCount;
	cStr[_szInputCharCount] = '\0';

	return szReadSize;
}


bool MyString::OutLine(FILE *_fOutput, const char _cEndLineChar) const noexcept
{
	cStr[szcStrLen] = _cEndLineChar;//临时将'\0'改成行结束符
	size_t szWriteSize = fwrite(cStr, sizeof(*cStr), szcStrLen + 1, _fOutput);//szcStrLen + 1包含行结束符
	cStr[szcStrLen] = '\0';//复原

	return szWriteSize == (szcStrLen + 1);
}

size_t MyString::OutChars(FILE *_fOutput, const size_t _szOutputCharCount) const noexcept
{
	return fwrite(cStr, sizeof(*cStr), _szOutputCharCount, _fOutput);
}

inline void MyString::SafeFree(char *_Mem) noexcept
{
	if (_Mem != NULL)
	{
		free(_Mem);
		_Mem = NULL;
	}
}

inline size_t MyString::StrLen2MemSize(size_t _szcStrLen)
{
	return (_szcStrLen + 1) * sizeof(*cStr);
}

inline void MyString::RellocStrMem(size_t _szNewMemSize)
{
	char *cNewStr = (char *)realloc(cStr, _szNewMemSize);//relloc失败则cStr指向的内容不会被修改
	if (cNewStr == NULL)
	{
		throw("No Memory!");
		return;
	}

	szMemSize = _szNewMemSize;
	cStr = cNewStr;
}

MyString::MyString(size_t _szcStrLen, size_t _szMemSize, char *_cStr) noexcept :
	szcStrLen(_szcStrLen), szMemSize(_szMemSize), cStr(_cStr)
{
	return;
}

void MyString::MallocMemAndCopyStr(const char *_cStr, size_t _szcStrLen)
{
	//计算字符串占用大小
	size_t _szMemSize = StrLen2MemSize(_szcStrLen);
	//分配字符串内存
	char *_cMem = (char *)malloc(_szMemSize);
	if (_cMem == NULL)
	{
		throw("No Memory!");
		return;
	}

	//拷贝外部字符串到分配的内存
	memcpy(_cMem, _cStr, _szMemSize);

	//初始化类成员
	szcStrLen = _szcStrLen;
	szMemSize = _szMemSize;
	cStr = _cMem;
}
