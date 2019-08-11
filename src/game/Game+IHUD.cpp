//
//  Game+IHUD.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderHUD(){
    
    CShaderProgram *hudProgram = (*m_pShaderPrograms)[0];
    GLint width = m_gameWindow->GetWidth();
    GLint height = m_gameWindow->GetHeight();
    glm::mat4 orthoMatrix =  glm::ortho(0.0f, GLfloat(width), 0.0f, GLfloat(height));
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);          // Type Of Blending To Perform
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);                   // disable texture mapping
    glShadeModel(GL_SMOOTH);
    
    hudProgram->UseProgram();
    hudProgram->SetUniform("matrices.projMatrix", orthoMatrix); // set othorgraphic view
    SetMaterialUniform(hudProgram, "material", m_textColor);
    
    // render labels
    RenderLabels(m_pFtFont, hudProgram, width, height, m_framesPerSecond, m_enableHud);
    
    glDisable(GL_BLEND);                // Re-Disable Blending
    glEnable(GL_DEPTH_TEST);            // Re-Enable Depth Testing
    glEnable(GL_TEXTURE_2D);            // Re-Enable Texture Mapping
    
}

void Game::RenderLabels(CFreeTypeFont *font, CShaderProgram *fontProgram, const GLint &width, const GLint &height,
                            const GLuint &framesPerSecond,  const bool &enableHud)
{

    if (framesPerSecond > 0) {
        if (enableHud) {
            fontProgram->SetUniform("bUseScreenQuad", false);
            fontProgram->SetUniform("material.bUseTexture", true);
            font->Render(fontProgram, 20, 20, 20, "FPS: %d", framesPerSecond);
            font->Render(fontProgram, (width / 2) - 100, height - 20, 20, "%s", PostProcessingEffectToString(m_currentPPFXMode));
        }
    }
    
}
