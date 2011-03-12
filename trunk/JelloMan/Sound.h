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
	};

public:

	// CONSTRUCTOR - DESTRUCTOR
	Sound(AudioEngine* pAudioEngine, tstring cueName);
	virtual ~Sound();

	// GENERAL
	void PreLoad();
	void Play();
	void Stop();
	void Tick();

	// GETTERS
	bool IsPlaying() const
	{ return m_bPlaying; }
	bool IsLoaded() const
	{ return m_bLoaded; }
	float GetSoundLength() const
	{ return m_SoundLength; }
	const tstring GetSoundInfo() const;
	void GetSoundInfo(SoundInfo* info) const;

	// SETTERS
	void SetVolume(int volume);

private:

	void UpdateSoundProperties(IXACT3Cue* pCue);

	// DATAMEMBERS
	AudioEngine* m_pAudioEngine;

	tstring m_CueName;

	bool m_bPlaying;
	bool m_bLoaded;

	float m_SoundLength;

	GameTimer m_SoundTimer;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Sound(const Sound& t);
	Sound& operator=(const Sound& t);
};

