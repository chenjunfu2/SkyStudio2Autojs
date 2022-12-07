#ifndef Menu_h
#define Menu_h

#include "Main.h"
#include "ConverterConfig.h"
#include "MyString.h"

bool Menu(
	__IO__ ConverterConfig *pConfig,
	__IN__ const MyString &ConfigFileName);

#endif // !Menu_h