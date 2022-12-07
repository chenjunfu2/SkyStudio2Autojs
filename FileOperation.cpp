#include "FileOperation.h"

#include <wchar.h>
#include <stdlib.h>
#include <string.h>

#ifndef WINDOWS_EXECUTE
#define fgetwc(var) my_fgetwc(var)//替换为模拟函数
#endif // WINDOWS_EXECUTE

/******************************文件操作函数******************************/

//模拟函数fgetwc
wchar_t my_fgetwc(
	__IO__ FILE *fInputFile)//UTF16-LE
{
	wchar_t wc;//小端字节序
	wc = (unsigned char)fgetc(fInputFile);//低8位
	wc |= (unsigned char)fgetc(fInputFile) << 8;//高8位
	return wc;
}

//打开输入文件并创建输出文件
bool OpenInputAndCreateOuputFile(
	__IN__ const MyString &SourceFileDirectory,
	__IN__ const MyString &TargetFileDirectory,
	__IN__ const MyString &SourceFileName,
	__OUT__ FILE **pfInput,
	__OUT__ FILE **pfOuput)
{
	//输入检查
	if (SourceFileName.IsEmpty())
	{
		return false;
	}

	/*输入文件*/
	//拼接目录名
	MyString InputFileName = SourceFileDirectory + SourceFileName;
	//只读二进制打开输入文件
	fprintf(stdout, "输入文件:\"%s\"", InputFileName.C_Str());
	FILE *fInput = fopen(InputFileName.C_Str(), "rb");
	if (fInput == NULL)
	{
		fprintf(stderr, "打开失败\n");
		return false;
	}
	fprintf(stdout, "打开成功\n");


	/*输出文件*/
	//拼接目录名并修改后缀
	MyString OutputFileName = TargetFileDirectory + SourceFileName;
	OutputFileName.ResizeLength(OutputFileName.Length() - strlen(".txt"));
	OutputFileName += ".js";

	//只读二进制打开输出文件
	fprintf(stdout, "输出文件:\"%s\"", OutputFileName.C_Str());
	FILE *fOuput = fopen(OutputFileName.C_Str(), "wb");
	if (fOuput == NULL)
	{
		fprintf(stderr, "打开失败\n");
		fclose(fInput);
		return false;
	}
	fprintf(stdout, "打开成功\n");

	//输入输出文件都打开成功才修改传入的变量
	*pfInput = fInput;
	*pfOuput = fOuput;

	return true;
}

//一直读取文件,直到Str后(移动文件指针到字符串Str后)
bool JumpFilePointerAfterStr(
	__IO__ FILE *fInput,
	__IN__ const wchar_t *wcJumpStr)
{
	//文件指针为空,匹配失败
	if (fInput == NULL)
	{
		return false;
	}

	//空字符串,匹配成功
	if (wcJumpStr[0] == L'\0')
	{
		return true;
	}

	//读取文件,直到读取到wcJumpStr后
	while (true)
	{
		//读取文件内容直到与目标字符串的第一个字符匹配
		while (!feof(fInput))//检查文件是否结束
		{
			if (fgetwc(fInput) == wcJumpStr[0])
			{
				break;
			}
		}

		//依次比较文件内容与字符串后面的内容
		long strIndex = 1;
		while (wcJumpStr[strIndex] != L'\0' && !feof(fInput))//检查字符串或文件是否结束
		{
			if (fgetwc(fInput) != wcJumpStr[strIndex])
			{
				break;
			}
			++strIndex;
		}

		//检查是否匹配成功
		if (wcJumpStr[strIndex] == L'\0')//匹配成功
		{
			return true;
		}
		else if (feof(fInput))//匹配失败
		{
			return false;
		}
		else//其它情况,继续匹配
		{
			continue;
		}
	}

	//正常情况下不应退出无限循环,如果遇到意外执行至此则返回失败
	return false;
}

//将文件内字符wcEndChar之前的内容读入wcReadStr
bool ReadFileContentsBeforeCharEncountered(
	__IO__ FILE *fRead,
	__IN__ const wchar_t wcEndChar,
	__OUT__ wchar_t *wcReadStr,
	__IN__ const long szReadStrSize)
{
	long strIndex = 0;
	bool bIsRead = false;

	//一直读取直到wcReadStr到达上限或读到文件尾或已经读取到符合的字符串
	while (strIndex < szReadStrSize && !feof(fRead))
	{
		wcReadStr[strIndex] = fgetwc(fRead);
		if (wcReadStr[strIndex] == wcEndChar)//如果读取已经到了符合的字符则跳出循环
		{
			bIsRead = true;
			break;
		}
		++strIndex;
	}

	if (strIndex < szReadStrSize)
	{
		wcReadStr[strIndex] = L'\0';//设置字符串结尾
	}

	//返回取状态
	return bIsRead;
}

//从wcJumpStr后，到wcEndChar之间的内容读入wcReadStr
bool ReadBetweenStrAndChar(
	__IO__ FILE *fInput,
	__IN__ const wchar_t *wcJumpStr,
	__IN__ const wchar_t wcEndChar,
	__OUT__ wchar_t *wcReadStr,
	__IN__ const long lReadStrSize)
{
	//跳到字符串wcStartStr后面
	if (!JumpFilePointerAfterStr(fInput, wcJumpStr))
	{
		return false;
	}
	//读取直到遇到wcEndChar之前的内容到字符串内
	if (!ReadFileContentsBeforeCharEncountered(fInput, wcEndChar, wcReadStr, lReadStrSize))
	{
		return false;
	}

	return true;
}

//忽略空格并从文件中读取字符串
bool ReadStrIgnoreSpace(
	__IO__ FILE *fInput,
	__OUT__ wchar_t *wcReadStr,
	__IN__ const long lReadStrSize)
{
	do
	{
		if (!ReadFileContentsBeforeCharEncountered(fInput, L' ', wcReadStr, lReadStrSize))//在空格之间读取一个非空字符串
		{
			return false;
		}
	}
	while (wcReadStr[0] == L'\0');//如果字符串为空则代表是多个空格，循环直到第一个非空字符串

	return true;
}

//读取并丢弃内容，直到文件下一行
bool JumpFileLine(
	__IO__ FILE *fInput)
{
	while (!feof(fInput) && fgetwc(fInput) != '\n')
	{
		continue;
	}

	if (feof(fInput))
	{
		return false;
	}
	else
	{
		return true;
	}

	//意外执行至此则返回false
	return false;
}

//文件头判断
bool IsUTF16(
	__IO__ FILE *fInput,
	__IN__ const bool LE)//LE == false -> BE
{
	//查看前两个字节
	unsigned short usFileHead = (unsigned short)fgetwc(fInput);

	//0xFEFF是UTF 16 - LE
	//0xFFFE是UTF 16 - BE
	if (LE)
	{
		return usFileHead == (unsigned short)0xFEFF;
	}
	else
	{
		return usFileHead == (unsigned short)0xFFFE;
	}
}

//ABC谱判断
bool IsABCScore(
	__IO__ FILE *fInput)
{
	wchar_t wcReadStr[19];
	if (fread(wcReadStr, sizeof(wchar_t), 18, fInput) != 18)
	{
		return false;
	}
	wcReadStr[18] = L'\0';

	if (wcscmp(wcReadStr, L"<DontCopyThisLine>") == 0)//比较这个字符串来确定是否为ABC谱
	{
		return true;
	}
	else
	{
		fseek(fInput, 18, SEEK_CUR);//如果不是ABC谱则回退刚刚读取的18个字符
		return false;
	}

	//意外执行至此则返回false
	return false;
}

//向文件输出Autojs代码
bool WriteCode(
	__IO__ FILE *fOutput,
	__IN__ const char *cFormatStr,
	__IN__ const char8_t *c8WriteCode[],
	__IN__ const long lCodeElementNumber)
{
	for (long ElementIndex = 0; ElementIndex < lCodeElementNumber; ++ElementIndex)
	{
		if (fprintf(fOutput, cFormatStr, (const char *)c8WriteCode[ElementIndex]) < 0)//失败返回负数
		{
			return false;
		}
	}
	return true;
}

//打开文件并文件输出整个Autojs代码数组然后关闭文件
bool OpenFileAndWriteCode(
	__IN__ const MyString &FileName,
	__IN__ const char8_t *c8WriteCode[],
	__IN__ const long lCodeElementNumber)
{
	FILE *fOutput = fopen(FileName.C_Str(), "wb");
	if (fOutput == NULL)
	{
		return false;
	}

	bool bWriteSuccess = WriteCode(fOutput, "%s\n", c8WriteCode, lCodeElementNumber);
	fclose(fOutput);

	return bWriteSuccess;
}

/******************************文件操作函数******************************/