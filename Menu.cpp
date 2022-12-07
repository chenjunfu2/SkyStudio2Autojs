#include "Menu.h"

#include "UserInputOperation.h"
#include "CodeResource.hpp"
#include "FileOperation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SWITCH_TRUE_OR_FALSE(var) ((var)?"是(true)":"否(false)")


//输出帮助信息
static void PrintHelpInfo(
	__UK__ void)
{
	fprintf(stdout, "程序说明:\n");

	fprintf(stdout, "懒得写了,有需要请自行看群内教程,群号:856108603\n\n");

	PressAnyKeyToContinue();
}

//用户设置
static bool Setting(
	__IO__ ConverterConfig *pConfig,
	__OUT__ bool *bUserSaveChanges)
{
	//打印配置
	fprintf(stdout, "设置模式:\n");
	fprintf(stdout, "配置文件版本:0x%X\n\n", pConfig->GetConfigVersion());

	fprintf(stdout, "-1.恢复默认设置\n");
	fprintf(stdout, "00.退出设置模式(退出时可选择是否保留更改)\n\n");

	fprintf(stdout, "01.模拟触控使用随机偏移量:%s\n", SWITCH_TRUE_OR_FALSE(pConfig->bUseRandomOffset()));
	fprintf(stdout, "02.生成的脚本包含控制窗口:%s\n", SWITCH_TRUE_OR_FALSE(pConfig->bHaveWindow()));
	fprintf(stdout, "03.转换时忽略乐谱开始时间:%s\n", SWITCH_TRUE_OR_FALSE(pConfig->bIgnoreScoreBeginTime()));
	fprintf(stdout, "04.拖拽转换输出到目标目录:%s\n\n", SWITCH_TRUE_OR_FALSE(pConfig->bDragUseTargetDir()));

	fprintf(stdout, "05.模拟触控使用的随机偏移量:%dP\n", pConfig->uiRandomOffset());
	fprintf(stdout, "06.无控制窗口时启动等待时间:%dms\n\n", pConfig->uiNoWindowStartTime());

	fprintf(stdout, "07.文件源目录:%s\n", pConfig->GetFileSourceDir().C_Str());
	fprintf(stdout, "08.文件目标目录:%s\n\n", pConfig->GetFileTargetDir().C_Str());

	fprintf(stdout, "\n请输入对应序号选择:\n");

	//获取合法的用户输入
	switch (GetUserInputNumber(-1, 8))//-1~8(包含)
	{
		case -1:
		{
			//恢复默认
			pConfig->SetDefault();
			fprintf(stdout, "已恢复默认设置!\n");
		}
		break;
		case 0:
		{
			//保存并退出
			fprintf(stdout, "保存对设置的修改(true/false):\n");
			*bUserSaveChanges = GetUserInputBool();//获取用户输入
			return false;
		}
		break;
		case 1:
		{
			fprintf(stdout, "要将该设置项修改为(true/false):\n");
			pConfig->bUseRandomOffset() = GetUserInputBool();
		}
		break;
		case 2:
		{
			fprintf(stdout, "要将该设置项修改为(true/false):\n");
			pConfig->bHaveWindow() = GetUserInputBool();
		}
		break;
		case 3:
		{
			fprintf(stdout, "要将该设置项修改为(true/false):\n");
			pConfig->bIgnoreScoreBeginTime() = GetUserInputBool();
		}
		break;
		case 4:
		{
			fprintf(stdout, "要将该设置项修改为(true/false):\n");
			pConfig->bDragUseTargetDir() = GetUserInputBool();
		}
		break;
		case 5:
		{
			fprintf(stdout, "要将该设置项修改为(0~2147483647):\n");
			pConfig->uiRandomOffset() = GetUserInputNumber(0, UINT32_MAX);
		}
		break;
		case 6:
		{
			fprintf(stdout, "要将该设置项修改为(0~2147483647):\n");
			pConfig->uiNoWindowStartTime() = GetUserInputNumber(0, UINT32_MAX);
		}
		break;
		case 7:
		{
			fprintf(stdout, "要将该设置项修改为(文件目录路径):\n");

			//获得一行输入并设置
			MyString UserInputLine;
			UserInputLine.GetLine(stdin);
			pConfig->SetFileSourceDir(UserInputLine);
		}
		break;
		case 8:
		{
			fprintf(stdout, "要将该设置项修改为(文件目录路径):\n");

			//获得一行输入并设置
			MyString UserInputLine;
			UserInputLine.GetLine(stdin);
			pConfig->SetFileTargetDir(UserInputLine);
		}
		break;
		default:
		{
			//nothing to do
		}
		break;
	}

	ClearScreen();

	return true;
}

//辅助功能
static bool Auxiliary(
	__IN__ ConverterConfig &Config)
{
	fprintf(stdout, "辅助功能:\n");
	fprintf(stdout, "0.返回菜单\n");
	fprintf(stdout, "1.生成键位校准脚本\n");
	fprintf(stdout, "2.生成直接弹奏脚本\n\n");
	fprintf(stdout, "\n请输入对应序号来选择功能:\n");

	long lFuncNumber = GetUserInputNumber(0, 2);//0~2(包含)
	ClearScreen();

	//输入验证
	switch (lFuncNumber)
	{
		case 0:
		{
			return false;//直到用户选择退出
		}
		break;
		case 1:
		{
			//输出
			MyString KeyCalibrationFileName = Config.GetFileTargetDir() + "按键校准.js";
			fprintf(stdout, "生成路径:%s\n", KeyCalibrationFileName.C_Str());

			if (!OpenFileAndWriteCode(KeyCalibrationFileName, c8KeyCalibrationCode, ARR_ELEMENT_NUMBER(c8KeyCalibrationCode)))
			{
				fprintf(stderr, "生成失败!\n\n");
				return true;
			}

			fprintf(stdout, "生成成功!\n\n");
		}
		break;
		case 2:
		{
			//输出
			MyString DirectlyPlayScoreName = Config.GetFileTargetDir() + "直接弹奏.js";
			fprintf(stdout, "生成路径:%s\n", DirectlyPlayScoreName.C_Str());

			if (!OpenFileAndWriteCode(DirectlyPlayScoreName, c8DirectlyPlayScoreCode, ARR_ELEMENT_NUMBER(c8DirectlyPlayScoreCode)))
			{
				fprintf(stderr, "生成失败!\n\n");
				return true;
			}

			fprintf(stdout, "生成成功!\n\n");
		}
		break;
		default:
		{
			//nothing to do
		}
		break;
	}

	

	return true;
}

bool Menu(
	__IO__ ConverterConfig *pConfig,
	__IN__ const MyString &ConfigFileName)
{
	fprintf(stdout, "菜单:\n");
	fprintf(stdout, "1.转换模式\n");
	fprintf(stdout, "2.修改设置\n");
	fprintf(stdout, "3.辅助功能\n");
	fprintf(stdout, "4.程序说明\n");
	fprintf(stdout, "\n请输入对应序号来选择功能:\n");

	//输入验证
	long lFuncNumber = GetUserInputNumber(1, 4);//1~4(包含)
	//清屏
	ClearScreen();

	//输入处理
	switch (lFuncNumber)
	{
		case 1:
		{
			return false;//进入转换模式，退出菜单循环
		}
		break;
		case 2:
		{
			bool bUserSaveChanges = false;//用户最后是否保存了设置？
			ConverterConfig UserChangeConfig = *pConfig;//拷贝一份

			while (Setting(&UserChangeConfig, &bUserSaveChanges))//setting continue
			{
				continue;//循环直到返回false(用户退出)
			}

			ClearScreen();

			if (bUserSaveChanges)//用户保留了当前设置
			{
				*pConfig = UserChangeConfig;//覆盖原来的设置数据

				fprintf(stdout, "写入配置文件:%s\n", ConfigFileName.C_Str());
				//写入设置
				if (pConfig->WriteConverterConfig(ConfigFileName))
				{
					fprintf(stdout, "设置已保存!\n\n");
				}
				else
				{
					fprintf(stdout, "设置保存失败!\n\n");
				}
			}
			else
			{
				//什么也不做，保持原先设置不变
				fprintf(stdout, "设置已复原!\n\n");
			}
		}
		break;
		case 3:
		{
			//用户可以选择导出脚本到某些位置
			while (Auxiliary(*pConfig))
			{
				continue;
			}

			ClearScreen();
		}
		break;
		case 4:
		{
			PrintHelpInfo();
			ClearScreen();
		}
		break;
		default:
		{
			//nothing to do
		}
		break;
	}

	return true;//重新进入菜单
}