#include "stdafx.h"
#include "GameWinMain.h"
#include "Engine.h"
#include "MainGame.h"

// console subsystem
void main()
{
	WinMain(0,0,0,0);
}

// windows subsystem

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	//notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL,0);

	// Enable run-time memory leak check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

//***********************
//get command line params
	//LPCWSTR
	int numArgs=0;
	WCHAR **sArgList =  CommandLineToArgvW(GetCommandLineW(), &numArgs);
	//print arguments
	if( NULL == sArgList )
	{
		wprintf(L"CommandLineToArgvW failed\n");
		return 0;
	}
	else 
	{
		wstringstream strstr;
		for(int  i=0; i < numArgs; i++) 
		{
			strstr << i <<  sArgList[i] << _T("\n");
		}
		OutputDebugStringW(strstr.str().c_str());
	}

	LocalFree(sArgList);//free memory
//end of parameters experiment
//****************************

	//create game singleton object
	Engine* pEngine = new Engine(hInstance);
	MainGame* pGame = new MainGame();

	pEngine->SetGame(pGame);
	pEngine->Initialize();

	int ret = pEngine->Run();

	//destroy objects
	delete pEngine;

	return ret;

}