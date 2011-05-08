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