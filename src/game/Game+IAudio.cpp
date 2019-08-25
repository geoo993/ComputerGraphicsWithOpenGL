//
//  Game+IAudio.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::InitialiseAudio(const std::string &path){
    
    ////add audio files from ////http://freemusicarchive.org/music/Kai_Engel/
    m_audioFiles.push_back(path+"/audio/Kai_Engel_-_02_-_Better_Way.mp3");
    m_audioFiles.push_back(path+"/audio/Kai_Engel_-_03_-_Brooks.mp3");
    m_audioFiles.push_back(path+"/audio/Kai_Engel_-_03_-_Realness.mp3");
    m_audioFiles.push_back(path+"/audio/Kai_Engel_-_08_-_Oecumene_Sleeps.mp3");
    
    m_audioNumber = int(Extensions::randomFloat(0.0f, m_audioFiles.size()));
    
    //// Initialise audio and play background music
    m_pAudio->Initialise();
    m_pAudio->LoadMusicStream(m_audioFiles[m_audioNumber].c_str());
    m_pAudio->PlayMusicStream();
    
}

void Game::UpdateAudio() {
    
    if (m_changeAudio == true) {
        m_pAudio->StopAll();
        m_pAudio->LoadMusicStream(m_audioFiles[m_audioNumber].c_str());
        m_pAudio->PlayMusicStream();
        m_changeAudio = false;
    }
    
    m_pAudio->Update();
}
