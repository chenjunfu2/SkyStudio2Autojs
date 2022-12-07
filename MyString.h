#ifndef MyString_h
#define MyString_h

#include <stdio.h>

class MyString
{
private:
	size_t szcStrLen = 0;//不包括'\0',是cStr的字符串长度
	size_t szMemSize = 0;//是cStr的实际分配内存大小
	char *cStr = NULL;//字符串

public:
	MyString(const char *_cStr = "");
	MyString(const MyString &_String);
	MyString(MyString &&_String) noexcept;

	MyString(const char *_cStr, const size_t _szcStrLen);
	MyString(const char *_cStr, const size_t szBegin, const size_t szEnd);
	MyString(const MyString &_String, const size_t szBegin, const size_t szEnd);
	
	~MyString() noexcept;

	void Clear(void) noexcept;//简单的将cStr[0]设为'\0'
	void Free(void);//缩小内存大小至1并将cStr[0]设为'\0'
	const char *C_Str(void) const noexcept;

	bool IsEmpty(void) const noexcept;//是否为空
	size_t Length(void) const noexcept;//不包含\0的字符串长度
	size_t Size(void) const noexcept;//包含\0字符的内存占用大小
	void ResizeLength(size_t _NewLen, char _FillChar = ' ');//调整字符串长度

	void Capacity(size_t _NewLen);//设置可容纳字符内存(扩容)
	size_t GetReserve(void) const noexcept;//获得当前可容纳字符的内存大小

	void Swap(MyString &_String) noexcept;//交换

	size_t FindFirstChar(const char cFindChar) const noexcept;//查找某个字符第一次出现的位置
	size_t FindLastChar(const char cFindChar) const noexcept;//查找某个字符最后一次出现的位置
	size_t EndIterator(void) const noexcept;//返回尾后迭代器

	int compare(const char *_cStr) const noexcept;
	int compare(const MyString &_String) const noexcept;

	bool operator==(const char *_cStr) const noexcept;
	bool operator==(const MyString &_String) const noexcept;

	MyString &operator=(const char *_cStr);
	MyString &operator=(const MyString &_String);

	MyString operator+(const char *_cStr) const;
	MyString operator+(const MyString &_String) const;

	MyString &operator+=(const char *_cStr);
	MyString &operator+=(const MyString &_String);

	char &operator[](size_t _szIndex) const noexcept;

	bool GetLine(FILE *_fInput, const char _cEndLineChar = '\n');//读取一行,返回读取字节数
	size_t GetChars(FILE *_fInput, const size_t _szInputCharCount);//读取指定字节数,返回读取字节数

	bool OutLine(FILE *_fOutput, const char _cEndLineChar = '\n') const noexcept;//输出一行,返回输出字节数
	size_t OutChars(FILE *_fOutput, const size_t _szOutputCharCount) const noexcept;//输出指定字节数,返回输出字节数

private:
	static void SafeFree(char *_Mem) noexcept;
	static size_t StrLen2MemSize(size_t _szcStrLen);//从字符串长度求取字符串占用内存大小

	void RellocStrMem(size_t _szNewMemSize);//重分配
	void MallocMemAndCopyStr(const char *_cStr, size_t _szcStrLen);//分配内存并拷贝字符串
	MyString(size_t _szcStrLen, size_t _szMemSize, char *_cStr) noexcept;//特殊的内建构造函数
};


#endif // !MyString_h