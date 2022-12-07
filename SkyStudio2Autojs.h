#ifndef SkyStudio2Autojs_h
#define SkyStudio2Autojs_h

//头文件包含
#include "Main.h"

#include <stdio.h>

//函数声明
void Json2Autojs(
	__IO__ FILE *fInput,
	__IO__ FILE *fOutput,
	__IN__ const bool bIgnoreScoreBeginTime,
	__IN__ const long lNoteTime);

void ABC2Autojs(
	__IO__ FILE *fInput,
	__IO__ FILE *fOutput,
	__IN__ const bool bIgnoreScoreBeginTime,
	__IN__ const long lNoteTime);


#endif // SkyStudio2Autojs_hpp