#include "Main.h"

#include "UserInputOperation.h"
#include "SkyStudio2Autojs.h"
#include "ConverterConfig.h"
#include "CodeResource.hpp"
#include "FileOperation.h"
#include "MyString.h"
#include "Menu.h"

#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

//文件报错输出文件路径!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int main(const int argc, char const *const argv[])
{
	//设置语言环境
	setlocale(LC_ALL, "zh-CN");
	//清屏并输出程序头
	ClearScreen();

	//配置文件
	ConverterConfig Config;
	const MyString ConfigFileName = GetConfigComFileName(argv[0], "SkyStudio2Autojs.ini");
	//从程序目录获得完整配置文件名称
	if (ConfigFileName.IsEmpty())
	{
		fprintf(stderr, "配置文件路径获取失败!\n");
		return -1;
	}

	fprintf(stdout, "配置文件路径:%s\n", ConfigFileName.C_Str());

	//读取配置
	if (!Config.ReadConverterConfig(ConfigFileName))
	{
		fprintf(stderr, "配置文件读取失败,");

		Config.SetDefault();
		fprintf(stderr, "已使用默认配置!\n");

		if (!Config.WriteConverterConfig(ConfigFileName))//强制覆写默认配置
		{
			fprintf(stderr, "默认配置文件写入失败!\n\n");
		}

		fprintf(stderr, "默认配置文件写入成功!\n\n");
	}
	else
	{
		fprintf(stdout, "配置文件读取成功!\n\n");
	}
	

	//参数小于2进入命令行输入模式，否则进入直接转换模式
	bool bUserInputMode = (argc <= 1);

	//根据命令行参数进入不同的模式
	if (bUserInputMode)
	{
		while (Menu(&Config, ConfigFileName))//menu continue
		{
			continue;//循环直到用户退出(进入转换模式)
		}

		fprintf(stdout, "文件源目录:%s\n", Config.GetFileSourceDir().C_Str());
		fprintf(stdout, "文件目标目录:%s\n\n", Config.GetFileTargetDir().C_Str());
	}
	else
	{
		if (Config.bDragUseTargetDir())
		{
			//只清理源文件目录，不清理目标文件目录
			Config.SetFileSourceDir(NULL);//设为NULL以在后续的调用中自动使用空串
		}
		else
		{
			//如果是直接转换模式且无需所有文件目录，则置空
			Config.SetFileSourceDir(NULL);
			Config.SetFileTargetDir(NULL);
		}

		fprintf(stdout, "共%d个文件\n\n", argc - 1);
	}

	//依次处理命令行的所有命令(文件名)
	MyString NextFileName;
	long lCurrentIndex = 0;
	while (true)
	{
		++lCurrentIndex;//获得下一个参数索引or文件数目

		if (bUserInputMode)
		{
			fprintf(stdout, "请输入源目录下要转换的文件名:\n");

			if (!NextFileName.GetLine(stdin))
			{
				break;
			}
			
			fprintf(stdout, "\n");
		}
		else
		{
			if (lCurrentIndex >= argc)
			{
				break;
			}

			NextFileName = argv[lCurrentIndex];
		}

		fprintf(stdout, "正在处理第%ld个文件\n", lCurrentIndex);

		//打开输入并创建输出文件
		FILE *fSkyStudio;
		FILE *fAutojs;
		if (!OpenInputAndCreateOuputFile(Config.GetFileSourceDir(), Config.GetFileTargetDir(), NextFileName, &fSkyStudio, &fAutojs))
		{
			continue;//如果函数失败,忽略并处理下一个文件
		}

		//设置SkyStudio输入文件为宽字节
#define WIDE_CHAR 1
#define GET_CHAR_MOD 0
#define MULTI_CHAR -1
		fwide(fSkyStudio, WIDE_CHAR);

		//判断文件头确认文件格式是否正确
		if (!IsUTF16(fSkyStudio, true))
		{
			fprintf(stderr, "错误的文件格式!\n");
			goto Colse_File_And_Next;
		}

		fprintf(stdout, "开始转换...");
		fflush(stdout);

		//abc谱判断
		bool bJsonScore;
		bJsonScore = !IsABCScore(fSkyStudio);

		//获取音符间隔
		long lNoteTime;
		{
			//读取bpm
			long lBpm;//BPM是每分钟的音符数
			wchar_t wcReadStr[READSTRSIZE];
			if (bJsonScore)//处理Json谱
			{
				if (!ReadBetweenStrAndChar(fSkyStudio, L"\"bpm\":", L',', wcReadStr, READSTRSIZE))
				{
					goto Colse_File_And_Next;
				}
			}
			else//处理ABC谱
			{
				if (!ReadStrIgnoreSpace(fSkyStudio, wcReadStr, READSTRSIZE))
				{
					goto Colse_File_And_Next;
				}
			}

			//转换为数值
			lBpm = wcstol(wcReadStr, NULL, 10);
			if (lBpm <= 0 || lBpm > MILLISECONDS_PER_MINUTE)
			{
				goto Colse_File_And_Next;
			}

			//计算间隔
			lNoteTime = MILLISECONDS_PER_MINUTE / lBpm;
		}

		//生成autojs基础代码

		//时间值
		fprintf(fAutojs, "%s%ld%s", (const char *)c8VarCode[0], lNoteTime * 1, (const char *)c8VarCode[1]);
		fprintf(fAutojs, "%s%ld%s", (const char *)c8VarCode[2], lNoteTime * 2, (const char *)c8VarCode[3]);
		fprintf(fAutojs, "%s%ld%s", (const char *)c8VarCode[4], lNoteTime * 4, (const char *)c8VarCode[5]);

		//偏移量
		fprintf(fAutojs, "%s%ld%s", (const char *)c8VarCode[6], Config.uiRandomOffset() * 1, (const char *)c8VarCode[7]);
		fprintf(fAutojs, "%s%ld%s", (const char *)c8VarCode[8], Config.uiRandomOffset() * 2, (const char *)c8VarCode[9]);

		//固定代码
		WriteCode(fAutojs, "%s", c8BaseCode, ARR_ELEMENT_NUMBER(c8BaseCode));


		//根据需要生成可变代码
		if (Config.bUseRandomOffset())
		{
			WriteCode(fAutojs, "%s\n", c8RandomCode, ARR_ELEMENT_NUMBER(c8RandomCode));
		}
		else
		{
			WriteCode(fAutojs, "%s\n", c8NoRandomCode, ARR_ELEMENT_NUMBER(c8NoRandomCode));
		}

		if (Config.bHaveWindow())
		{
			WriteCode(fAutojs, "%s\n", c8WindowCode, ARR_ELEMENT_NUMBER(c8WindowCode));
		}
		else
		{
			WriteCode(fAutojs, "%s\n", c8NoWindowCode, ARR_ELEMENT_NUMBER(c8NoWindowCode) - 2);
			fprintf(fAutojs, "%s%ld%s\n", (const char *)c8NoWindowCode[3], Config.uiNoWindowStartTime(), (const char *)c8NoWindowCode[4]);
		}


		//根据乐谱类型跳转到乐谱数据开始处
		if (bJsonScore)
		{
			if (!JumpFilePointerAfterStr(fSkyStudio, L"\"songNotes\":"))
			{
				goto Colse_File_And_Next;
			}
		}
		else
		{
			if (!JumpFileLine(fSkyStudio))
			{
				goto Colse_File_And_Next;
			}
		}


		//开始转换乐谱数据
		if (bJsonScore)
		{
			Json2Autojs(fSkyStudio, fAutojs, Config.bIgnoreScoreBeginTime(), lNoteTime);
		}
		else
		{
			ABC2Autojs(fSkyStudio, fAutojs, Config.bIgnoreScoreBeginTime(), lNoteTime);
		}


		//关闭文件,然后处理下一个
	Colse_File_And_Next:
		fclose(fSkyStudio);
		fclose(fAutojs);
		fprintf(stdout, "转换完成!\n\n");
	}

	fprintf(stdout, "所有文件处理完毕!\n");
	PressAnyKeyToContinue();
	return 0;
}