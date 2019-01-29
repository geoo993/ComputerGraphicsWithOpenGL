#pragma once
#include "Common.h"

class CAudio
{
public:
    
	CAudio();
	~CAudio();
	bool Initialise();
	bool LoadEventSound(const char *filename);
	bool PlayEventSound();
	bool LoadMusicStream(const char *filename);
	bool PlayMusicStream();
	void Update();
    void StopAll();

private:
		
	void FmodErrorCheck(FMOD_RESULT result);

    const int m_numChannels = 32;
    
	FMOD_RESULT result;
	FMOD::System *m_FmodSystem;	// the global variable for talking to FMOD
	FMOD::Sound *m_eventSound;

	FMOD::Sound *m_music;
	FMOD::Channel* m_musicChannel;

};
