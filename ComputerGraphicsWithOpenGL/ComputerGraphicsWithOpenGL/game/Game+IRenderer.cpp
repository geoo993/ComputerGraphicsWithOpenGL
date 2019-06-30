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
        
        if (i == m_pointLightPositions.size() - 1) {
            RenderLamp(pLampProgram, m_pointLightPositions[i], 50.0f);
        } else {
            RenderLamp(pLampProgram, m_pointLightPositions[i], 10.0f);
        }
    }
    
    /// Blinn Phong Lighting
    CShaderProgram *pLightProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 5];
    SetCameraUniform(pLightProgram, "camera", m_pCamera);
    SetLightUniform(pLightProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pLightProgram, "material", m_materialColor, m_materialShininess, useAO);
    
    // Render Wooden Boxes
    m_woodenBoxesRotation += 1;
    for ( GLuint i = 0; i < m_woodenBoxesPosition.size(); ++i){
        GLfloat angle = (i % 2 == 0) ? (GLfloat)m_woodenBoxesRotation : (GLfloat)(-m_woodenBoxesRotation);
        //RenderWoodenBox(pLightProgram, m_woodenBoxesPosition[i], 25.0f, angle, -angle, angle);
    }
    RenderLight(pLightProgram, m_pCamera);
    
    //RenderTerrain(pLightProgram, false, m_materialUseTexture);
     
    CShaderProgram *pPBRProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 3];
    
    GLboolean isPBR =
    m_currentPPFXMode == PostProcessingEffectMode::PBR
    || m_currentPPFXMode == PostProcessingEffectMode::IBL;
    
    GLfloat spacing = 100.0f;
    GLfloat shift = 300.0f;
    GLuint numRows = 7;
    GLuint numCol = 7;
    
    if (isPBR) {
        
        if (m_currentPPFXMode == PostProcessingEffectMode::IBL) {
            GLint irradianceTextureUnit = static_cast<GLint>(TextureType::IRRADIANCEMAP);
            m_pIrrSkybox->BindIrrSkyboxTo(irradianceTextureUnit);
            
            GLint cubemapTextureUnit = static_cast<GLint>(TextureType::CUBEMAP);
            m_pIrrSkybox->BindPrefilterSkyboxTo(cubemapTextureUnit);
            
            GLint brdfLUTTextureUnit = static_cast<GLint>(TextureType::SPECULAR);
            m_pIrrSkybox->BindBRDFLUTTextureTo(brdfLUTTextureUnit);
        }
        
        ///  Physically Based Rendering
        SetCameraUniform(pPBRProgram, "camera", m_pCamera);
        SetLightUniform(pPBRProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pPBRProgram, "material", m_materialColor, m_materialShininess, useAO);
        
        for (GLuint row = 0; row < numRows; ++row) {
            GLfloat metallic = (GLfloat)row / (GLfloat)numRows;
            for (GLuint col = 0; col < numCol; ++col) {
                GLfloat roughness = glm::clamp((GLfloat)col / (GLfloat)numCol, 0.05f, 1.0f);
                //glm::vec3 horizontalPositioning = glm::vec3(((GLfloat)row * spacing) - shift, 300.0f, ((GLfloat)col * spacing) - shift);
                glm::vec3 verticalPositioning = glm::vec3((float)(col - (numCol / 2)) * spacing, ((float)(row - (numRows / 2)) * spacing) + shift, shift);
                SetPBRMaterialUniform(pPBRProgram, "material", m_albedo, metallic, roughness, m_ao, m_useIrradiance);
                RenderSphere(pPBRProgram, m_pSphere, verticalPositioning, 30.0f);
            }
        }
        
        SetPBRMaterialUniform(pPBRProgram, "material", m_albedo, m_metallic, m_roughness, m_ao, m_useIrradiance);
        
        // 5 Render Models
        // RenderModel(pPBRProgram, m_pSuitcase, glm::vec3(-400.0f, 200.0f, 0.0f), 3.0f);
        
        // 6 gun
        // RenderModel(pPBRProgram, m_pSuitcase, glm::vec3(1000.0f, 400.0f, 0.0f), 5.0f);
        
        
        // Render Lights
        RenderLight(pPBRProgram, m_pCamera);
        
    } else {
        for (GLuint row = 0; row < numRows; ++row) {
            for (GLuint col = 0; col < numCol; ++col) {
                //glm::vec3 horizontalPositioning = glm::vec3(((GLfloat)row * spacing) - shift, 300.0f, ((GLfloat)col * spacing) - shift);
                glm::vec3 verticalPositioning = glm::vec3((float)(col - (numCol / 2)) * spacing, ((float)(row - (numRows / 2)) * spacing) + shift, shift);
                RenderSphere(pLightProgram, m_pSphere, verticalPositioning, 30.0f);
            }
        }
    }
    
    CShaderProgram *pShaderProgram = isPBR ? pPBRProgram : pLightProgram;
    
    SetPBRMaterialUniform(pShaderProgram, "material", m_albedo, m_metallic, m_roughness, m_ao, m_useIrradiance);
    
    //   bUseAlbedo, bUseMetallic, bUseRoughness, bUseAO;
    RenderSphere(pShaderProgram, m_pSpherePBR1, glm::vec3(50.0f, 500.0f, 0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR2, glm::vec3(-50.0f, 500.0f, 0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR3, glm::vec3(150.0f, 500.0f, 0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR4, glm::vec3(-150.0f, 500.0f, 0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR5, glm::vec3(250.0f, 500.0f, 0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR6, glm::vec3(-250.0f, 500.0f, 0.0f), 30.0f);
  
    // Parallax Mapping
    
    
    /// Environment Mapping
    CShaderProgram *pEnvironmentMapProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 9];
    SetCameraUniform(pEnvironmentMapProgram, "camera", m_pCamera);
    SetMaterialUniform(pEnvironmentMapProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f));
    SetHRDLightUniform(pEnvironmentMapProgram, "R_hrdlight", m_exposure, m_gama, m_HDR);
    
    SetEnvironmentMapUniform(pEnvironmentMapProgram, m_useRefraction);
    
    // 8 sphere
    RenderSphere(pEnvironmentMapProgram, m_pSphere, glm::vec3(-350.0f, 500.0f, 0.0f), 30.0f);
    
    RenderMetalBalls(isPBR ? pPBRProgram : pEnvironmentMapProgram, glm::vec3(400.0f, 500.0f, 0.0f), 100.0f);
    
    
    /// Bump Mapping
    CShaderProgram *pBumpMappingProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 7];
    SetCameraUniform(pBumpMappingProgram, "camera", m_pCamera);
    SetLightUniform(pBumpMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pBumpMappingProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    SetHRDLightUniform(pBumpMappingProgram, "R_hrdlight", m_exposure, m_gama, m_HDR);
    SetBumpMapUniform(pBumpMappingProgram, m_uvTiling);
    
    RenderTorusKnot(pBumpMappingProgram, glm::vec3(600.0f, 500.0f, 0.0f), 2.0f);
    //RenderTorus(pToonProgram, m_torusPosition, 20.0f, false);
    
    // Add Bump mapping Lights
    RenderLight(pBumpMappingProgram, m_pCamera);
    
    
    /// Chromatic Aberration Mapping
    CShaderProgram *pChromaticAberrationProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 10];
    SetCameraUniform(pChromaticAberrationProgram, "camera", m_pCamera);
    SetMaterialUniform(pChromaticAberrationProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f));
    SetHRDLightUniform(pChromaticAberrationProgram, "R_hrdlight", m_exposure, m_gama, m_HDR);
    
    // 9 sphere
    m_textures[8]->BindTexture2DToTextureType();
    m_textures[9]->BindTexture2DToTextureType();
    RenderSphere(pChromaticAberrationProgram, m_pSphere, glm::vec3(-450.0f, 500.0f, 0.0f), 30.0f, false);

    /// 10 Toon / Cell Program
    CShaderProgram *pToonProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 14];
    SetCameraUniform(pToonProgram, "camera", m_pCamera);
    SetLightUniform(pToonProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
    SetMaterialUniform(pToonProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    SetHRDLightUniform(pChromaticAberrationProgram, "R_hrdlight", m_exposure, m_gama, m_HDR);
    RenderSphere(pToonProgram, m_pSpherePBR1, glm::vec3(-550.0f, 500.0f, 0.0f), 30.0f);
    
    // Add Toon Shading Lights
    RenderLight(pToonProgram, m_pCamera);
    
    /// 11 FireBall Program
    CShaderProgram *pFireBallProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 54];
    SetMaterialUniform(pFireBallProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
    SetFireBallUniform(pFireBallProgram);
    RenderSphere(pFireBallProgram, m_pFireBallSphere, glm::vec3(-650.0f, 500.0f, 0.0f), 30.0f);
    
    // reset cubmap
    GLint cubemapTextureUnit = static_cast<GLint>(TextureType::CUBEMAP);
    m_pEnvSkybox->BindEnvSkyboxTo(cubemapTextureUnit);
    
    /// Render skybox
    if (m_currentPPFXMode == PostProcessingEffectMode::IBL) {
        CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 1];
        SetMaterialUniform(pSkyBoxProgram, "material");
        SetHRDLightUniform(pSkyBoxProgram, "R_hrdlight", m_exposure, m_gama, m_HDR);
        RenderEnvSkyBox(pSkyBoxProgram);
    } else {
        CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 1];
        SetMaterialUniform(pSkyBoxProgram, "material");
        RenderSkyBox(pSkyBoxProgram);
    }
}
