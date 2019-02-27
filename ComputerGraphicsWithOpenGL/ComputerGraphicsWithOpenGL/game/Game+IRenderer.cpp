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
    
    /// Render skybox
    CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 1];
    SetMaterialUniform(pSkyBoxProgram, "material");
    RenderSkyBox(pSkyBoxProgram);
/*
    /// Render terrain
    CShaderProgram *pTerrainProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 2];
    SetMaterialUniform(pTerrainProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    RenderTerrain(pTerrainProgram, true, true);
    */
    
    /*
    ///  Physically Based Rendering
    CShaderProgram *pPBRProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 3];
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
            SetMaterialUniform(pPBRProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
            RenderSphere(pPBRProgram, position, 30.0f, m_woodenBoxesUseTexture);
        }
    }
    //RenderGrenade(pNormalMappingProgram,  glm::vec3(600.0f, 200.0f, -500.0f), 20.0f, true);
    
    // Add Physically Based Rendering Lights
    RenderLight(pPBRProgram, m_pCamera);
    */

    /// Render Lamps
    CShaderProgram *pLampProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 4];
    for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
        SetMaterialUniform(pLampProgram, "material", m_pointLightColors[i], m_materialShininess, useAO);
        RenderLamp(pLampProgram, m_pointLightPositions[i], 10.0f);
    }

    /// Render Lights
    CShaderProgram *pLightProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 5];
    SetCameraUniform(pLightProgram, "camera", m_pCamera);
    SetLightUniform(pLightProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pLightProgram, "material", m_woodenBoxesColor, m_materialShininess, useAO);
    
    
    // Render Wooden Boxes
    for ( GLuint i = 0; i < m_woodenBoxesPosition.size(); ++i){
        GLfloat angle = 20.0f * (GLfloat)i;
        RenderWoodenBox(pLightProgram, m_woodenBoxesPosition[i], 25.0f, angle, m_woodenBoxesUseTexture);
    }
    
    GLfloat objGap = 100.0f;
    GLuint objRows = 1;
    GLuint objCols = 1;
    for (GLuint row = 0; row < objRows; row++) {
        for (GLuint col = 0; col < objCols; col++) {
            glm::vec3 position = glm::vec3(((GLfloat)row * objGap) - 100.0f, 60.0f, ((GLfloat)col * objGap) -100.0f);
            RenderNanosuit(pLightProgram, position, glm::vec3(0.0f, 0.0f, 0.0f), 15.0f, m_woodenBoxesUseTexture);
        }
    }
    
    // RenderTerrain
    RenderTerrain(pLightProgram, false, true);
    
    // Render Big cube underneath
    RenderInteriorBox(pLightProgram, glm::vec3(0.0f,  550.0f,  0.0f ), 50.0f, true, true);
    
    // Add Default Lights
    RenderLight(pLightProgram, m_pCamera);
    

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
