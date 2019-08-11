#pragma once

#ifndef Audio_h
#define Audio_h

#include "../AudioBase.h"

class CAudio
{
public:
    
	CAudio();
	virtual ~CAudio();
	bool Initialise();
	bool LoadEventSound(const char *filename);
	bool PlayEventSound();
	bool LoadMusicStream(const char *filename);
	bool PlayMusicStream();
	void Update();
    void StopAll();
    
    void IncreaseMusicVolume();
    void DecreaseMusicVolume();
    GLfloat Volume() const;

    void Release();
    
private:
		
	void FmodErrorCheck(FMOD_RESULT result);
    void ToFMODVector(glm::vec3 &glVec3, FMOD_VECTOR *fmodVec);
    
    const int m_numChannels = 32;
    
	FMOD_RESULT m_result;
	FMOD::System *m_FmodSystem;	// the global variable for talking to FMOD
	FMOD::Sound *m_eventSound;

	FMOD::Sound *m_music;
	FMOD::Channel* m_musicChannel;
    
    GLfloat m_musicVolume;

};

#endif /* Audio_h */
