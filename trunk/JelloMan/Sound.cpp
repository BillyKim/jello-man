#include "Sound.h"

// CONSTRUCTOR - DESTRUCTOR
Sound::Sound(AudioEngine* pAudioEngine, tstring cueName)	:	m_pAudioEngine(pAudioEngine),
																m_CueName(cueName),
																m_bPlaying(false),
																m_bLoaded(false),
																m_SoundLength(0.0f)
{
	m_SoundTimer.Reset();
}


Sound::~Sound()
{
}

// GENERAL
void Sound::PreLoad()
{
	string s(m_CueName.begin(), m_CueName.end());
	s.assign(m_CueName.begin(), m_CueName.end());
	
	IXACT3Cue* pCue = m_pAudioEngine->Prepare(s);

	UpdateSoundProperties(pCue);

	m_bLoaded = true;
}
void Sound::Play()
{
	string s(m_CueName.begin(), m_CueName.end());
	s.assign(m_CueName.begin(), m_CueName.end());

	m_pAudioEngine->Play(s);

	m_SoundTimer.Reset();
	m_SoundTimer.Start();

	if (!m_bLoaded) m_bLoaded = true;
	m_bPlaying = true;
}
void Sound::Stop()
{
	string s(m_CueName.begin(), m_CueName.end());
	s.assign(m_CueName.begin(), m_CueName.end());

	if (m_bPlaying)
	{
		m_pAudioEngine->Stop(s);

		m_SoundTimer.Stop();
		m_SoundTimer.Reset();

		m_bPlaying = false;
	}
	else
		cout << "->can't stop sound '" << s << "', reason: is not playing\n";
}
void Sound::Tick()
{
	if (m_bPlaying)
	{
		m_SoundTimer.Tick();

		if (m_SoundTimer.GetGameTime() > m_SoundLength)
		{
			m_bPlaying = false;
			m_SoundTimer.Reset();
		}
	}
}
void Sound::UpdateSoundProperties(IXACT3Cue* pCue)
{
	LPXACT_CUE_INSTANCE_PROPERTIES cueProp;
	pCue->GetProperties(&cueProp);

	m_SoundLength = static_cast<float>(cueProp->activeVariationProperties.soundProperties.arrTrackProperties[0].duration / 1000);
}
const tstring Sound::GetSoundInfo() const
{
	tstringstream stream;
	stream.precision(3);

	stream << _T("Name: ") << m_CueName << _T("\n");
	stream << _T("Length: ") << m_SoundLength << _T(" sec\n");
	stream << _T("Playtime: ") << m_SoundTimer.GetGameTime() << _T(" sec\n");

	return stream.str();
}
void Sound::GetSoundInfo(SoundInfo* info) const
{
	if (info) delete info;

	info = new SoundInfo();
	info->name = m_CueName;
	info->playTime = m_SoundTimer.GetGameTime();
	info->totalPlayTime = m_SoundLength;
	info->loopCount = 0;
	info->totalLoopCount = 0;
}
// SETTERS
void Sound::SetVolume(int volume)
{

}