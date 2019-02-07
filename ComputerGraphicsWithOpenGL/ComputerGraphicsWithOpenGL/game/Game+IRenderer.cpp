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
    
    // Render terrain shader program
    CShaderProgram *pTerrainProgram = (*m_pShaderPrograms)[2];
    SetMaterialUniform(pTerrainProgram, "material");
    RenderTerrain(pTerrainProgram, true, true);
    
    // Render Lamps
    CShaderProgram *pLampProgram = (*m_pShaderPrograms)[4];
    for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
        RenderLamp(pLampProgram, m_pointLightPositions[i], 10.0f, m_pointLightColors[i]);
    }
    
    // Use Render Light
    CShaderProgram *pLightProgram = (*m_pShaderPrograms)[5];
    SetCameraUniform(pLightProgram, "camera", m_pCamera);
    SetLightUniform(pLightProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    
    // Render Wooden Boxes
    for ( GLuint i = 0; i < m_woodenBoxesPosition.size(); ++i){
        GLfloat angle = 20.0f * (GLfloat)i;
        SetMaterialUniform(pLightProgram, "material", m_woodenBoxesColor, m_materialShininess);
        RenderWoodenBox(pLightProgram, m_woodenBoxesPosition[i], 25.0f, angle, m_woodenBoxesUseTexture);
    }
    
    // Add Default Lights
    RenderLight(pLightProgram, m_pCamera);
    
    // Render Physically Based Rendering Objects
    CShaderProgram *pPBRProgram = (*m_pShaderPrograms)[3];
    SetCameraUniform(pPBRProgram, "camera", m_pCamera);
    SetLightUniform(pPBRProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    
    // Render Spheres
    GLfloat gap = 100.0f;
    GLuint numRows = 5;
    GLuint numCol = 5;
    glm::vec3 albedo = glm::vec3(0.5f, 0.0f, 0.0f);
    for (GLuint row = 0; row < numRows; row++) {
        GLfloat metallic = (GLfloat)row / (GLfloat)numRows;
        for (GLuint col = 0; col < numCol; col++) {
            GLfloat roughness = glm::clamp((GLfloat)col / (GLfloat)numCol, 0.05f, 1.0f);
            glm::vec3 position = glm::vec3(((GLfloat)row * gap) - 100.0f, 300.0f, ((GLfloat)col * gap) - 100.0f);
            SetPBRMaterialUniform(pPBRProgram, "material", albedo, metallic, roughness);
            SetMaterialUniform(pPBRProgram, "material");
            RenderSphere(pPBRProgram, position, 30.0f, m_woodenBoxesUseTexture);
        }
    }
    
    // Add Physically Based Rendering Lights
    RenderLight(pPBRProgram, m_pCamera);
    
    //RenderBarrel(pPBRProgram, m_barrelPosition, 1.0f, true);
    //RenderCube(pPBRProgram, m_cubePosition, 20.0f, true);
    
    //RenderSphere(pPBRProgram, m_spherePosition, 30.0f, true);
    //RenderTorus(pPBRProgram, m_torusPosition, 5.0f, false);
    //RenderTorusKnot(pPBRProgram, m_torusKnotPosition, 5.0f, true);
    //RenderMetalBalls(pPBRProgram, m_metalballsPosition, 100.0f, true);
    CShaderProgram *pNormalMappingProgram = (*m_pShaderPrograms)[6];
    SetMaterialUniform(pNormalMappingProgram, "material", glm::vec3(0.3f, 0.1f, 0.7f), m_materialShininess);
    //RenderGrenade(pNormalMappingProgram,  glm::vec3(600.0f, 200.0f, -500.0f), 20.0f, true);
    RenderNanosuit(pNormalMappingProgram,  glm::vec3(600.0f, 200.0f, -500.0f), 20.0f, true);
    
    /*
    // Render skybox
    CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[1];
    RenderSkyBox(pSkyBoxProgram, CUBEMAPTEXTUREUNIT);
    */
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
