//
//  Game+IRenderer.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderScene(const GLboolean &toLightSpace){
    const GLint lightSpaceIndex = 51;
    const GLboolean useAO = m_currentPPFXMode == PostProcessingEffectMode::SSAO;
    
    // enable depth testing (is disabled for rendering screen-space quad post processing)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    
    // comment out if stencil buffer is not used
    // glStencilMask(0xFF); // each bit is written to the stencil buffer as is
    // glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
    // Most of the cases you'll just be writing 0x00 or 0xFF as the stencil mask, but it's good to know there are options to set custom bit-masks.
    glStencilMask(0x00);
    
    /// Render Lamps
    CShaderProgram *pLampProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 4];
    for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
        SetMaterialUniform(pLampProgram, "material", m_pointLightColors[i], m_materialShininess, useAO);
        RenderLamp(pLampProgram, m_pointLightPositions[i], 10.0f);
    }

    /// Blinn Phong Lighting
    CShaderProgram *pLightProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 5];
    
    ///  Physically Based Rendering
    CShaderProgram *pPBRProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 3];
    
    GLboolean isPBR = m_currentPPFXMode == PostProcessingEffectMode::PBR || m_currentPPFXMode == PostProcessingEffectMode::IBL;
    CShaderProgram *mainProgram = isPBR ? pPBRProgram : pLightProgram;
    SetCameraUniform(mainProgram, "camera", m_pCamera);
    SetLightUniform(mainProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(mainProgram, "material", m_woodenBoxesColor, m_materialShininess, useAO);
    
    /*
    // Render Wooden Boxes
    for ( GLuint i = 0; i < m_woodenBoxesPosition.size(); ++i){
        GLfloat angle = 20.0f * (GLfloat)i;
        RenderWoodenBox(mainProgram, m_woodenBoxesPosition[i], 25.0f, angle, m_woodenBoxesUseTexture);
    }
    */
    
    GLfloat gap = 100.0f;
    GLuint numRows = 5;
    GLuint numCol = 5;
    if (isPBR) {
        glm::vec3 albedo = glm::vec3(m_albedo, 0.0f, 0.0f);
        for (GLuint row = 0; row < numRows; row++) {
            GLfloat metallic = (GLfloat)row / (GLfloat)numRows;
            for (GLuint col = 0; col < numCol; col++) {
                GLfloat roughness = glm::clamp((GLfloat)col / (GLfloat)numCol, 0.05f, 1.0f);
                glm::vec3 position = glm::vec3(((GLfloat)row * gap) - 100.0f, 300.0f, ((GLfloat)col * gap) - 100.0f);
                SetPBRMaterialUniform(mainProgram, "material", albedo, metallic, roughness, m_fresnel, m_ao);
                RenderSphere(mainProgram, position, 30.0f, m_woodenBoxesUseTexture);
            }
        }
        
        SetPBRMaterialUniform(mainProgram, "material", albedo, m_metallic, m_roughness, m_fresnel, m_ao);
    } else {
        for (GLuint row = 0; row < numRows; row++) {
            for (GLuint col = 0; col < numCol; col++) {
                glm::vec3 position = glm::vec3(((GLfloat)row * gap) - 100.0f, 300.0f, ((GLfloat)col * gap) - 100.0f);
                RenderSphere(mainProgram, position, 30.0f, m_woodenBoxesUseTexture);
            }
        }
    }
    
    //RenderGrenade(pNormalMappingProgram,  glm::vec3(600.0f, 200.0f, -500.0f), 20.0f, true);
    
    
    // Render Big cube underneath
    //RenderInteriorBox(mainProgram, glm::vec3(0.0f,  550.0f,  0.0f ), 50.0f, m_woodenBoxesUseTexture, true);
    
    // RenderTerrain
    //RenderTerrain(mainProgram, false, m_woodenBoxesUseTexture);
    
    // Render Lights
    RenderLight(mainProgram, m_pCamera);

    /*
    /// Normal Mapping
    CShaderProgram *pNormalMappingProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 6];
    SetCameraUniform(pNormalMappingProgram, "camera", m_pCamera);
    SetLightUniform(pNormalMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pNormalMappingProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    RenderCube(pNormalMappingProgram, glm::vec3(1000.0f, 500.0f, 1000.0f), 100.0f, true);
    
    // Add Normal mapping Lights
    RenderLight(pNormalMappingProgram, m_pCamera);
    
    /// Bump Mapping
    CShaderProgram *pBumpMappingProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 7];
    SetCameraUniform(pBumpMappingProgram, "camera", m_pCamera);
    SetLightUniform(pBumpMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pBumpMappingProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    SetBumpMapUniform(pBumpMappingProgram, m_uvTiling);
    RenderCube(pBumpMappingProgram, glm::vec3(500.0f, 500.0f, 1000.0f), 100.0f, true);
    RenderTorusKnot(pBumpMappingProgram, m_torusKnotPosition, 5.0f, true);
    
    // Add Bump mapping Lights
    RenderLight(pBumpMappingProgram, m_pCamera);
    
    /// Parallax Normal Mapping
    CShaderProgram *pParallaxNormalMappingProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 8];
    SetCameraUniform(pParallaxNormalMappingProgram, "camera", m_pCamera);
    SetLightUniform(pParallaxNormalMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pParallaxNormalMappingProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    SetParallaxMapUniform(pParallaxNormalMappingProgram, m_parallaxHeightScale);
    RenderParallaxCube(pParallaxNormalMappingProgram, glm::vec3(0.0f, 500.0f, 1000.0f), 100.0f, true);
    
    // Add Parallax Normal mapping Lights
    RenderLight(pParallaxNormalMappingProgram, m_pCamera);
    */
    /// Environment Mapping
    CShaderProgram *pEnvironmentMapProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 9];
    SetCameraUniform(pEnvironmentMapProgram, "camera", m_pCamera);
    SetMaterialUniform(pEnvironmentMapProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f));
    SetEnvironmentMapUniform(pEnvironmentMapProgram, m_useRefraction);
    RenderCube(pEnvironmentMapProgram, glm::vec3(-500.0f, 500.0f, 1000.0f), 100.0f, true);
    //RenderMetalBalls(pEnvironmentMapProgram, m_metalballsPosition, 100.0f, true);
    
    
    /// Chromatic Aberration Mapping
    CShaderProgram *pChromaticAberrationProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 10];
    SetCameraUniform(pChromaticAberrationProgram, "camera", m_pCamera);
    SetMaterialUniform(pChromaticAberrationProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f));
    RenderChromaticAberrationCube(pChromaticAberrationProgram, glm::vec3(-1000.0f, 500.0f, 1000.0f), 100.0f, m_woodenBoxesUseTexture);
    
    /*
    /// Equirectangular Cube Mapping
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    CShaderProgram *pEquirectangularCubeProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 77];
    SetMaterialUniform(pEquirectangularCubeProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f));
    RenderEquirectangularCube(pEquirectangularCubeProgram, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, true);
    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    */
    
    /// Render skybox
    CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 1];
    SetMaterialUniform(pSkyBoxProgram, "material");
    RenderSkyBox(pSkyBoxProgram);
    
    
    /*
    /// Explosion Program
    CShaderProgram *pExplosionProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 11];
    SetMaterialUniform(pExplosionProgram, "material", m_woodenBoxesColor, m_materialShininess, useAO);
    SetExplosionUniform(pExplosionProgram, true, true, glfwGetTime(), m_magnitude);
    RenderNanosuit(pExplosionProgram,  glm::vec3(1000.0f, 500.0f, -1000.0f), 20.0f, m_woodenBoxesUseTexture);
    
    /// Porcupine Rendering Program
    CShaderProgram *pPorcupineRenderingProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 12];
    SetMaterialUniform(pPorcupineRenderingProgram, "material");
    SetPorcupineRenderingUniform(pPorcupineRenderingProgram, glm::vec3(1.0f, 1.0f, 0.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f), m_magnitude);
    RenderNanosuit(pPorcupineRenderingProgram,  glm::vec3(1000.0f, 500.0f, -500.0f), 20.0f, false);
    
    /// Wireframe Rendering Program
    CShaderProgram *pWireframeProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 13];
    SetMaterialUniform(pWireframeProgram, "material", m_woodenBoxesColor);
    SetWireframeUniform(pWireframeProgram, true, m_magnitude);
    SetChromaticAberrationUniform(pWireframeProgram, glm::vec2(0.3f, 1.5f));
    RenderNanosuit(pWireframeProgram,  glm::vec3(1000.0f, 500.0f, 0.0f), 20.0f, m_woodenBoxesUseTexture);
    */
    /*
    /// Toon / Cell Program
    CShaderProgram *pToonProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 14];
    SetCameraUniform(pToonProgram, "camera", m_pCamera);
    SetLightUniform(pToonProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pToonProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    RenderTorus(pToonProgram, m_torusPosition, 20.0f, false);
    RenderSphere(pToonProgram, m_spherePosition, 30.0f, false);
    
    // Add Toon Shading Lights
    RenderLight(pToonProgram, m_pCamera);
    
    /// FireBall Program
    CShaderProgram *pFireBallProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 54];
    SetMaterialUniform(pFireBallProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    SetFireBallUniform(pFireBallProgram);
    RenderFireBallSphere(pFireBallProgram, glm::vec3(-500.0f, 470.0f, -1000.0f), 50.0f);
     */
    
}
