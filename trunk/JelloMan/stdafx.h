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