#ifndef UserInputOperation_h
#define UserInputOperation_h

#include "Main.h"

//跳过标准输入行
 void JumpStdinLine(
	__UK__ void);

//按任意键继续
 void PressAnyKeyToContinue(
	__UK__ void);

//清空屏幕
 void ClearScreen(
	__UK__ void);

//读入一个在边界内合法的用户输入数值
long GetUserInputNumber(
	__IN__ const long lLowerBound,//下边界(包含)
	__IN__ const long lUpperBound);

//获得用户输入的布尔值t/f
bool GetUserInputBool(
	__UK__ void);

#endif // !UserInputOperation_h