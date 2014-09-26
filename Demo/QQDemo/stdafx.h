// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once




#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include <windows.h>
#include <objbase.h>
#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shlwapi.h>
#include <list>


// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include "..\..\DuiLib\UIlib.h"

#ifndef NO_USING_DUILIB_NAMESPACE
using namespace DuiLib;
using namespace std;
#endif

#include "debug.h"


#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\..\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\..\\Lib\\DuiLib.lib")
#   endif
#endif

#define MESSAGE_RICHEDIT_MAX  1024

