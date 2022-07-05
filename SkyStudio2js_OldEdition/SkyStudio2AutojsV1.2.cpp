#include "SkyStudio2Autojs.h"
#include <stdlib.h>

#define TEMPSIZE 32
#define PRESSOFF 18

int main(int argc, char* argv[])
{
	wchar_t temp[32];
	long noteTimeArr[3] = { 0 };
	long nowTime = 0, lastTime = 0, diffTime = 0;
	long note = 0;

	//文件处理
#ifndef NDEBUG
	FILE* fSkyStudio = fopen("test1.txt", "rb");
	FILE* fAutojs = fopen("test1.js", "wb");
#else
	if (argc < 2)
		return 0;

	int filec = 1;
nfile:
	FILE* fSkyStudio = fopen(argv[filec], "rb");

	char* tmpFileName = (char*)malloc((strlen(argv[filec]) + 1) * sizeof(char));
	unsigned long tmpSize = strcpy(tmpFileName, argv[filec]);
	tmpFileName[tmpSize - 3] = 'j';
	tmpFileName[tmpSize - 2] = 's';
	tmpFileName[tmpSize - 1] = 0;

	FILE* fAutojs = fopen(tmpFileName, "wb");

	free(tmpFileName);
	tmpFileName = NULL;
#endif
	if (!fSkyStudio || !fAutojs)
		goto end;
	if (fgetwc(fSkyStudio) != 0xFEFF)
		goto end;

	//读取bpm，计算间隔
	if (!Read2Str(fSkyStudio, L"\"bpm\":"))
		goto end;
	if (!ReadUntilChar(fSkyStudio, L',', temp, TEMPSIZE))
		goto end;

	noteTimeArr[0] = 60000 / _wtol(temp);
	noteTimeArr[1] = noteTimeArr[0] * 2;
	noteTimeArr[2] = noteTimeArr[0] * 4;
	//printf("%d %d %d", noteTimeArr[0], noteTimeArr[1], noteTimeArr[2]);

	//生成autojs基础代码
	for (int i = 0; i < 3; ++i)
		fprintf(fAutojs, "%s%d;", code[i], noteTimeArr[i]);

	fprintf(fAutojs, "%s%d;%s%d;", code[3], PRESSOFF, code[4], PRESSOFF * 2);
	fprintf(fAutojs, "%s%s", code[5], code[6]);

	for (int i = 7; i < sizeof(code) / sizeof(char*); ++i)
		fprintf(fAutojs,"%s\n", code[i]);

	//读取乐谱同时生成autojs演奏代码

	if(!Read2Str(fSkyStudio, L"\"songNotes\":"))
		goto end;

	lastTime = 0;
	while (true)
	{
		if (!Read2Str(fSkyStudio, L"\"time\":"))
			break;
		if (!ReadUntilChar(fSkyStudio, L',', temp, TEMPSIZE))
			break;
		
		nowTime = _wtol(temp);

		if (!Read2Str(fSkyStudio, L"\"key\":\""))
			break;
		if (!ReadUntilChar(fSkyStudio, L'"', temp, TEMPSIZE))
			break;

		note = _wtol(&temp[4]);


		diffTime = nowTime - lastTime;

		if (diffTime)
		{
			for (int i = 2; i >= 0; --i)
			{
				for (int j = 0; j < diffTime / noteTimeArr[i]; ++j)
				{
					fprintf(fAutojs, "%s", timeFunc[i]);
				}
				diffTime %= noteTimeArr[i];
			}
		}

		lastTime = nowTime;

		fprintf(fAutojs, "%s", noteFunc[note]);
	}

	//多文件循环操作
#ifdef NDEBUG
	if (++filec < argc)
	{
		fclose(fSkyStudio);
		fclose(fAutojs);
		goto nfile;
	}
#endif
	//结束清理
end:
	fclose(fSkyStudio);
	fclose(fAutojs);
	return 0;
}