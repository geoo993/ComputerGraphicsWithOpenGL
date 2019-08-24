//
//  IAudio.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//
#pragma once

#ifndef IAudio_h
#define IAudio_h

#include "../audio/Audio.h"

struct IAudio {
    CAudio *m_pAudio;
    GLint m_audioNumber;
    GLboolean m_changeAudio;
    std::vector <std::string> m_audioFiles;
    virtual void InitialiseAudio(const std::string &path) = 0;
    virtual void UpdateAudio() = 0;
};

#endif /* IAudio_h */
