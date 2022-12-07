#ifndef FileOperation_h
#define FileOperation_h

#include "Main.h"
#include "MyString.h"

#include <stdio.h>

//打开输入文件并创建输出文件
bool OpenInputAndCreateOuputFile(
	__IN__ const MyString &SourceFileDirectory,
	__IN__ const MyString &TargetFileDirectory,
	__IN__ const MyString &SourceFileName,
	__OUT__ FILE **pfInput,
	__OUT__ FILE **pfOuput);

//一直读取文件,直到Str后(移动文件指针到字符串Str后)
bool JumpFilePointerAfterStr(
	__IO__ FILE *fInput,
	__IN__ const wchar_t *wcJumpStr);

//将文件内字符wcEndChar之前的内容读入wcReadStr
bool ReadFileContentsBeforeCharEncountered(
	__IO__ FILE *fRead,
	__IN__ const wchar_t wcEndChar,
	__OUT__ wchar_t *wcReadStr,
	__IN__ const long szReadStrSize);

//从wcJumpStr后，到wcEndChar之间的内容读入wcReadStr
bool ReadBetweenStrAndChar(
	__IO__ FILE *fInput,
	__IN__ const wchar_t *wcJumpStr,
	__IN__ const wchar_t wcEndChar,
	__OUT__ wchar_t *wcReadStr,
	__IN__ const long lReadStrSize);

//忽略空格并从文件中读取字符串
bool ReadStrIgnoreSpace(
	__IO__ FILE *fInput,
	__OUT__ wchar_t *wcReadStr,
	__IN__ const long lReadStrSize);

//读取并丢弃内容，直到文件下一行
bool JumpFileLine(
	__IO__ FILE *fInput);

//文件头判断
bool IsUTF16(
	__IO__ FILE *fInput,
	__IN__ const bool LE);

//ABC谱判断
bool IsABCScore(
	__IO__ FILE *fInput);

//向文件输出Autojs代码
bool WriteCode(
	__IO__ FILE *fOutput,
	__IN__ const char *cFormatStr,
	__IN__ const char8_t *c8WriteCode[],
	__IN__ const long lCodeElementNumber);

//打开文件并文件输出整个Autojs代码数组然后关闭文件
bool OpenFileAndWriteCode(
	__IN__ const MyString &FileName,
	__IN__ const char8_t *c8WriteCode[],
	__IN__ const long lCodeElementNumber);

#endif // !FileOperation_h