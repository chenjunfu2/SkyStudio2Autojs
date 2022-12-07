#include "SkyStudio2Autojs.h"

#include "CodeResource.hpp"
#include "FileOperation.h"

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

/******************************转换函数******************************/
//ABC谱面数据转换到按键函数索引
long ABC2NoteKey(
	__IN__ const wchar_t *wcABCStr)
{
	long lABC = (wcABCStr[0] - L'A') * 5;
	long lNum = (wcABCStr[1] - L'0') - 1;
	long lKey = lABC + lNum;

	if (lKey < 0 || lKey >= ARR_ELEMENT_NUMBER(c8NoteFuncCode))//检查计算后的索引是否符合范围
	{
		return (ARR_ELEMENT_NUMBER(c8NoteFuncCode)) - 1;
	}

	return lKey;
}

 void Json2Autojs(
	__IO__ FILE *fInput,
	__IO__ FILE *fOutput,
	__IN__ const bool bIgnoreScoreBeginTime,
	__IN__ const long lNoteTime)
{
	wchar_t wcReadStr[READSTRSIZE];//读取数据存放数组
	long nowTime = 0;//现在时间
	long lastTime = 0;//上一个时间
	long diffTime = 0;//时间差
	const long lNoteTimeArr[3] = { lNoteTime * 1,lNoteTime * 2,lNoteTime * 4 };//时间倍数数组

	//判断是否略过开始等待时间
	if (bIgnoreScoreBeginTime)
	{
		//获取当前时间值
		if (!ReadBetweenStrAndChar(fInput, L"\"time\":", L',', wcReadStr, READSTRSIZE))
		{
			return;
		}
		lastTime = wcstol(wcReadStr, NULL, 10);//设置开始时间为第一个音符的时间，而不以0开始

		goto Ignore_Score_Start_Time_Execute;
	}

	//依次读取时间数据和键位数据生成对应的代码
	while (true)
	{
		//获取当前时间值
		if (!ReadBetweenStrAndChar(fInput, L"\"time\":", L',', wcReadStr, READSTRSIZE))
		{
			return;
		}
		nowTime = wcstol(wcReadStr, NULL, 10);

		//计算时间差值
		diffTime = nowTime - lastTime;

		//根据时间差计算与上一个音的间隔，输出等待函数
		if (diffTime != 0)
		{
			for (long i = 2; i >= 0; --i)//长时间的先输出，最后是短时间的
			{
				for (long j = 0; j < diffTime / lNoteTimeArr[i]; ++j)
				{
					fprintf(fOutput, "%s", (const char *)c8TimeFuncCode[i]);
				}
				diffTime %= lNoteTimeArr[i];
			}
		}

		//设置上一个时间为现在的时间
		lastTime = nowTime;

	Ignore_Score_Start_Time_Execute:
		//获取键位
		if (!ReadBetweenStrAndChar(fInput, L"\"key\":\"", L'"', wcReadStr, READSTRSIZE))
		{
			return;
		}
		long lNoteKey = wcstol(&wcReadStr[4], NULL, 10);
		fprintf(fOutput, "%s", (const char *)c8NoteFuncCode[lNoteKey]);//根据键位值输出触控模拟函数
	}

	return;
}

 void ABC2Autojs(
	__IO__ FILE *fInput,
	__IO__ FILE *fOutput,
	__IN__ const bool bIgnoreScoreBeginTime,
	__IN__ const long lNoteTime)
{
	wchar_t wcReadStr[READSTRSIZE];//读取数据存放数组
	long lNoteInterval = 0;//第一次进入时没有默认间隔
	const long lNotelIntervalArr[3] = { 1,2,4 };//间隔倍数数组

	//判断是否略过开始等待时间
	if (bIgnoreScoreBeginTime)
	{
		//一直读取到非L'.'
		while (true)
		{
			if (!ReadStrIgnoreSpace(fInput, wcReadStr, READSTRSIZE))
			{
				return;//函数失败则结束
			}

			if (wcReadStr[0] != L'.')
			{
				break;//读取到一个非L'.'数据则跳出循环处理
			}
		}
		lNoteInterval = 1;//设置默认间隔为1，而不是开始时候的0

		goto Ignore_Score_Start_Time_Execute;
	}

	//依次读取间隔和键位生成对应的代码
	while (true)
	{
		//一直读取到非L'.'
		while (true)
		{
			if (!ReadStrIgnoreSpace(fInput, wcReadStr, READSTRSIZE))
			{
				return;//函数失败则结束
			}

			if (wcReadStr[0] == L'.')
			{
				++lNoteInterval;//记录L'.'数
			}
			else
			{
				break;//读取到一个非L'.'数据则跳出循环处理
			}
		}

		//根据间隔输出等待函数
		for (long i = 2; i >= 0; --i)//长时间的先输出，最后是短时间的
		{
			for (long j = 0; j < lNoteInterval / lNotelIntervalArr[i]; ++j)
			{
				fprintf(fOutput, "%s", (const char *)c8TimeFuncCode[i]);
			}
			lNoteInterval %= lNotelIntervalArr[i];
		}

		lNoteInterval = 1;//两个音符之间默认间隔是1

	Ignore_Score_Start_Time_Execute:
		//依次解析ABC谱面数据并输出按键模拟函数
		wchar_t *pwcReadStr = wcReadStr;
		while (*pwcReadStr != L'\0')
		{
			const long lNoteKey = ABC2NoteKey(pwcReadStr);
			fprintf(fOutput, "%s", (const char *)c8NoteFuncCode[lNoteKey]);
			pwcReadStr += 2;
		}
	}

	return;
}
/******************************转换函数******************************/