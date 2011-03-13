#include "D3DUtil.h"
#include "AudioEngine.h"
#include "Engine.h"

AudioEngine::AudioEngine(wstring projectName) 
	:m_pXACT3Engine(0)
	,m_pWaveBank(0)
	,m_pSoundBank(0)
	,m_iCueIndex(0)
	,m_pMapViewWaveBank(0)
	,m_pMapViewSoundBank(0)
	,m_pGlobalSettings(0)
	,m_p3DCue(0)
{
	ZeroMemory(&m_3DAudioDSPSettings,sizeof(X3DAUDIO_DSP_SETTINGS));
	ZeroMemory(&m_Xact3dInstance, sizeof(X3DAUDIO_HANDLE_BYTESIZE));

	m_ProjectName = projectName;
	
	m_sWavebank = projectName;
	m_sWavebank.append(L".xwb");
	m_sSoundbank =  projectName;
	m_sSoundbank.append(L".xsb");
	m_sGlobalSettings = projectName;
	m_sGlobalSettings.append(L".xgs");

	
}

AudioEngine::~AudioEngine(void)
{
	Cleanup();
}
void AudioEngine::Cleanup()
{
	if(m_pSoundBank)HR(m_pSoundBank->Destroy());
	if(m_pWaveBank)HR(m_pWaveBank->Destroy());
	if(m_pXACT3Engine)
	{
		HR(m_pXACT3Engine->ShutDown());
		m_pXACT3Engine->Release();
		m_pXACT3Engine=0;
	}

	if( m_pGlobalSettings ) delete[] m_pGlobalSettings;
	m_pGlobalSettings = NULL;

	if( m_pMapViewSoundBank ) delete[] m_pMapViewSoundBank;
	m_pMapViewSoundBank = NULL;

    // After pEngine->ShutDown() returns it is safe to release memory mapped files
    if( m_pMapViewWaveBank )UnmapViewOfFile( m_pMapViewWaveBank );
    m_pMapViewWaveBank = NULL;

    CoUninitialize();

}
HRESULT AudioEngine::Initialize()
{
	//On Windows, the application must call the CoInitializeEx method before calling XACT3CreateEngine. 
	//In addition, the rules of COM require CoInitialize or CoInitializeEx to be called in any thread 
	//that makes calls to XACT.

	HR(CoInitializeEx(NULL, COINIT_MULTITHREADED));
        
	bool bDebugMode = false;
	bool bAuditionMode = false;

	DWORD dwCreationFlags = 0;
	if( bAuditionMode ) dwCreationFlags |= XACT_FLAG_API_AUDITION_MODE;
	if( bDebugMode ) dwCreationFlags |= XACT_FLAG_API_DEBUG_MODE;

	HR(XACT3CreateEngine( dwCreationFlags, &m_pXACT3Engine ));

	XACT_RUNTIME_PARAMETERS EngineParameters = {0};

	HRESULT hr = E_FAIL; // assume failure
	HANDLE hFile;
	DWORD dwFileSize=0;
	DWORD dwBytesRead=0;
	HANDLE hMapFile;

	//read global settings (.xgs)
	hr = E_FAIL; // assume failure
    hFile = CreateFile( m_sGlobalSettings.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            // Allocate the data here and free the data when recieving the sound bank destroyed notification
            m_pGlobalSettings = new BYTE[dwFileSize];
            if( m_pGlobalSettings )
            {
                if( 0 != ReadFile( hFile, m_pGlobalSettings, dwFileSize, &dwBytesRead, NULL ) )
                {
					EngineParameters.pGlobalSettingsBuffer=m_pGlobalSettings;
					EngineParameters.globalSettingsBufferSize=dwFileSize;
				}
			}
		}
	}
	hr = m_pXACT3Engine->Initialize( &EngineParameters ) ;
	if( FAILED( hr ) )
	{
		string s(m_ProjectName.begin(), m_ProjectName.end());
		s.assign(m_ProjectName.begin(), m_ProjectName.end());

		cout << "-AudioEngine project: " << s << " initialization FAILED!\n";

		return E_FAIL;
	}

	//Initalize the X3AudioEngine
	XACT3DInitialize(m_pXACT3Engine,m_Xact3dInstance);
	X3DAUDIO_DSP_SETTINGS DSPSettings = {0};
	WAVEFORMATEXTENSIBLE format;
	m_pXACT3Engine->GetFinalMixFormat(&format);

	m_3DAudioDSPSettings.SrcChannelCount = 2;//srcChannelCount;
	m_3DAudioDSPSettings.DstChannelCount = format.Format.nChannels;    
	m_3DAudioDSPSettings.pMatrixCoefficients = new FLOAT32[DSPSettings.SrcChannelCount * DSPSettings.DstChannelCount];


	// Create an "in memory" XACT wave bank file using memory mapped file IO
    // Memory mapped files tend to be the fastest for most situations assuming you 
    // have enough virtual address space for a full map of the file
 

    hFile = CreateFile( m_sWavebank.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
            if( hMapFile )
            {
                m_pMapViewWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( m_pMapViewWaveBank )
                {
                    hr = m_pXACT3Engine->CreateInMemoryWaveBank( m_pMapViewWaveBank, dwFileSize, 0,
                                                                       0, &m_pWaveBank );
                }
                CloseHandle( hMapFile ); // pbWaveBank maintains a handle on the file so close this unneeded handle
            }
        }
        CloseHandle( hFile ); // pbWaveBank maintains a handle on the file so close this unneeded handle
    }
    if( FAILED( hr ) )
	{
		string s(m_ProjectName.begin(), m_ProjectName.end());
		s.assign(m_ProjectName.begin(), m_ProjectName.end());

		cout << "-AudioEngine project: " << s << " initialization FAILED!\n";

        return E_FAIL; // CleanupXACT() will cleanup state before exiting
	}

    // Read and register the sound bank file with XACT.  Do not use memory mapped file IO because the 
    // memory needs to be read/write and the working set of sound banks are small.
    hr = E_FAIL; // assume failure
    hFile = CreateFile( m_sSoundbank.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            // Allocate the data here and free the data when recieving the sound bank destroyed notification
            m_pMapViewSoundBank = new BYTE[dwFileSize];
            if( m_pMapViewSoundBank )
            {
                if( 0 != ReadFile( hFile, m_pMapViewSoundBank, dwFileSize, &dwBytesRead, NULL ) )
                {
                    hr = m_pXACT3Engine->CreateSoundBank( m_pMapViewSoundBank, dwFileSize, 0,
                                                                0, &m_pSoundBank);
                }
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( hr ) )
	{
		string s(m_ProjectName.begin(), m_ProjectName.end());
		s.assign(m_ProjectName.begin(), m_ProjectName.end());

		cout << "-AudioEngine project: " << s << " initialization FAILED!\n";

		return E_FAIL; // Cleanup() will cleanup before exiting
	}

	string s(m_ProjectName.begin(), m_ProjectName.end());
	s.assign(m_ProjectName.begin(), m_ProjectName.end());

	cout << "-AudioEngine initialized project: " << s << "\n";

	return S_OK;
}
void AudioEngine::Update3DAudio(IXACT3Cue *p3DCue, X3DAUDIO_EMITTER &emitter, const X3DAUDIO_LISTENER &listener)
{
	XACT3DCalculate( m_Xact3dInstance, &listener, &emitter, &m_3DAudioDSPSettings );
	XACT3DApply( &m_3DAudioDSPSettings, p3DCue);
}
void AudioEngine::DoWork()
{
	m_pXACT3Engine->DoWork();
}

IXACT3Cue* AudioEngine::Play(string name)
{
	XACTINDEX dwSoundCueIndex=0;
	dwSoundCueIndex = m_pSoundBank->GetCueIndex( name.c_str() );
	if(dwSoundCueIndex == XACTINDEX_INVALID)
	{
		cout << "->invalid sound name: " << name << "\n";
		return 0;
	}

	IXACT3Cue* pCue=0;
	m_pSoundBank->Stop(dwSoundCueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
	HR(m_pSoundBank->Play(dwSoundCueIndex, 0, 0, &pCue));	

	cout << "->playing sound: " << name << "\n";

	return pCue;
}
void AudioEngine::Stop(string name)
{
	XACTINDEX dwSoundCueIndex=0;
	dwSoundCueIndex = m_pSoundBank->GetCueIndex( name.c_str() );
	if(dwSoundCueIndex == XACTINDEX_INVALID)
	{
		cout << "->invalid sound name: " << name << "\n";
		return;
	}

	m_pSoundBank->Stop(dwSoundCueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);	

	cout << "->stopping sound: " << name << "\n";
}
IXACT3Cue* AudioEngine::Prepare(string name)
{
	XACTINDEX dwSoundCueIndex=0;
	dwSoundCueIndex = m_pSoundBank->GetCueIndex( name.c_str() );
	if(dwSoundCueIndex == XACTINDEX_INVALID)
	{
		cout << "->invalid sound name: " << name << "\n";
		return 0;
	}

	IXACT3Cue* pCue=0;
	//m_pSoundBank->Stop(m_dwSoundCueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
	HR(m_pSoundBank->Prepare( dwSoundCueIndex, 0, 0, &pCue));

	cout << "->preloaded sound: " << name << "\n";

	return pCue;
}
void AudioEngine::SetGlobalVariable(string name, float value)
{
	XACTINDEX varI;
	varI = m_pXACT3Engine->GetGlobalVariableIndex(name.c_str());
	
	m_pXACT3Engine->SetGlobalVariable(varI,value);
}
float AudioEngine::GetGlobalVariable(string name)
{
	XACTINDEX varI;
	varI = m_pXACT3Engine->GetGlobalVariableIndex(name.c_str());
	
	float var = 0.0f;
	m_pXACT3Engine->GetGlobalVariable(varI,&var);

	return var;
}
