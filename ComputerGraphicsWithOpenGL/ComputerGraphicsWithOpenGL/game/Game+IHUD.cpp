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
    GLint width = m_gameWindow->GetWidth();
    GLint height = m_gameWindow->GetHeight();
    glm::mat4 orthoMatrix =  glm::ortho(0.0f, GLfloat(width), 0.0f, GLfloat(height));
    
    // Use the font shader program and render the text
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    fontProgram->UseProgram();
    fontProgram->SetUniform("matrices.projMatrix", orthoMatrix);
    SetMaterialUniform(fontProgram, "material", glm::vec3(1.0f, 1.0f, 1.0f));
    DisplayFrameRate(fontProgram, width, height, m_framesPerSecond, m_enableHud);
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
}

void Game::DisplayFrameRate(CShaderProgram *fontProgram, const GLint &width, const GLint &height,
                            const GLuint &framesPerSecond,  const bool &enableHud)
{

    if (framesPerSecond > 0) {
        if (enableHud) {
            m_pFtFont->Render(fontProgram, width - 100, height - 20, 20, "FPS: %d", framesPerSecond);
            m_pFtFont->Render(fontProgram, (width / 2) - 100, height - 20, 20, "PPFX: %s", PostProcessingEffectToString(m_currentPPFXMode));
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
            m_pFtFont->Render(fontProgram, 20, height - 320, 20, "Light Smooth SpotLight: %s", Extensions::BoolToString(m_useSmoothSpot));
            m_pFtFont->Render(fontProgram, 20, height - 340, 20, "Light Use Blinn-Phong: %s", Extensions::BoolToString(m_useBlinn));
            m_pFtFont->Render(fontProgram, 20, height - 360, 20, "Light Exposure: %f", m_exposure);
            m_pFtFont->Render(fontProgram, 20, height - 380, 20, "Light Gama: %f", m_gama);
            m_pFtFont->Render(fontProgram, 20, height - 400, 20, "Use Texture: %s", Extensions::BoolToString(m_woodenBoxesUseTexture));
            m_pFtFont->Render(fontProgram, 20, height - 420, 20, "Use Refraction: %s", Extensions::BoolToString(m_useRefraction));
            m_pFtFont->Render(fontProgram, 20, height - 440, 20, "Parallax Height Scale: %f", m_parallaxHeightScale);
            m_pFtFont->Render(fontProgram, 20, height - 460, 20, "UV Tiling: %f", m_uvTiling);
            m_pFtFont->Render(fontProgram, 20, height - 480, 20, "Magnitude: %f", m_magnitude);
            m_pFtFont->Render(fontProgram, 20, height - 500, 20, "PPFX Coverage: %f", m_coverage);
        }
        
    }
    
}
