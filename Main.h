#ifndef Main_h
#define Main_h

/******************************辅助定义******************************/
#define __IN__		//输入参数
#define __OUT__		//输出参数
#define __IO__		//交互参数
#define __UK__		//未定义参数

#define WINDOWS_EXECUTE
#define READSTRSIZE 32		//临时数组大小
#define USERINPUTMAXLEN 256		//用户输入数据上限
#define MILLISECONDS_PER_MINUTE 60000		//每分钟的毫秒数
#define ARR_ELEMENT_NUMBER(arr) (sizeof(arr)/sizeof(*(arr)))		//求取某数组元素个数
/******************************辅助定义******************************/

/******************************程序信息******************************/
#define EXENAME		"SkyStudio2Autojs"
#define VERSION		"2.9"
#define YEAR		"2022"
#define COODER		"Chen Personal"
/******************************程序信息******************************/

#endif // Main_h