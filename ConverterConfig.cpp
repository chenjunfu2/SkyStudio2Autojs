#include "ConverterConfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************默认配置****************************************/
#define MAGIC_CHAR			("Sky2Js\0")
static_assert(sizeof(MAGIC_CHAR) >= sizeof(uint64_t));

#define MAGIC_NUMBER				(*(uint64_t *)MAGIC_CHAR)
#define CONFIG_VERSION				0x12//1.2

#define USE_RANDOMOFFSET			true//按键使用随机偏移
#define HAVE_WINDOW					true//包含窗口
#define IGNORE_SCORE_START_TIME		true//忽略乐谱开头在第一个音符之前的等待时间
#define DRAG_USE_TARGET_DIR			false//拖拽转换输出到目标目录而不是拖拽文件所在目录

#define RANDOM_OFFSET				18//按键随机偏移量,以pixel(像素)为单位
#define NO_WINDOW_START_TIME		1000//无窗口时的启动等待时间,以ms(毫秒)为单位

#ifdef WINDOWS_EXECUTE//pc端为空串
#define FILE_SOURCE_DIR			""
#define FILE_TARGET_DIR			""
#else
#define FILE_SOURCE_DIR			"/storage/emulated/0/SkyStudio2Autojs/"
#define FILE_TARGET_DIR			"/storage/emulated/0/脚本/"
#endif // TEST


/****************************************默认配置****************************************/

/******************************转换器配置类******************************/
ConverterConfig::~ConverterConfig(
	__UK__ void)
{
	memset(&stFileHead, 0, sizeof(stBinData));
	memset(&stBinData, 0, sizeof(stBinData));

	stMutableData.FileSourceDir.Clear();
	stMutableData.FileTargetDir.Clear();
}

void ConverterConfig::SetDefault(
	__UK__ void)
{
	{
		stFileHead.uMagicNumber.ullMagicNumber = MAGIC_NUMBER;
		stFileHead.uiConfigVersion = CONFIG_VERSION;
		stFileHead.uiBinDataSize = sizeof(stBinData);
	}

	{
		stBinData.bUseRandomOffset = USE_RANDOMOFFSET;
		stBinData.bHaveWindow = HAVE_WINDOW;
		stBinData.bIgnoreScoreBeginTime = IGNORE_SCORE_START_TIME;
		stBinData.bDragUseTargetDir = DRAG_USE_TARGET_DIR;

		stBinData.uiRandomOffset = RANDOM_OFFSET;
		stBinData.uiNoWindowStartTime = NO_WINDOW_START_TIME;
	}
	
	{
		stMutableData.FileSourceDir = FILE_SOURCE_DIR;
		stMutableData.FileTargetDir = FILE_TARGET_DIR;
	}
}

bool ConverterConfig::WriteConverterConfig(
	__IN__ const MyString &ConfigFileName) const
{
	//打开配置文件
	FILE *fConfig = fopen(ConfigFileName.C_Str(), "wb");
	if (fConfig == NULL)
	{
		return false;
	}

	//写出结构体头
	if (fwrite(&stFileHead, sizeof(stFileHead), 1, fConfig) != 1)
	{
		fclose(fConfig);
		return false;
	}

	//写出结构体二进制数据段
	if (fwrite(&stBinData, sizeof(stBinData), 1, fConfig) != 1)
	{
		fclose(fConfig);
		return false;
	}

	//写出源字符串
	if (!stMutableData.FileSourceDir.OutLine(fConfig, '\0'))
	{
		fclose(fConfig);
		return false;
	}

	//写出目标字符串
	if (!stMutableData.FileTargetDir.OutLine(fConfig, '\0'))
	{
		fclose(fConfig);
		return false;
	}

	//结束清理
	fclose(fConfig);
	return true;
}

bool ConverterConfig::ReadConverterConfig(
	__IN__ const MyString &ConfigFileName)
{
	//打开配置文件
	FILE *fConfig = fopen(ConfigFileName.C_Str(), "rb");
	if (fConfig == NULL)
	{
		return false;
	}

	//读取前清理
	stMutableData.FileSourceDir.Clear();
	stMutableData.FileTargetDir.Clear();

	//读取结构体头
	if (fread(&stFileHead, sizeof(stFileHead), 1, fConfig) != 1)
	{
		fclose(fConfig);
		return false;
	}

	//验证魔幻数字
	if (stFileHead.uMagicNumber.ullMagicNumber != MAGIC_NUMBER)
	{
		goto Error_return;
	}

	//结构体版本不对则读取失败
	if (stFileHead.uiConfigVersion != CONFIG_VERSION)
	{
		goto Error_return;
	}

	//验证二进制数据大小
	if (stFileHead.uiBinDataSize != sizeof(stBinData))
	{
		goto Error_return;
	}

	//读取二进制数据
	if (fread(&stBinData, sizeof(stBinData), 1, fConfig) != 1)
	{
		goto Error_return;
	}

	//读取源字符串
	if (!stMutableData.FileSourceDir.GetLine(fConfig, '\0'))
	{
		goto Error_return;
	}

	//读取目标字符串
	if (!stMutableData.FileTargetDir.GetLine(fConfig, '\0'))
	{
		goto Error_return;
	}

	//结束清理
	fclose(fConfig);
	return true;
Error_return:
	//错误清理
	fclose(fConfig);
	stMutableData.FileSourceDir.Clear();
	stMutableData.FileTargetDir.Clear();
	return false;
}
/******************************转换器配置类******************************/

/******************************配置文件路径******************************/
MyString GetConfigComFileName(
	__IN__ const MyString &ProgComFileName,
	__IN__ const MyString &ConfigFileName)
{
	if (ProgComFileName.IsEmpty())
	{
		return ConfigFileName;
	}

	//从完整文件名中查找最后一个目录分隔符
	size_t szLastDirSeparator = ProgComFileName.Length();
	bool bFindDirSeparator = false;
	while (szLastDirSeparator != 0)
	{
		--szLastDirSeparator;//倒序遍历
		if (ProgComFileName[szLastDirSeparator] == '\\' || ProgComFileName[szLastDirSeparator] == '/')
		{
			bFindDirSeparator = true;
			break;
		}
	}

	MyString ConfigComFileName(ProgComFileName, 0, szLastDirSeparator + 1);//szLastDirSeparator + 1包含目录分隔符
	ConfigComFileName += ConfigFileName;

	return ConfigComFileName;
}
/******************************配置文件路径******************************/