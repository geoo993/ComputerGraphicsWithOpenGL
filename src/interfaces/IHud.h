//
//  IHud.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IHud_h
#define IHud_h

#include "../font/FreeTypeFont.h"
#include "../shaders/ShaderProgram.h"

struct IHud
{
    glm::vec4 m_textColor, m_guiColor, m_guiBackgroundColor, m_guiHighlightedColor;
    GLboolean m_enableHud;
    CFreeTypeFont *m_pFtFont;
    virtual void RenderHUD() = 0;
    virtual void RenderLabels(CFreeTypeFont *font, CShaderProgram *fontProgram,
                              const GLint &width, const GLint &height,
                              const GLuint &framesPerSecond, const bool &enableHud) = 0;
};

#endif /* IHud_h */
