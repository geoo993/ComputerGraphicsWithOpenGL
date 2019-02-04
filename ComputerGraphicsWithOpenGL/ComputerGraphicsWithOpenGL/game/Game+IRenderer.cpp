//
//  Game+IRenderer.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderScene(){
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    
    // uncomment if stencil buffer is not used
    glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor
    
    // Use terrain shader program
    //CShaderProgram *pTerrainProgram = (*m_pShaderPrograms)[2];
    //SetMaterialUniform(pTerrainProgram, "material");
    //RenderTerrain(pTerrainProgram, true, true);
    
    CShaderProgram *pLampProgram = (*m_pShaderPrograms)[4];
    for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
        RenderLamp(pLampProgram, m_pointLightPositions[i], 10.0f, m_pointLightColors[i]);
    }
    
    CShaderProgram *pLightProgram = (*m_pShaderPrograms)[5];
    for ( GLuint i = 0; i < m_woodenBoxesPosition.size(); ++i){
        GLfloat angle = 20.0f * (GLfloat)i;
        SetCameraUniform(pLightProgram, "camera", m_pCamera);
        SetMaterialUniform(pLightProgram, "material", m_woodenBoxesColor, m_materialShininess);
        SetLightUniform(pLightProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot);
        RenderWoodenBox(pLightProgram, m_woodenBoxesPosition[i], 25.0f, angle, m_woodenBoxesUseTexture);
    }
    
    RenderLight(pLightProgram, m_pCamera);
    
    //CShaderProgram *pPBRProgram = (*m_pShaderPrograms)[3];
    //RenderBarrel(pPBRProgram, 30.0f, true);
    //RenderCube(pPBRProgram, 20.0f, true);
    //RenderSphere(pPBRProgram, 30.0f, true);
    //RenderTorus(pPBRProgram, 5.0f, false);
    //RenderTorusKnot(pPBRProgram, 5.0f, true);
    //RenderMetalBalls(pPBRProgram, 100.0f, true);
    
    // render skybox
    //CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[1];
    //RenderSkyBox(pSkyBoxProgram, CUBEMAPTEXTUREUNIT);
    
}

// Render scene method runs
void Game::Render()
{
    
    RenderScene();
    
    // It is useful to switch back to the default framebuffer for this to easily see your results.
    // Unbind to render to our default framebuffer or switching back to the default buffer at 0.
    // To make sure all rendering operations will have a visual impact on the main window we need to make the default framebuffer active again by binding to 0:
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    m_gameWindow.SetViewport();
    
    // Post Processing Effects
    RenderPPFX( m_currentPPFXMode );
    
    // Draw the 2D graphics after the 3D graphics
    RenderHUD();
    
    // Swap buffers right after rendering all, this is to show the current rendered image
    m_gameWindow.SwapBuffers();
}
