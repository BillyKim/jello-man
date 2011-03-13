#pragma once

#include <xact3.h>
#include <Xact3d3.h>

#pragma comment(lib, "X3DAudio.lib")

using namespace std;

class AudioEngine
{
public:
	AudioEngine(wstring projectName);
	virtual ~AudioEngine(void);
	HRESULT Initialize();
	void Cleanup();
	void DoWork();
	IXACT3Cue* Play(string name);
	void Stop(string name);
	IXACT3Cue* Prepare(string name);
	void Update3DAudio(IXACT3Cue *p3DCue, X3DAUDIO_EMITTER &emitter, const X3DAUDIO_LISTENER &listener);
	void SetGlobalVariable(string name, float value);
	float GetGlobalVariable(string name);

private:

	IXACT3Engine *m_pXACT3Engine;
	IXACT3WaveBank *m_pWaveBank;
	IXACT3SoundBank *m_pSoundBank;
	tstring m_ProjectName;
	wstring m_sWavebank;
	wstring m_sSoundbank;
	wstring m_sGlobalSettings;
	XACTINDEX m_iCueIndex;
	void *m_pMapViewWaveBank, *m_pMapViewSoundBank, *m_pGlobalSettings;
	X3DAUDIO_HANDLE m_Xact3dInstance;
	X3DAUDIO_DSP_SETTINGS m_3DAudioDSPSettings;
	IXACT3Cue *m_p3DCue;
	AudioEngine(const AudioEngine& t);
	AudioEngine& operator=(const AudioEngine& t);
};

