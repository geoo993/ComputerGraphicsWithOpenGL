#include "Audio.h"

CAudio::CAudio()
{
    m_musicVolume = 0.8f;
}

CAudio::~CAudio()
{
    Release();
}

bool CAudio::Initialise()
{
	// Create an FMOD system
	m_result = FMOD::System_Create(&m_FmodSystem);
	FmodErrorCheck(m_result);
	if (m_result != FMOD_OK)
		return false;

	// Initialise the system
	m_result = m_FmodSystem->init(m_numChannels, FMOD_INIT_NORMAL, 0);
	FmodErrorCheck(m_result);
	if (m_result != FMOD_OK)
		return false;

	return true;
	
}

// Load an event sound
bool CAudio::LoadEventSound(const char *filename)
{
	m_result = m_FmodSystem->createSound(filename, NULL, 0, &m_eventSound);
	FmodErrorCheck(m_result);
	if (m_result != FMOD_OK)
		return false;

	return true;
	

}

// Play an event sound
bool CAudio::PlayEventSound()
{
	m_result = m_FmodSystem->playSound(m_eventSound, NULL, false, NULL);
    
	FmodErrorCheck(m_result);
	if (m_result != FMOD_OK)
		return false;
    
	return true;
}


// Load a music stream
bool CAudio::LoadMusicStream(const char *filename)
{
	m_result = m_FmodSystem->createStream(filename, NULL | FMOD_LOOP_NORMAL, 0, &m_music);
	FmodErrorCheck(m_result);

	if (m_result != FMOD_OK)
		return false;

	return true;
	

}

// Play a music stream
bool CAudio::PlayMusicStream()
{
	m_result = m_FmodSystem->playSound(m_music, NULL, false, &m_musicChannel);
    m_result = m_musicChannel->setVolume(m_musicVolume);
    
	FmodErrorCheck(m_result);

	if (m_result != FMOD_OK)
		return false;
	return true;
}

// Check for error
void CAudio::FmodErrorCheck(FMOD_RESULT result)
{						
	if (result != FMOD_OK) {
		const char *errorString = FMOD_ErrorString(result);
        //MessageBox(NULL, errorString, "FMOD Error", MB_OK);
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

/*
 increase the volume of the music channel
 */
void CAudio::IncreaseMusicVolume()
{
    // called externally from Game::KeyBoardControls
    // increment the volume
    m_musicVolume += 0.05f;
    if (m_musicVolume > 1)
        m_musicVolume = 1.0f;
    
    m_musicChannel->setVolume(m_musicVolume);
}

/*
 decrease the volume of the music channel
 */
void CAudio::DecreaseMusicVolume()
{
    // called externally from Game::KeyBoardControls
    // deccrement the volume
    m_musicVolume -= 0.05f;
    if (m_musicVolume < 0)
        m_musicVolume = 0.0f;
    
    m_musicChannel->setVolume(m_musicVolume);
}

/*
 get the value of the volume
 */
GLfloat CAudio::Volume() const {
    return  m_musicVolume;
}

void CAudio::Release() {
    m_result = m_eventSound->release();
    m_result = m_FmodSystem->close();
    m_result = m_FmodSystem->release();
    
    delete m_FmodSystem;
    delete m_eventSound;
    delete m_music;
    delete m_musicChannel;
    
}
