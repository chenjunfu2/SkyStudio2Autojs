#include "UserInputOperation.h"

#include <tuple>
#include <stdio.h>
#include <conio.h>

#ifdef WINDOWS_EXECUTE
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif // !WINDOWS_EXECUTE

//跳过标准输入行
 void JumpStdinLine(
	__UK__ void)
{
	while (getchar() != '\n')//读取丢弃字符直到遇到换行符
	{
		continue;
	}
}

//按任意键继续
 void PressAnyKeyToContinue(
	__UK__ void)
{
	fprintf(stdout, "按下任意键继续......\n");
	std::ignore = _getch();//Don't warning
}

//清空屏幕
 void ClearScreen(
	__UK__ void)
{
	system(CLEAR_SCREEN);
	//输出程序头
	fprintf(stdout, EXENAME " [V" VERSION "]\nCopyright (C) " YEAR " " COODER ".\n\n");
}

//读入一个在边界内合法的用户输入数值
long GetUserInputNumber(
	__IN__ const long lLowerBound,//下边界(包含)
	__IN__ const long lUpperBound)//上边界(包含)
{
	long lUserInputNumber;
	while (true)
	{
		if (fscanf(stdin, "%ld", &lUserInputNumber) != 1)
		{
			JumpStdinLine();//清掉缓冲区
			fprintf(stderr, "输入有误,请重试:\n");
			continue;
		}

		JumpStdinLine();//清空缓冲区

		if (lUserInputNumber < lLowerBound || lUserInputNumber > lUpperBound)
		{
			fprintf(stderr, "未知的数值,请重试:\n");
			continue;
		}

		break;
	}

	return lUserInputNumber;
}

//获得用户输入的布尔值t/f
bool GetUserInputBool(
	__UK__ void)
{
	char cUserInputChar;
	while (true)
	{
		cUserInputChar = getchar();
		if (cUserInputChar != '\n')
		{
			JumpStdinLine();//清空缓冲区
		}
		else
		{
			continue;
		}

		if (cUserInputChar == 't' || cUserInputChar == 'T')
		{
			return true;
		}
		else if (cUserInputChar == 'f' || cUserInputChar == 'F')
		{
			return false;
		}
		else
		{
			fprintf(stderr, "输入有误,请重试:\n");
		}
	}
}