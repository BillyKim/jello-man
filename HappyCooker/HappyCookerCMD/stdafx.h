// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers, disables fileopendialog
//PhysX:you must suppress its definition of the macros min and max, as these are common C++ method names in the SDK
#define NOMINMAX
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <cmath>
float sqr(float f);

using namespace std;

#ifdef _UNICODE	// extra unicode defines
	#define tstring wstring
	#define tcin wcin
	#define tcout wcout
	#define tstringstream wstringstream
	#define tofstream wofstream
	#define tifstream wifstream
	#define tfstream wfstream
	#define tstrtod wcstod
	#define tstrtol wcstol
	#define tchar wchar_t 
	#define tstrcmp wcscmp
	#define tstrcpy_s wcscpy_s
	#define tfopen _wfopen_s
#else
	#define tstring string
	#define tcin cin
	#define tcout cout
	#define tstringstream stringstream
	#define tofstream ofstream
	#define tifstream ifstream
	#define tfstream fstream
	#define tstrod strtod
	#define tstrtol strtol
	#define tchar char 
	#define tstrcmp strcmp
	#define tstrcpy_s strcpy_s
	#define tfopen fopen_s
#endif


//*****************************************************************************
// Assert Macro
//*****************************************************************************
#define XMA2DEFS_ASSERT(a)
#ifndef NDEBUG
#define ASSERT \
if ( false ) {} \
else \
struct LocalAssert \
{ \
	int mLine; \
	LocalAssert(int line=__LINE__) : mLine(line) {} \
	LocalAssert(bool isOK, const TCHAR* message=_T("")) \
    { \
		if ( !isOK ) \
        { \
			tcout << "ERROR!! Assert failed on line " << LocalAssert().mLine << " in file '" << __FILE__ << "'\nBoodschap: \"" << message << "\"\n"; \
			__asm { int 3 } \
		} \
	} \
} myAsserter = LocalAssert
#else
#define ASSERT \
if ( true ) {} else \
struct NoAssert { \
	NoAssert(bool isOK, const TCHAR* message=_T("")) {} \
} myAsserter = NoAssert
#endif