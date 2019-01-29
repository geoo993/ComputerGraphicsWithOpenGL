#include "Audio.h"

CAudio::CAudio()
{}

CAudio::~CAudio()
{}

bool CAudio::Initialise()
{
	// Create an FMOD system
	result = FMOD::System_Create(&m_FmodSystem);
	FmodErrorCheck(result);
	if (result != FMOD_OK) 
		return false;

	// Initialise the system
	result = m_FmodSystem->init(m_numChannels, FMOD_INIT_NORMAL, 0);
	FmodErrorCheck(result);
	if (result != FMOD_OK) 
		return false;

	return true;
	
}

// Load an event sound
bool CAudio::LoadEventSound(const char *filename)
{
	result = m_FmodSystem->createSound(filename, NULL, 0, &m_eventSound);
	FmodErrorCheck(result);
	if (result != FMOD_OK) 
		return false;

	return true;
	

}

// Play an event sound
bool CAudio::PlayEventSound()
{
	result = m_FmodSystem->playSound(m_eventSound, NULL, false, NULL);
    //result = m_FmodSystem->playSound(m_eventSound, FMOD_INIT_NORMAL, false, 0);
    
	FmodErrorCheck(result);
	if (result != FMOD_OK)
		return false;
    
	return true;
}


// Load a music stream
bool CAudio::LoadMusicStream(const char *filename)
{
	result = m_FmodSystem->createStream(filename, NULL | FMOD_LOOP_NORMAL, 0, &m_music);
	FmodErrorCheck(result);

	if (result != FMOD_OK) 
		return false;

	return true;
	

}

// Play a music stream
bool CAudio::PlayMusicStream()
{
	result = m_FmodSystem->playSound(m_music, NULL, false, &m_musicChannel);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;
	return true;
}

// Check for error
void CAudio::FmodErrorCheck(FMOD_RESULT result)
{						
	if (result != FMOD_OK) {
		const char *errorString = FMOD_ErrorString(result);
	}
}

void CAudio::StopAll(){
    
    for(int i=0; i<m_numChannels; i++)
    {
        FMOD::Channel* pChannel = nullptr;
        FMOD_RESULT res = m_FmodSystem->getChannel(i, &pChannel);
        
        if(res == FMOD_OK && pChannel)
        {
            pChannel->stop();
        }
    }
}

void CAudio::Update()
{
	m_FmodSystem->update();
}
