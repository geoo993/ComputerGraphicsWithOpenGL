//
//  Control.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 22/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef Control_h
#define Control_h

#pragma once

#include "Common.h"
#include "ControlType.h"
#include "FreeTypeFont.h"
#include "VertexBufferObject.h"

struct MouseState {
    ControlType m_leftButtonDown;
    ControlType m_rightButtonDown;
    ControlType m_middleButtonDown;
    
    GLdouble x;
    GLdouble y;
    
    MouseState(): m_leftButtonDown(ControlType::LEFTMOUSE),
    m_rightButtonDown(ControlType::MIDDLEMOUSE),
    m_middleButtonDown(ControlType::RIGHTMOUSE),
    x(0), y(0) {}
};

struct KeyboardState {
    GLint m_keyDown;
    GLint m_keyReleased;
    GLint m_keyAction;
    
    KeyboardState(): m_keyDown(0), m_keyReleased(0), m_keyAction(0) {}
};

// abstract class
class CControl {

public:
    CControl(GLint positionX, GLint positionY, GLint width, GLint height);
    virtual ~CControl();
    
    virtual void Create() = 0;
    virtual void Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material) = 0;
    virtual GLboolean Update(const MouseState &state);
    virtual std::string GetControlType() = 0;
    
    void SetPosition(GLint x, GLint y);
    void SetSize(GLint width, GLint height);
    
    GLint GetWidth() const;
    GLint GetHeight() const;
    
    virtual void Release();
    
public:
    static std::list<CControl *> m_controls;
    
protected:
    GLboolean           m_isInside;
    
    GLint               m_posX, m_posY;
    GLint               m_width, m_height;
    GLuint              m_vao, m_numTriangles;
    CVertexBufferObject m_vbo;
    
};

CControl * AddControl(CControl *control);
#endif /* Control_h */
