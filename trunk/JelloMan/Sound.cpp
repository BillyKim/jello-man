#include "Sound.h"

// CONSTRUCTOR - DESTRUCTOR
Sound::Sound(	AudioEngine* pAudioEngine,
				tstring cueName,
				tstring volumePresetName)	:	m_pAudioEngine(pAudioEngine),
												m_CueName(""),
												m_bPlaying(false),
												m_bLoaded(false),
												m_SoundLength(0.0f),
												m_pCue(0),
												m_LoopCount(0),
												m_bPaused(false),
												m_VolumePresetName(""),
												m_Volume(0)
{
	string s(cueName.begin(), cueName.end());
	s.assign(cueName.begin(), cueName.end());

	m_CueName = s;

	string m(volumePresetName.begin(), volumePresetName.end());
	m.assign(volumePresetName.begin(), volumePresetName.end());

	m_VolumePresetName = m;

	if (volumePresetName != _T(""))
		m_Volume = GetVolume();

	m_SoundTimer.Reset();
}


Sound::~Sound()
{
}

// GENERAL
void Sound::PreLoad()
{	
	m_pCue = m_pAudioEngine->Prepare(m_CueName);

	UpdateSoundProperties();

	m_bLoaded = true;
}
void Sound::Play()
{
	m_pCue = m_pAudioEngine->Play(m_CueName);

	m_SoundTimer.Reset();
	m_SoundTimer.Start();

	if (!m_bLoaded) m_bLoaded = true;
	m_bPlaying = true;
}
void Sound::Stop()
{
	if (m_bPlaying)
	{
		m_pAudioEngine->Stop(m_CueName);

		m_SoundTimer.Stop();
		m_SoundTimer.Reset();

		m_bPlaying = false;
	}
	else
		cout << "->can't stop sound '" << m_CueName << "', reason: is not playing\n";
}
void Sound::Pause()
{
	if (m_bPlaying && !m_bPaused)
	{
		m_pCue->Pause(true);
		m_bPaused = true;

		m_SoundTimer.Stop();

		cout << "->pausing sound '" << m_CueName << "'\n";
	}
	else if (m_bPlaying)
	{
		m_pCue->Pause(false);
		m_bPaused = false;

		m_SoundTimer.Start();

		cout << "->unpausing sound '" << m_CueName << "'\n";
	}
	else
		cout << "->can't pause sound '" << m_CueName << "'\n";
}
void Sound::Tick()
{
	m_SoundTimer.Tick();

	if (m_bPlaying)
	{
		if (m_SoundTimer.GetGameTime() > (m_SoundLength/1000))
		{
			if (m_TotalLoopCount == -1 || m_LoopCount < m_TotalLoopCount)
			{
				Play();
				++m_LoopCount;
			}
			else
			{
				m_bPlaying = false;
				m_SoundTimer.Reset();

				m_LoopCount = 0;
			}
		}

		if (m_VolumePresetName != "")
			m_Volume = GetVolume();
	}
}
void Sound::UpdateSoundProperties()
{
	LPXACT_CUE_INSTANCE_PROPERTIES cueProp;
	m_pCue->GetProperties(&cueProp);

	XACTTIME temp;
	temp = cueProp->activeVariationProperties.soundProperties.arrTrackProperties[0].duration;

	m_SoundLength = static_cast<float>(temp);
}
const tstring Sound::GetSoundInfo() const
{
	tstringstream stream;
	stream.precision(3);

	stream << _T("Name: ") << m_CueName.c_str() << _T("\n");
	stream << _T("Length: ") << m_SoundLength/1000 << _T(" sec\n");
	stream << _T("Playtime: ") << m_SoundTimer.GetGameTime() << _T(" sec\n");
	stream << _T("Loop Count: ") << m_LoopCount << _T("\n");
	stream << _T("Total Loop Count: ") << m_TotalLoopCount << _T("\n");
	stream << _T("Volume: ") << m_Volume << _T("\n");

	return stream.str();
}
void Sound::GetSoundInfo(SoundInfo* info) const
{
	if (info) delete info;

	info = new SoundInfo();

	tstringstream stream;
	stream << m_CueName.c_str();

	info->name = stream.str();
	info->playTime = m_SoundTimer.GetGameTime();
	info->totalPlayTime = m_SoundLength;
	info->loopCount = m_LoopCount;
	info->totalLoopCount = m_TotalLoopCount;
	info->volume = m_Volume;
}
void Sound::Destroy()
{
	if (m_pCue)
	{
		m_pCue->Destroy();

		cout << "->sound '" << m_CueName << "' destroyed\n";
	}
	else
	{
		cout << "->can't destroy sound '" << m_CueName << "', reason: not yet preloaded\n";
	}
}
int Sound::GetVolume() const
{
	if (m_VolumePresetName == "")
	{
		cout << "->can't get volume of sound '" << m_CueName << "', reason: VolumePresetName not set\n";
		return -1;
	}
	else
	{
		return static_cast<int>(m_pAudioEngine->GetGlobalVariable(m_VolumePresetName));
	}
}

// SETTERS
void Sound::SetVolume(int volume)
{
	if (m_VolumePresetName == "")
		cout << "->can't set volume of sound '" << m_CueName << "', reason: VolumePresetName not set\n";
	else
	{
		m_pAudioEngine->SetGlobalVariable(m_VolumePresetName,static_cast<float>(volume));
	}
}
void Sound::SetLoopCount(int loopCount)
{
	if (loopCount >= -1)
		m_TotalLoopCount = loopCount;
}
