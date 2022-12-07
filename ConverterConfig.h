#ifndef ConverterConfig_h
#define ConverterConfig_h

#include "Main.h"
#include "MyString.h"

#include <stdint.h>

/******************************转换器配置类******************************/
class ConverterConfig
{
private:
	struct
	{
		union//联合
		{
			uint8_t ucMagicChar[sizeof(uint64_t)];
			uint64_t ullMagicNumber;
		}uMagicNumber = { 0 };//魔幻数字
		uint32_t uiConfigVersion = 0;//配置文件版本（版本验证与自动更新）
		uint32_t uiBinDataSize = 0;//配置文件二进制数据部分大小
	}stFileHead;

	struct//存放实际可直接读取的二进制数据
	{
		uint8_t bUseRandomOffset = 0;//模拟触控随机偏移
		uint8_t bHaveWindow = 0;//包含控制窗口
		uint8_t bIgnoreScoreBeginTime = 0;//忽略乐谱开头在第一个音符之前的等待时间
		uint8_t bDragUseTargetDir = 0;//拖拽转换输出到目标目录（否则输出到拖拽文件的源目录）

		uint32_t uiRandomOffset = 0;//模拟触控随机偏移量,以P(像素)为单位
		uint32_t uiNoWindowStartTime = 0;//无控制窗口时的启动等待时间,以ms(毫秒)为单位
	}stBinData;

	struct//使用MyString类代替此处两个数据段
	{
		MyString FileSourceDir = "";//源目录
		MyString FileTargetDir = "";//目标目录
	}stMutableData;

public:
	ConverterConfig(
		__UK__ void) = default;

	~ConverterConfig(
		__UK__ void);

	void SetDefault(
		__UK__ void);

	bool WriteConverterConfig(
		__IN__ const MyString &ConfigFileName) const;

	bool ReadConverterConfig(
		__IN__ const MyString &ConfigFileName);

	/*stFileHead*/
	uint32_t GetConfigVersion(
		__UK__ void) const
	{
		return stFileHead.uiConfigVersion;
	}

	/*stBinData*/
	uint8_t &bUseRandomOffset(
		__UK__ void)
	{
		return stBinData.bUseRandomOffset;
	}

	uint8_t &bHaveWindow(
		__UK__ void)
	{
		return stBinData.bHaveWindow;
	}

	uint8_t &bIgnoreScoreBeginTime(
		__UK__ void)
	{
		return stBinData.bIgnoreScoreBeginTime;
	}

	uint8_t &bDragUseTargetDir(
		__UK__ void)
	{
		return stBinData.bDragUseTargetDir;
	}

	uint32_t &uiRandomOffset(
		__UK__ void)
	{
		return stBinData.uiRandomOffset;
	}

	uint32_t &uiNoWindowStartTime(
		__UK__ void)
	{
		return stBinData.uiNoWindowStartTime;
	}

	/*stMutableData*/
	void SetFileSourceDir(
		__IN__ const MyString &NewFileTargetDir)
	{
		stMutableData.FileSourceDir = NewFileTargetDir;
	}

	void SetFileTargetDir(
		__IN__ const MyString &NewFileSourceDir)
	{
		stMutableData.FileTargetDir = NewFileSourceDir;
	}

	const MyString &GetFileSourceDir(
		__UK__ void) const
	{
		return stMutableData.FileSourceDir;
	}

	const MyString &GetFileTargetDir(
		__UK__ void) const
	{
		return stMutableData.FileTargetDir;
	}
};
/******************************转换器配置类******************************/

/******************************配置文件路径******************************/
MyString GetConfigComFileName(
	__IN__ const MyString &ProgComFileName,
	__IN__ const MyString &ConfigFileName);
/******************************配置文件路径******************************/

#endif // !ConverterConfig_h