#pragma once

#include "D3DUtil.h"
#include "AudioEngine.h"
#include "GameTimer.h"

class Sound
{
	struct SoundInfo
	{
		tstring name;
		float totalPlayTime;
		float playTime;
		int totalLoopCount;
		int loopCount;
		int volume;
	};

public:

	// CONSTRUCTOR - DESTRUCTOR
	Sound(AudioEngine* pAudioEngine, tstring cueName, tstring volumePresetName = _T(""));
	virtual ~Sound();

	// GENERAL
	void PreLoad();
	void Play();
	void Stop();
	void Pause();
	void Tick();
	void Destroy();

	// GETTERS
	bool IsPlaying() const
	{ return m_bPlaying; }
	bool IsLoaded() const
	{ return m_bLoaded; }
	bool IsPaused() const
	{ return m_bPaused; }
	float GetSoundLength() const
	{ return m_SoundLength; }
	const tstring GetSoundInfo() const;
	void GetSoundInfo(SoundInfo* info) const;
	int GetVolume() const;

	// SETTERS
	void SetVolume(int volume);
	void SetLoopCount(int loopCount);

private:

	void UpdateSoundProperties();

	// DATAMEMBERS
	AudioEngine* m_pAudioEngine;

	IXACT3Cue* m_pCue;

	string m_CueName;
	string m_VolumePresetName;

	bool m_bPlaying;
	bool m_bLoaded;
	bool m_bPaused;

	int m_TotalLoopCount;
	int m_LoopCount;
	int m_Volume;

	float m_SoundLength;

	GameTimer m_SoundTimer;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Sound(const Sound& t);
	Sound& operator=(const Sound& t);
};

