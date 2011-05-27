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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*prevInstance*/,
				   PSTR /*cmdLine*/, int /*showCmd*/)
{
	//notify user if heap is corrupt
	#if defined _DEBUG
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
    #endif

    #pragma region junk
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
    #pragma endregion

	//create game singleton object
	Engine* pEngine = new Engine(hInstance);
	MainGame* pGame = new MainGame();

	#if defined DEBUG || _DEBUG
	cout << "-Engine & Game created\n";
	#endif

	pEngine->SetGame(pGame);
	pEngine->Initialize();

	#if defined DEBUG || _DEBUG
	cout << "------------------------\n:::Engine Initialized:::\n------------------------\n\n";
	#endif

	int ret = pEngine->Run();

	//destroy objects
	delete pGame;
	delete pEngine;

    cout << "\n\n";
    cout << "   Happy Engine wishes you a happy day!   \n";
    cout << "--------------\\Press any key/-------------\n";
    cin.get();
    
	#if defined _DEBUG
    _CrtDumpMemoryLeaks();
    #endif

	return ret;

}