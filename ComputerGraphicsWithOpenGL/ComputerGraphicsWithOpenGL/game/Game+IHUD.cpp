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
//    glDisable(GL_TEXTURE_2D);                   // disable texture mapping
//    glShadeModel(GL_SMOOTH);
    
    hudProgram->UseProgram();
    hudProgram->SetUniform("matrices.projMatrix", orthoMatrix); // set othorgraphic view
    SetMaterialUniform(hudProgram, "material", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // render labels
    RenderLabels(m_pFtFont, hudProgram, width, height, m_framesPerSecond, m_enableHud);
    
    glDisable(GL_BLEND);                // Re-Disable Blending
    glEnable(GL_DEPTH_TEST);            // Re-Enable Depth Testing
//    glEnable(GL_TEXTURE_2D);            // Re-Enable Texture Mapping
    
}

void Game::RenderLabels(CFreeTypeFont *font, CShaderProgram *fontProgram, const GLint &width, const GLint &height,
                            const GLuint &framesPerSecond,  const bool &enableHud)
{

    if (framesPerSecond > 0) {
        if (enableHud) {
            fontProgram->SetUniform("bUseScreenQuad", false);
            fontProgram->SetUniform("bUseTexture", true);
            font->Render(fontProgram, 20, 20, 20, "FPS: %d", framesPerSecond);
            font->Render(fontProgram, (width / 2) - 100, height - 20, 20, "PPFX: %s", PostProcessingEffectToString(m_currentPPFXMode));
            /*
            font->Render(fontProgram, 20, height - 20, 20, "Mat Shininess: %f", m_materialShininess);
            font->Render(fontProgram, 20, height - 40, 20, "DirLight Intensity: %f", m_dirIntensity);
            font->Render(fontProgram, 20, height - 60, 20, "PointLight Intensity: %f", m_pointIntensity);
            font->Render(fontProgram, 20, height - 80, 20, "SpotLight Intensity: %f", m_spotIntensity);
            font->Render(fontProgram, 20, height - 100, 20, "Use DirLight: %s", Extensions::BoolToString(m_useDir));
            font->Render(fontProgram, 20, height - 120, 20, "Use PointLight: %s", Extensions::BoolToString(m_usePoint));
            font->Render(fontProgram, 20, height - 140, 20, "Use SpotLight: %s", Extensions::BoolToString(m_useSpot));
            font->Render(fontProgram, 20, height - 160, 20, "Light Ambient: %f", m_ambient);
            font->Render(fontProgram, 20, height - 180, 20, "Light Diffuse: %f", m_diffuse);
            font->Render(fontProgram, 20, height - 200, 20, "Light Specular: %f", m_specular);
            font->Render(fontProgram, 20, height - 220, 20, "Light CutOff: %f", m_spotCutOff);
            font->Render(fontProgram, 20, height - 240, 20, "Light OuterCutOff: %f", m_spotOuterCutOff);
            font->Render(fontProgram, 20, height - 260, 20, "Light Att Constant: %f", m_constant);
            font->Render(fontProgram, 20, height - 280, 20, "Light Att Linear: %f", m_linear);
            font->Render(fontProgram, 20, height - 300, 20, "Light Att Exponent: %f", m_exponent);
            font->Render(fontProgram, 20, height - 320, 20, "Light Smooth SpotLight: %s", Extensions::BoolToString(m_useSmoothSpot));
            font->Render(fontProgram, 20, height - 340, 20, "Light Use Blinn-Phong: %s", Extensions::BoolToString(m_useBlinn));
            font->Render(fontProgram, 20, height - 360, 20, "Light Exposure: %f", m_exposure);
            font->Render(fontProgram, 20, height - 380, 20, "Light Gama: %f", m_gama);
            font->Render(fontProgram, 20, height - 400, 20, "Light HDR: %s", Extensions::BoolToString(m_HDR));
            font->Render(fontProgram, 20, height - 420, 20, "Use Texture: %s", Extensions::BoolToString(m_woodenBoxesUseTexture));
            font->Render(fontProgram, 20, height - 440, 20, "Use Refraction: %s", Extensions::BoolToString(m_useRefraction));
            font->Render(fontProgram, 20, height - 460, 20, "Parallax Height Scale: %f", m_parallaxHeightScale);
            font->Render(fontProgram, 20, height - 480, 20, "UV Tiling: %f", m_uvTiling);
            font->Render(fontProgram, 20, height - 500, 20, "Magnitude: %f", m_magnitude);
            font->Render(fontProgram, 20, height - 520, 20, "SSAO Bias: %f", m_ssaoBias);
            font->Render(fontProgram, 20, height - 540, 20, "SSAO Radius: %f", m_ssaoRadius);
            font->Render(fontProgram, 20, height - 560, 20, "PPFX Coverage: %f", m_coverage);
             */
        }
        
    }
    
}
