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
            m_pFtFont->Render(fontProgram, 20, height - 20, 20, "Mat Shininess: %f", m_materialShininess);
            m_pFtFont->Render(fontProgram, 20, height - 40, 20, "DirLight Intensity: %f", m_dirIntensity);
            m_pFtFont->Render(fontProgram, 20, height - 60, 20, "PointLight Intensity: %f", m_pointIntensity);
            m_pFtFont->Render(fontProgram, 20, height - 80, 20, "SpotLight Intensity: %f", m_spotIntensity);
            m_pFtFont->Render(fontProgram, 20, height - 100, 20, "Use DirLight: %s", Extensions::BoolToString(m_useDir));
            m_pFtFont->Render(fontProgram, 20, height - 120, 20, "Use PointLight: %s", Extensions::BoolToString(m_usePoint));
            m_pFtFont->Render(fontProgram, 20, height - 140, 20, "Use SpotLight: %s", Extensions::BoolToString(m_useSpot));
            m_pFtFont->Render(fontProgram, 20, height - 160, 20, "Light Ambient: %f", m_ambient);
            m_pFtFont->Render(fontProgram, 20, height - 180, 20, "Light Diffuse: %f", m_diffuse);
            m_pFtFont->Render(fontProgram, 20, height - 200, 20, "Light Specular: %f", m_specular);
            m_pFtFont->Render(fontProgram, 20, height - 220, 20, "Light CutOff: %f", m_spotCutOff);
            m_pFtFont->Render(fontProgram, 20, height - 240, 20, "Light OuterCutOff: %f", m_spotOuterCutOff);
            m_pFtFont->Render(fontProgram, 20, height - 260, 20, "Light Att Constant: %f", m_constant);
            m_pFtFont->Render(fontProgram, 20, height - 280, 20, "Light Att Linear: %f", m_linear);
            m_pFtFont->Render(fontProgram, 20, height - 300, 20, "Light Att Exponent: %f", m_exponent);
            m_pFtFont->Render(fontProgram, 20, height - 320, 20, "Use Texture: %s", Extensions::BoolToString(m_woodenBoxesUseTexture));
            m_pFtFont->Render(fontProgram, 20, height - 340, 20, "Use Smooth SpotLight: %s", Extensions::BoolToString(m_woodenBoxesUseTexture));
        }
        
    }
    
}
