//
//  IControls.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IControls_h
#define IControls_h

struct IControls
{
    // inputs
    GLboolean m_mouseButtonDown;
    GLdouble m_mouseX;
    GLdouble m_mouseY;
    virtual const char * const ControlTypeToString(const ControlType &type) = 0;
    virtual void LoadControls() = 0;
    virtual void RenderControls() = 0;
    virtual void UpdateControls() = 0;
    virtual void ClearControls() = 0;
    virtual void RemoveControls() = 0;
    virtual void UpdateKeyBoardControls(KeyboardState &state) = 0;
    virtual void UpdateMouseControls(MouseState &state) = 0;
};

#endif /* IControls_h */
