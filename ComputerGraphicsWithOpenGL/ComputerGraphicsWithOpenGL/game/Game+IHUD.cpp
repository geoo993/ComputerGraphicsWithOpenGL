//
//  Game+IHUD.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderHUD(){
    
    CShaderProgram *fontProgram = (*m_pShaderPrograms)[0];
    
    // Use the font shader program and render the text
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    fontProgram->UseProgram();
    fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
    fontProgram->SetUniform("textColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    DisplayFrameRate(fontProgram, m_framesPerSecond, m_enableHud);
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
}

void Game::DisplayFrameRate(CShaderProgram *fontProgram, const GLuint &framesPerSecond,  const bool &enableHud)
{

    int width = m_gameWindow.GetWidth();
    int height = m_gameWindow.GetHeight();
    
    if (framesPerSecond > 0) {
        
        if (enableHud) {
            m_pFtFont->Render(fontProgram, width - 100, height - 20, 20, "FPS: %d", framesPerSecond);
        }
        
    }
    
}
