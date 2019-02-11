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
    
    /// Render skybox
    CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[1];
    SetMaterialUniform(pSkyBoxProgram, "material");
    RenderSkyBox(pSkyBoxProgram);
    
    /// Render terrain
    //CShaderProgram *pTerrainProgram = (*m_pShaderPrograms)[2];
    //SetMaterialUniform(pTerrainProgram, "material");
    //RenderTerrain(pTerrainProgram, true, true);
    
    /// Render Lamps
    CShaderProgram *pLampProgram = (*m_pShaderPrograms)[4];
    for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
        RenderLamp(pLampProgram, m_pointLightPositions[i], 10.0f, m_pointLightColors[i]);
    }
    
    /// Render Lights
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
    
    ///  Physically Based Rendering
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
            SetMaterialUniform(pPBRProgram, "material", glm::vec3(0.3f, 0.1f, 0.7f), m_materialShininess);
            RenderSphere(pPBRProgram, position, 30.0f, m_woodenBoxesUseTexture);
        }
    }
    //RenderGrenade(pNormalMappingProgram,  glm::vec3(600.0f, 200.0f, -500.0f), 20.0f, true);
    
    // Add Physically Based Rendering Lights
    RenderLight(pPBRProgram, m_pCamera);
    
    /// Normal Mapping
    CShaderProgram *pNormalMappingProgram = (*m_pShaderPrograms)[6];
    SetCameraUniform(pNormalMappingProgram, "camera", m_pCamera);
    SetLightUniform(pNormalMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pNormalMappingProgram, "material", glm::vec3(0.3f, 0.1f, 0.7f), m_materialShininess);
    RenderCube(pNormalMappingProgram, glm::vec3(1000.0f, 500.0f, 1000.0f), 100.0f, true);
    
    // Add Normal mapping Lights
    RenderLight(pNormalMappingProgram, m_pCamera);
    
    /// Bump Mapping
    CShaderProgram *pBumpMappingProgram = (*m_pShaderPrograms)[7];
    SetCameraUniform(pBumpMappingProgram, "camera", m_pCamera);
    SetLightUniform(pBumpMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pBumpMappingProgram, "material", glm::vec3(0.3f, 0.1f, 0.7f), m_materialShininess);
    SetBumpMapUniform(pBumpMappingProgram, m_uvTiling);
    RenderCube(pBumpMappingProgram, glm::vec3(500.0f, 500.0f, 1000.0f), 100.0f, true);
    RenderTorusKnot(pBumpMappingProgram, m_torusKnotPosition, 5.0f, true);
    
    // Add Bump mapping Lights
    RenderLight(pBumpMappingProgram, m_pCamera);
    
    /// Parallax Normal Mapping
    CShaderProgram *pParallaxNormalMappingProgram = (*m_pShaderPrograms)[8];
    SetCameraUniform(pParallaxNormalMappingProgram, "camera", m_pCamera);
    SetLightUniform(pParallaxNormalMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pParallaxNormalMappingProgram, "material", glm::vec3(0.3f, 0.1f, 0.7f), m_materialShininess);
    SetParallaxMapUniform(pParallaxNormalMappingProgram, m_parallaxHeightScale);
    RenderParallaxCube(pParallaxNormalMappingProgram, glm::vec3(0.0f, 500.0f, 1000.0f), 100.0f, true);
    
    // Add Parallax Normal mapping Lights
    RenderLight(pParallaxNormalMappingProgram, m_pCamera);
    
    /// Environment Mapping
    CShaderProgram *pEnvironmentMapProgram = (*m_pShaderPrograms)[9];
    SetCameraUniform(pEnvironmentMapProgram, "camera", m_pCamera);
    SetMaterialUniform(pEnvironmentMapProgram, "material");
    SetEnvironmentMapUniform(pEnvironmentMapProgram, m_useRefraction);
    RenderCube(pEnvironmentMapProgram, glm::vec3(-500.0f, 500.0f, 1000.0f), 100.0f, true);
    RenderMetalBalls(pEnvironmentMapProgram, m_metalballsPosition, 100.0f, true);
    
    
    /// Chromatic Aberration Mapping
    CShaderProgram *pChromaticAberrationProgram = (*m_pShaderPrograms)[10];
    SetCameraUniform(pChromaticAberrationProgram, "camera", m_pCamera);
    SetMaterialUniform(pChromaticAberrationProgram, "material", glm::vec3(0.3f, 0.1f, 0.7f));
    RenderChromaticAberrationCube(pChromaticAberrationProgram, glm::vec3(-1000.0f, 500.0f, 1000.0f), 100.0f, m_woodenBoxesUseTexture);
    
    
    /// Explosion Program
    CShaderProgram *pExplosionProgram = (*m_pShaderPrograms)[11];
    SetMaterialUniform(pExplosionProgram, "material", m_woodenBoxesColor);
    SetExplosionUniform(pExplosionProgram, true, true, glfwGetTime(), m_magnitude);
    RenderNanosuit(pExplosionProgram,  glm::vec3(1000.0f, 500.0f, -1000.0f), 20.0f, m_woodenBoxesUseTexture);
    
    /// Porcupine Rendering Program
    CShaderProgram *pPorcupineRenderingProgram = (*m_pShaderPrograms)[12];
    SetMaterialUniform(pPorcupineRenderingProgram, "material");
    SetPorcupineRenderingUniform(pPorcupineRenderingProgram, glm::vec3(1.0f, 1.0f, 0.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f), m_magnitude);
    RenderNanosuit(pPorcupineRenderingProgram,  glm::vec3(1000.0f, 500.0f, -500.0f), 20.0f, false);
    
    /// Wireframe Rendering Program
    CShaderProgram *pWireframeProgram = (*m_pShaderPrograms)[13];
    SetMaterialUniform(pWireframeProgram, "material", m_woodenBoxesColor);
    SetWireframeUniform(pWireframeProgram, true, m_magnitude);
    SetChromaticAberrationUniform(pWireframeProgram, glm::vec2(0.3f, 1.5f));
    RenderNanosuit(pWireframeProgram,  glm::vec3(1000.0f, 500.0f, 0.0f), 20.0f, m_woodenBoxesUseTexture);
    
    
    /// Toon / Cell Program
    CShaderProgram *pToonProgram = (*m_pShaderPrograms)[14];
    SetCameraUniform(pToonProgram, "camera", m_pCamera);
    SetLightUniform(pToonProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pToonProgram, "material", glm::vec3(0.3f, 0.1f, 0.7f), m_materialShininess);
    RenderTorus(pToonProgram, m_torusPosition, 20.0f, false);
    RenderSphere(pToonProgram, m_spherePosition, 30.0f, false);
    
    // Add Toon Shading Lights
    RenderLight(pToonProgram, m_pCamera);
    
    
    
    
    
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    /// Image Progcessing Shader Program
    CShaderProgram *pImageProcessingProgram = (*m_pShaderPrograms)[15];
    SetMaterialUniform(pImageProcessingProgram, "material", m_woodenBoxesColor);
    SetImageProcessingUniform(pImageProcessingProgram, true);
    RenderQuad(pImageProcessingProgram, glm::vec3(1000, 200, 400), 1.0f, m_woodenBoxesUseTexture);
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
