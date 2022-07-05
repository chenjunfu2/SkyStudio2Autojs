#include "resource.h"
#include "SkyStudio2Autojs.h"
#include "String_Operation.h"
#include "File_Operation.h"

#include <stdio.h>
#include <stdlib.h>

#define NOTESIZE 5
static const char* noteFunc[] =
{
	"c4();",
	"d4();",
	"e4();",
	"f4();",
	"g4();",
	"a4();",
	"b4();",
	"c5();",
	"d5();",
	"e5();",
	"f5();",
	"g5();",
	"a5();",
	"b5();",
	"c6();",
};

#define TIMESIZE 5
static const char* timeFunc[] =
{
	"t1();",
	"t2();",
	"t4();",
};

#define TEMPSIZE 32


int main(int argc, char* argv[])
{
	int ret = 0;
	bool bFindCmdH = false;
	bool bFindCmdG = false;
	bool bFindCmdN = false;
	bool bFindCmdT = false;
	bool bFindCmdD = false;
	bool bFindCmdW = false;
	bool bFindCmdC = false;
	bool bFindCmdR = false;
	bool bFindCmdS = false;
	bool bFindCmdF = false;
	char cCmdGVar = 0;
	long lCmdRVar = 0;
	long lCmdSVar = 0;
	char* pCmdFVar = 0;

	//配置文件确认
	HANDLE hConfig = CreateFileInPath(argv[0], "config.ini");
	DWORD err = GetLastError();
	CONFIG stConfig;
	if (hConfig == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "配置文件打开失败！\n\n");
		ret = -2;
		goto end;
	}

	if (err == ERROR_ALREADY_EXISTS)//打开成功
	{
		if (!ReadConfig(hConfig, &stConfig))
		{
			fprintf(stderr, "配置文件读取失败！\n\n");
			ret = -4;
			goto end;
		}
	}
	else//文件不存在
	{
		//写入文件
		if (!WriteDefaultConfig(hConfig))
		{
			fprintf(stderr, "配置文件写入失败！\n\n");
			ret = -3;
			goto end;
		}
		if (!ReadConfig(hConfig, &stConfig))
		{
			fprintf(stderr, "配置文件读取失败！\n\n");
			ret = -4;
			goto end;
		}
	}
	
	fprintf(stdout, "SkyStudio2Autojs [V2.1]\nCopyright (C) 2022 Chen Personal.\n\n");
	
	//参数验证
	if (argc < 2)
	{
		system("start cmd.exe");
		ret = -1;
		goto end;
	}


	//参数解析
	int i;

	if (argv[1][0] != '/' && argv[1][0] != '-')//程序直接接受拖拽产生的参数
	{
		bFindCmdT = true;
		bFindCmdD = true;
		i = 1;
		goto start;
	}

	//错误参数判断、处理命令行指令
	for (i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '/' || argv[i][0] == '-')
		{
			switch (tolower(argv[i][1]))
			{
			case 'h':
			case '?':
			{
				if (bFindCmdT || bFindCmdH)
					goto cmderr;
				bFindCmdH = true;
			}
			break;
			case 'n':
			{
				if (bFindCmdT || bFindCmdN)
					goto cmderr;
				bFindCmdN = true;
			}
			break;
			case 'g':
			{
				if (++i >= argc)
					goto cmdlack;
				if (bFindCmdT || bFindCmdG)
					goto cmderr;
				bFindCmdG = true;
				cCmdGVar = tolower(argv[i][0]);//吃掉参数
			}
			break;
			case 't':
			{
				if (bFindCmdT || bFindCmdG || bFindCmdN || bFindCmdH)
					goto cmderr;
				bFindCmdT = true;
			}
			break;
			case 'd':
			{
				if (!bFindCmdT || bFindCmdD)
					goto cmderr;
				bFindCmdD = true;
			}
			break;
			case 'w':
			{
				if (!bFindCmdT || bFindCmdW)
					goto cmderr;
				bFindCmdW = true;
				if (bFindCmdD)
					break;
			}
			break;
			case 'c':
			{
				if (!bFindCmdT || bFindCmdC)
					goto cmderr;
				bFindCmdC = true;
				if (bFindCmdD)
					break;
			}
			break;
			case 'r':
			{
				if (++i >= argc)
					goto cmdlack;
				if (!bFindCmdT || bFindCmdR)
					goto cmderr;
				bFindCmdR = true;
				if (bFindCmdD)
					break;
				
				if (argv[i][0] == '/' || argv[i][0] == '-' || !IsUnsignedNum(argv[i]))
				{
					--i;//回退参数
					lCmdRVar = stConfig.ulRandomMove;
				}
				else
					lCmdRVar = atol(argv[i]);//吃掉参数
			}
			break;
			case 's':
			{
				if (++i >= argc)
					goto cmdlack;
				if (!bFindCmdT || bFindCmdS)
					goto cmderr;
				bFindCmdS = true;
				if (bFindCmdD)
					break;
				
				if (argv[i][0] == '/' || argv[i][0] == '-' || !IsUnsignedNum(argv[i]))
				{
					--i;//回退参数
					lCmdSVar = stConfig.ulStartSleep;
				}
				else
					lCmdSVar = atol(argv[i]);//吃掉参数
			}
			break;
			case 'f':
			{
				if (++i >= argc)
					goto cmdlack;
				if (!bFindCmdT || bFindCmdF)
					goto cmderr;
				bFindCmdF = true;
				if (bFindCmdD)
					break;
				
				if (argv[i][0] == '/' || argv[i][0] == '-')
				{
					--i;//回退参数
					pCmdFVar = stConfig.pFolderPath;
				}
				else if (argv[i][0] == '?')
				{
					pCmdFVar = stConfig.pFolderPath;//吃掉参数
				}
				else
				{
					pCmdFVar = argv[i];//吃掉参数
				}
			}
			break;
			default:
			{
				fprintf(stderr, "未知参数！\n\n");
				ret = 0;
				goto end;
			}
			break;
			}
		}
		else
		{
			if (!bFindCmdT)
				goto cmderr;
			else
				goto start;
		}
	}


start://先处理命令行结果
	if (bFindCmdH)
	{
		//输出帮助
		DWORD dwHelpSize;
		char* pHelp = (char*)GetResourceData(MAKEINTRESOURCE(IDR_TEMPLATE2), L"template", &dwHelpSize);
		fprintf(stdout, "%.*s", dwHelpSize, pHelp);
	}

	if (bFindCmdN)
	{
		//重新生成配置文件
		if (!WriteDefaultConfig(hConfig))
		{
			fprintf(stderr, "配置文件写入失败！\n\n");
			ret = -5;
			goto end;
		}
		if (!ReadConfig(hConfig, &stConfig))
		{
			fprintf(stderr, "配置文件读取失败！\n\n");
			ret = -6;
			goto end;
		}
		fprintf(stdout, "配置文件覆写成功！\n\n");
	}
	
	if (bFindCmdG)
	{
		//生成辅助文件
		if (cCmdGVar == 'g')
		{
			if (!ResourceToFile(MAKEINTRESOURCE(IDR_DATA1), L"data", L".\\坐标获取与保存.js"))
			{
				fprintf(stderr, "\"坐标获取与保存.js\"写入失败！\n\n");
				ret = -7;
				goto end;
			}
			fprintf(stdout, "已生成文件\"坐标获取与保存.js\"");
		}
		else if (cCmdGVar == 'p')
		{
			if (!ResourceToFile(MAKEINTRESOURCE(IDR_DATA2), L"data", L".\\直接运行SkyStudio乐谱.js"))
			{
				fprintf(stderr, "\"直接运行SkyStudio乐谱.js\"写入失败！\n\n");
				ret = -8;
				goto end;
			}
			fprintf(stdout, "已生成文件\"直接运行SkyStudio乐谱.js\"");
		}
		else
			goto cmderr;

		fprintf(stdout, "\n\n");
	}

	if (bFindCmdT)
	{
		if (i >= argc)
			goto cmdlack;
	}
	else//没有文件要转换
	{
		goto end;
	}

	//使用配置文件默认设置
	if (bFindCmdD)
	{
		bFindCmdW = stConfig.bUseWindow;
		bFindCmdC = stConfig.bCoverFile;
		bFindCmdR = stConfig.bUseRandomMove;
		bFindCmdS = stConfig.bUseStartSleep;
		bFindCmdF = stConfig.bUseFolderPath;

		lCmdRVar = stConfig.ulRandomMove;
		lCmdSVar = stConfig.ulStartSleep;
		pCmdFVar = stConfig.pFolderPath;
	}
	
	if (bFindCmdW)
		bFindCmdS = false;
	if (!bFindCmdR)
		lCmdRVar = 0;
	if (!bFindCmdS)
		lCmdSVar = 0;
	if (!bFindCmdF)
		pCmdFVar = NULL;

	//此处开始按照规则转换文件 argv[i]就是第一个文件位置
	fprintf(stdout, "共%d个文件\n\n", argc - i);

	//处理块
	{
		//得到模板数据
		DWORD dwTemplaSize;
		char* pTemplaData = (char*)GetResourceData(MAKEINTRESOURCE(IDR_TEMPLATE1), L"template", &dwTemplaSize);
		//得出模板变参位置
#define POSCOUNT 22
		DWORD dwPosArr[POSCOUNT] = { 0 };
		DWORD dwArrAddr(1);
		//0~1 t1 2~3 t2 4~5 t4 6~7 o 8~9 o2 10~11 random 12~13 nrandom 14~15 window 16~17 nwindow 18~19 s 19~20 end

		for (DWORD j = 0; j < dwTemplaSize; ++j)
		{
			if (pTemplaData[j] == '/' && ++j < dwTemplaSize)
			{
				if (pTemplaData[j] == '*')// /*
				{
					if (dwArrAddr < POSCOUNT)
						dwPosArr[dwArrAddr++] = ++j - 2;
					else
						break;
				}
				else
				{
					--j;
				}
			}
			else if (pTemplaData[j] == '*' && ++j < dwTemplaSize)
			{
				if (pTemplaData[j] == '/')// */
				{
					if (dwArrAddr < POSCOUNT)
						dwPosArr[dwArrAddr++] = ++j + 0;
					else
						break;
				}
				else
				{
					--j;
				}
			}
		}

		//文件处理
		//填充参数
		long lOutParam[5] = { 0, 0, 0, lCmdRVar, lCmdRVar * 2 };//t1 t2 t4 o o2
		long lFileCount = 0;

		//循环处理块
		while (i < argc)
		{
			wchar_t pTemp[TEMPSIZE];
			long nowTime = 0, lastTime = 0, diffTime = 0;
			long note = 0;
			bool bWideChar = false;

			fprintf(stdout, "正在转换第%d个文件...\n从 %s\n到 ", ++lFileCount , argv[i]);

			HANDLE hReadFile = CreateFileA(argv[i], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			HANDLE hWriteFile = CreateJsFileAtSpecifiedLocation(argv[i], pCmdFVar, bFindCmdC);
			if (hReadFile == INVALID_HANDLE_VALUE)
			{
				fprintf(stderr, "输入文件打开失败\n\n");
				CloseHandle(hReadFile);
				CloseHandle(hWriteFile);
				++i;
				continue;
			}
			if (hWriteFile == INVALID_HANDLE_VALUE)
			{
				fprintf(stderr, "输出文件创建失败\n\n");
				CloseHandle(hReadFile);
				CloseHandle(hWriteFile);
				++i;
				continue;
			}

			//宽字节字符集验证
			{
				wchar_t wc;
				if (!Read2Byte(hReadFile, &wc))
				{
					CloseHandle(hReadFile);
					CloseHandle(hWriteFile);
					goto end;
				}
				bWideChar = wc == 0xFEFF;

				//不是宽字节字符集回退两个字符（定位到文件开头）
				if (!bWideChar)
				{
					if (!SetFilePointerEx(hReadFile, LARGE_INTEGER{ 0 }, NULL, FILE_BEGIN))
					{
						CloseHandle(hReadFile);
						CloseHandle(hWriteFile);
						goto end;
					}
				}
			}



			//读取bpm，计算间隔
			if (bWideChar)
			{
				if (!HandleRead2wStr(hReadFile, L"\"bpm\":"))
				{
					CloseHandle(hReadFile);
					CloseHandle(hWriteFile);
					goto end;
				}
				if (!HandleReadUntilwChar(hReadFile, L',', pTemp, TEMPSIZE))
				{
					CloseHandle(hReadFile);
					CloseHandle(hWriteFile);
					goto end;
				}
				if (IsUnsignedwNum(pTemp))
					lOutParam[0] = 60000 / _wtol(pTemp);
				else
					goto end;
			}
			else
			{
				if (!HandleRead2Str(hReadFile, "\"bpm\":"))
				{
					CloseHandle(hReadFile);
					CloseHandle(hWriteFile);
					goto end;
				}
				if (!HandleReadUntilChar(hReadFile, ',', (char*)pTemp, TEMPSIZE))
				{
					CloseHandle(hReadFile);
					CloseHandle(hWriteFile);
					goto end;
				}
				if (IsUnsignedNum((char*)pTemp))
					lOutParam[0] = 60000 / atol((char*)pTemp);
				else
					goto end;
			}


			lOutParam[1] = lOutParam[0] * 2;
			lOutParam[2] = lOutParam[0] * 4;

			//生成js基础代码
			//0~1 t1 2~3 t2 4~5 t4 6~7 o 8~9 o2 10~11 必填参数
			//random 12~13 nrandom 14~15 可选参数2选1
			//window 16~17 nwindow 18~19 s 20~21 可选参数2选1
			//end

			for (int j = 0, k = 0; j < 12 && k < 5; j += 2)
			{
				if (j)
				{
					ltoa(lOutParam[k++], (char*)pTemp, 10);
					ULONG ulLen = StrLen((char*)pTemp);
					WriteNByte(hWriteFile, pTemp, ulLen);
				}
				WriteNByte(hWriteFile, &pTemplaData[dwPosArr[j]], dwPosArr[j + 1] - dwPosArr[j]);
			}

			//生成js可选代码
			if (bFindCmdR)
			{
				WriteNByte(hWriteFile, &pTemplaData[dwPosArr[12]], dwPosArr[12 + 1] - dwPosArr[12]);
			}
			else
			{
				WriteNByte(hWriteFile, &pTemplaData[dwPosArr[14]], dwPosArr[14 + 1] - dwPosArr[14]);
			}

			if (bFindCmdW)
			{
				WriteNByte(hWriteFile, &pTemplaData[dwPosArr[16]], dwPosArr[16 + 1] - dwPosArr[16]);
			}
			else
			{
				WriteNByte(hWriteFile, &pTemplaData[dwPosArr[18]], dwPosArr[18 + 1] - dwPosArr[18]);

				ltoa(lCmdSVar, (char*)pTemp, 10);
				ULONG ulLen = StrLen((char*)pTemp);
				WriteNByte(hWriteFile, pTemp, ulLen);

				WriteNByte(hWriteFile, &pTemplaData[dwPosArr[20]], dwPosArr[20 + 1] - dwPosArr[20]);
			}

			//生成js乐谱弹奏部分
			if (bWideChar)
			{
				if (!HandleRead2wStr(hReadFile, L"\"songNotes\":"))
				{
					CloseHandle(hReadFile);
					CloseHandle(hWriteFile);
					goto end;
				}
			}
			else
			{
				if (!HandleRead2Str(hReadFile, "\"songNotes\":"))
				{
					CloseHandle(hReadFile);
					CloseHandle(hWriteFile);
					goto end;
				}
			}



			while (true)
			{
				if (bWideChar)
				{
					if (!HandleRead2wStr(hReadFile, L"\"time\":"))
						break;
					if (!HandleReadUntilwChar(hReadFile, L',', pTemp, TEMPSIZE))
						break;

					if (IsUnsignedwNum(pTemp))
						nowTime = _wtol(pTemp);
					else
						goto end;

					if (!HandleRead2wStr(hReadFile, L"\"key\":\""))
						break;
					if (!HandleReadUntilwChar(hReadFile, L'"', pTemp, TEMPSIZE))
						break;

					if (IsUnsignedwNum(&pTemp[4]))
						note = _wtol(&pTemp[4]);
					else
						goto end;
				}
				else
				{
					if (!HandleRead2Str(hReadFile, "\"time\":"))
						break;
					if (!HandleReadUntilChar(hReadFile, ',', (char*)pTemp, TEMPSIZE))
						break;

					if (IsUnsignedNum((char*)pTemp))
						nowTime = atol((char*)pTemp);
					else
						goto end;

					if (!HandleRead2Str(hReadFile, "\"key\":\""))
						break;
					if (!HandleReadUntilChar(hReadFile, '"', (char*)pTemp, TEMPSIZE))
						break;

					if (IsUnsignedNum((char*)pTemp))
						note = atol((char*)&pTemp[4]);
					else
						goto end;
				}


				diffTime = nowTime - lastTime;

				if (diffTime)
				{
					for (int i = 2; i >= 0; --i)
					{
						for (int j = 0; j < diffTime / lOutParam[i]; ++j)
						{
							WriteNByte(hWriteFile, timeFunc[i], TIMESIZE);
						}
						diffTime %= lOutParam[i];
					}
				}

				lastTime = nowTime;

				WriteNByte(hWriteFile, noteFunc[note], NOTESIZE);
			}

			//关闭上一个文件，处理下一个
			CloseHandle(hReadFile);
			CloseHandle(hWriteFile);
			++i;

			fprintf(stdout, "转换成功！\n\n");
		}

	}

	goto end;
cmdlack:
	fprintf(stderr, "缺少参数！\n\n");
	ret = 1;
	goto end;
cmderr:
	fprintf(stderr, "参数错误！\n\n");
	ret = 2;
end:
	CloseHandle(hConfig);
	free(stConfig.pFolderPath);
	stConfig.pFolderPath = 0;
	return ret;
}