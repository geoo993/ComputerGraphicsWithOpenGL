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

#include "../ControlsBase.h"

// abstract class
class CControl {

public:
    CControl(GUIBoxData *data, const GUIMode &mode, const PostProcessingEffectMode &ppfxMode);
    virtual ~CControl();
    
    virtual void Create() = 0;
    virtual void Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material,
    const glm::vec4 &textColor, const glm::vec4 &textHighlightedColor,
    const glm::vec4 &color, const glm::vec4 &backgroundColor) = 0;
    virtual GLboolean Update(const MouseState &state);
    virtual GUIType GetGUIType() = 0;
    
    void SetPosition(GLint x, GLint y);
    void SetSize(GLint width, GLint height);
    void SetIsActive(const GLboolean &active);
    void SetIsInside(const GLboolean &inside);
    
    GLint GetWidth() const;
    GLint GetHeight() const;
    GLboolean GetIsInside() const;
    GLboolean GetIsActive() const;
    GUIMode GetGUIMode() const;
    PostProcessingEffectMode GetPostProcessingEffectMode() const;
    
    virtual void Clear();
    virtual void Release();
    
public:
    static std::list<CControl *> m_controls;
    std::string                     m_uuid;
    
protected:
    GLboolean                       m_isInside, m_isActive;
    GUIMode                         m_mode;
    PostProcessingEffectMode        m_ppfx;
    GLint                           m_posX, m_posY;
    GLint                           m_width, m_height;
    GLuint                          m_vao, m_numTriangles;
    CVertexBufferObject             m_vbo;
    
};

CControl * AddControl(CControl *control);
#endif /* Control_h */
