// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if _DEBUG
    #include <NxPhysics.h>
    #pragma warning(disable: 4100)
    #include "ControllerManager.h"

    #define _CrtCheckMemory() {}   //we dont check memory in NxUserAlloc for compatibility reasons
    #include <NxUserAllocatorDefault.h>
    #undef _CrtCheckMemory

    #pragma warning(default: 4100)

    #undef _malloca
    #undef free
    #undef malloc
    #undef realloc
    #undef new
    #undef delete

    //Tell crtdbg to give me information such as line number and source file
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>

#endif



#include <stdlib.h>

#if _DEBUG
    #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif



//#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers, disables fileopendialog
//PhysX:you must suppress its definition of the macros min and max, as these are common C++ method names in the SDK
#define NOMINMAX
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#if _DEBUG
    #undef new
#endif

#include "boost\thread\thread.hpp"
#undef INT8_MIN
#undef INT16_MIN
#undef INT32_MIN
#undef INT64_MIN
#undef INT8_MAX
#undef INT16_MAX
#undef INT32_MAX
#undef INT64_MAX
#undef UINT8_MAX
#undef UINT16_MAX
#undef UINT32_MAX
#undef UINT64_MAX

#if _DEBUG
    #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


//*****************************************************************************
// Assert Macro
//*****************************************************************************
#define XMA2DEFS_ASSERT(a)
#if _DEBUG

struct LocalAssert
{
	static void showAssert(int line, const char* file, bool isOk, const std::string& message = "")
	{	
		if (isOk == false) 
		{
			using namespace std;
			stringstream stream;
			stream << "ERROR! Assertion failure on line " << line << ", in file '" << file << "' \nInfo: " << message;
			cout << stream.str();

			stream << "\nPress abort to close process, Retry to debug, Ignore to continue";
			int ret = MessageBoxA(0, stream.str().c_str(), "Error!", MB_ICONERROR | MB_ABORTRETRYIGNORE);
			switch (ret)
			{
				case IDABORT: cout << "\n***Process aborted***"; ExitProcess(1); break;
				case IDRETRY: cout << "--->Break"; __debugbreak(); break;
				default: break;
			}
		}
	}
};

#define ASSERT(isOk, message) \
		LocalAssert::showAssert(__LINE__, __FILE__, isOk, message)


struct LocalPanic
{
	static void showPanic(int line, const char* file, const std::string& message = "")
	{
		using namespace std;
		stringstream stream;
		stream << "Warning! Panic on line " << line << ", in file '" << file << "' \nInfo: " << message;
		cout << stream.str();

		stream << "\nDo you want to debug?";
		int ret = MessageBoxA(0, stream.str().c_str(), "Warning!", MB_ICONWARNING | MB_YESNO);
		switch (ret)
		{
            case IDYES: cout << "--->Break"; __debugbreak(); break;
			default: break;
		}
	}
};
#define PANIC(message) LocalPanic::showPanic(__LINE__, __FILE__, message)

#else
#define ASSERT(...)
#define PANIC(...)
#endif