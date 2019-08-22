//
//  ControlType.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 25/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef ControlType_h
#define ControlType_h

#include "../UtilitiesBase.h"

enum class ControlType {
    // Define mouse touches
    LEFTMOUSE,
    MIDDLEMOUSE,
    RIGHTMOUSE,
    
    // Defines several possible options for camera movement.
    KEYFORWARD,
    KEYBACKWARD,
    KEYLEFT,
    KEYRIGHT,
    
    UNKNOWN
};

struct MouseState {
    GLboolean m_isEnabled;
    
    ControlType m_leftButtonDown;
    ControlType m_rightButtonDown;
    ControlType m_middleButtonDown;
    
    GLdouble x;
    GLdouble y;
    GLdouble xoffset;
    GLdouble yoffset;
    GLboolean m_isDragging;
    GLboolean m_isNavigating;
    GLdouble m_scroll;
    
    MouseState(): m_leftButtonDown(ControlType::UNKNOWN),
    m_rightButtonDown(ControlType::UNKNOWN),
    m_middleButtonDown(ControlType::UNKNOWN),
    x(0), y(0), m_scroll(0), xoffset(0), yoffset(0),
    m_isEnabled(true), m_isDragging(false), m_isNavigating(false) {}
};

struct KeyboardState {
    ControlType m_arrowKeyDown;
    GLint m_keyPressed;
    GLint m_keyReleased;
    GLdouble m_keyPressTime;
    GLdouble m_lastKeyPressTime;
    GLboolean m_isSlowMotion;
    KeyboardState(): m_arrowKeyDown(ControlType::UNKNOWN),
    m_keyPressed(0), m_keyReleased(0), m_keyPressTime(0), m_lastKeyPressTime(0),
    m_isSlowMotion(false)  {}
};


#endif /* ControlType_h */
