//
//  IGameTimer.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IGameTimer_h
#define IGameTimer_h

#include "../timer/HighResolutionTimer.h"

struct IGameTimer
{
    CHighResolutionTimer *m_pGameTimer;
    glm::vec4 m_date;
    GLfloat m_timeInSeconds, m_timeInMilliSeconds, m_timePerSecond, m_channelTime;
    GLdouble m_deltaTime, m_elapsedTime;
    GLint m_framesPerSecond, m_frameCount;
    virtual void UpdateSystemTime() = 0;
    virtual void UpdateGameTime() = 0;
};

#endif /* IGameTimer_h */
