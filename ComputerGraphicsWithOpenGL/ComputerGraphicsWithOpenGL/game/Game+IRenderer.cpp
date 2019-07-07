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
    const GLfloat zfront = -200.0f;
    const GLfloat zback = 300.0f;
    
    CShaderProgram *pShaderProgram;
    
    GLboolean isPBR =
    m_currentPPFXMode == PostProcessingEffectMode::PBR
    || m_currentPPFXMode == PostProcessingEffectMode::IBL;

    if (isPBR) {
        ///  Physically Based Rendering
        pShaderProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 3];
        SetCameraUniform(pShaderProgram, "camera", m_pCamera);
        SetLightUniform(pShaderProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pShaderProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetPBRMaterialUniform(pShaderProgram, "material", m_albedo, m_metallic, m_roughness, m_ao, m_useIrradiance);
        SetFogMaterialUniform(pShaderProgram, "fog", m_fogColor, m_useFog);
        
        if (m_currentPPFXMode == PostProcessingEffectMode::IBL) {
            GLint irradianceTextureUnit = static_cast<GLint>(TextureType::IRRADIANCEMAP);
            m_pIrrSkybox->BindIrrSkyboxTo(irradianceTextureUnit);
            
            GLint cubemapTextureUnit = static_cast<GLint>(TextureType::CUBEMAP);
            m_pIrrSkybox->BindPrefilterSkyboxTo(cubemapTextureUnit);
            
            GLint brdfLUTTextureUnit = static_cast<GLint>(TextureType::SPECULAR);
            m_pIrrSkybox->BindBRDFLUTTextureTo(brdfLUTTextureUnit);
        }
        // Render Lights
        RenderLight(pShaderProgram, m_pCamera);
        
    } else {
        
        /// Blinn Phong Lighting
        pShaderProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 5];
        SetCameraUniform(pShaderProgram, "camera", m_pCamera);
        SetLightUniform(pShaderProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pShaderProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetPBRMaterialUniform(pShaderProgram, "material", m_albedo, m_metallic, m_roughness, m_ao, m_useIrradiance);
        SetFogMaterialUniform(pShaderProgram, "fog", m_fogColor, m_useFog);
        // Render Lights
        RenderLight(pShaderProgram, m_pCamera);
        
        //RenderTerrain(pShaderProgram, false, m_materialUseTexture);
    }
    
    // 1 - 10
    RenderSphere(pShaderProgram, m_pSpherePBR1, glm::vec3(50.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube1, glm::vec3(50.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR2, glm::vec3(-50.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube2, glm::vec3(-50.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR3, glm::vec3(150.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube3, glm::vec3(150.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR4, glm::vec3(-150.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube4, glm::vec3(-150.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR5, glm::vec3(250.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube5, glm::vec3(250.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR6, glm::vec3(-250.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube6, glm::vec3(-250.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR7, glm::vec3(350.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube7, glm::vec3(350.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR8, glm::vec3(-350.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube8, glm::vec3(-350.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR9, glm::vec3(450.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube9, glm::vec3(450.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    RenderSphere(pShaderProgram, m_pSpherePBR10, glm::vec3(-450.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
    RenderCube(pShaderProgram, m_pCube10, glm::vec3(-450.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    
    /// Bump Mapping
    {
        CShaderProgram *pBumpMappingProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 7];
        SetCameraUniform(pBumpMappingProgram, "camera", m_pCamera);
        SetLightUniform(pBumpMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pBumpMappingProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetFogMaterialUniform(pBumpMappingProgram, "fog", m_fogColor, m_useFog);
        SetBumpMapUniform(pBumpMappingProgram, m_uvTiling);
        // Add Bump mapping Lights
        RenderLight(pBumpMappingProgram, m_pCamera);
        
        // 11
        RenderSphere(pBumpMappingProgram, m_pSpherePBR11, glm::vec3(550.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
        RenderTorus(pBumpMappingProgram, glm::vec3(550.0f, 0.0f, zback), 10.0f);
        
        //RenderTorusKnot(pBumpMappingProgram, glm::vec3(600.0f, 500.0f, 0.0f), 2.0f);
    }
    
    /// Environment Mapping
    {
        CShaderProgram *pEnvironmentMapProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 9];
        SetCameraUniform(pEnvironmentMapProgram, "camera", m_pCamera);
        SetMaterialUniform(pEnvironmentMapProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetHRDLightUniform(pEnvironmentMapProgram, "hrdlight", m_exposure, m_gama, m_HDR);
        SetEnvironmentMapUniform(pEnvironmentMapProgram, m_useRefraction);
        SetFogMaterialUniform(pEnvironmentMapProgram, "fog", m_fogColor, m_useFog);
        
        // 12
        RenderSphere(pEnvironmentMapProgram, m_pSpherePBR12, glm::vec3(-550.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f, false);
        RenderCube(pEnvironmentMapProgram, m_pCube12, glm::vec3(-550.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f, false);
        //RenderMetalBalls(isPBR ? pShaderProgram : pEnvironmentMapProgram, glm::vec3(-500.0f, 0.0f, zback), 100.0f, isPBR ? m_materialUseTexture : false);
    }

    /// Parallax Normal Mapping
    {
        CShaderProgram *pParallaxNormalMappingProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 8];
        SetCameraUniform(pParallaxNormalMappingProgram, "camera", m_pCamera);
        SetLightUniform(pParallaxNormalMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pParallaxNormalMappingProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetParallaxMapUniform(pParallaxNormalMappingProgram, m_parallaxHeightScale);
        SetFogMaterialUniform(pParallaxNormalMappingProgram, "fog", m_fogColor, m_useFog);
        // Add Parallax Normal mapping Lights
        RenderLight(pParallaxNormalMappingProgram, m_pCamera);
        
        // 13
        RenderSphere(pParallaxNormalMappingProgram, m_pSpherePBR13, glm::vec3(650.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
        RenderCube(pParallaxNormalMappingProgram, m_pCube13, glm::vec3(650.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    }
    

    /// Chromatic Aberration Mapping
    {
        CShaderProgram *pChromaticAberrationProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 10];
        SetCameraUniform(pChromaticAberrationProgram, "camera", m_pCamera);
        SetMaterialUniform(pChromaticAberrationProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetFogMaterialUniform(pChromaticAberrationProgram, "fog", m_fogColor, m_useFog);
        SetHRDLightUniform(pChromaticAberrationProgram, "hrdlight", m_exposure, m_gama, m_HDR);
        SetChromaticAberrationUniform(pChromaticAberrationProgram, glm::vec2(0.3f, 1.5f));
        
        // 14
        RenderSphere(pChromaticAberrationProgram, m_pSpherePBR14, glm::vec3(-650.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
        RenderCube(pChromaticAberrationProgram, m_pCube14, glm::vec3(-650.0f, 0.0f, zback), glm::vec3(0.0f), 30.0f);
    }
    
    /// Discard
    {
        CShaderProgram *pDiscardProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 81];
        glEnable (GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        SetCameraUniform(pDiscardProgram, "camera", m_pCamera);
        SetLightUniform(pDiscardProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pDiscardProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetFogMaterialUniform(pDiscardProgram, "fog", m_fogColor, m_useFog);
        SetDisintegrationUniform(pDiscardProgram);
        RenderLight(pDiscardProgram, m_pCamera);
        
        // 9 sphere
        m_sphereRotation += m_deltaTime * 0.05f;
        //float scale = sin(m_woodenBoxesRotation / 100.0f);
        RenderSphere(pDiscardProgram, m_pSpherePBR15, glm::vec3(750.0f, 0.0f, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), 30.0f);
        RenderModel(pDiscardProgram, m_teapot2, glm::vec3(750.0f, 0.0f, zback), 2.0f);
        
        // Add Discard Lights
        glDisable (GL_BLEND);
    }
    
    /// Toon / Cell Program
    {
        CShaderProgram *pToonProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 14];
        SetCameraUniform(pToonProgram, "camera", m_pCamera);
        SetLightUniform(pToonProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pToonProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetFogMaterialUniform(pToonProgram, "fog", m_fogColor, m_useFog);
        // Add Toon Shading Lights
        RenderLight(pToonProgram, m_pCamera);
        
        // 11 sphere
        RenderSphere(pToonProgram, m_pSpherePBR16, glm::vec3(-750.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
        RenderModel(pToonProgram, m_teapot1, glm::vec3(-750.0f, 0.0f, zback), 2.0f);
    }
    
    /// Porcupine Rendering
    {
        CShaderProgram *pPorcupineRenderingProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 12];
        SetMaterialUniform(pPorcupineRenderingProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetPorcupineRenderingUniform(pPorcupineRenderingProgram, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), m_magnitude);
        
        RenderSphere(pPorcupineRenderingProgram, m_pSpherePBR17, glm::vec3(850.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f, false);
        RenderModel(pPorcupineRenderingProgram, m_teapot1, glm::vec3(850.0f, 0.0f, zback), 2.0f);
        
    }
    
     /// FireBall
     {
         CShaderProgram *pFireBallProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 54];
         SetMaterialUniform(pFireBallProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, useAO);
         SetFireBallUniform(pFireBallProgram);
         
         // 12 sphere
         RenderSphere(pFireBallProgram, m_pSpherePBR18, glm::vec3(-850.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
     }
    
    
    /// Wireframe Rendering
    {
        CShaderProgram *pWireframeProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 13];
        SetCameraUniform(pWireframeProgram, "camera", m_pCamera);
        SetLightUniform(pWireframeProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pWireframeProgram, "material", m_materialColor, m_materialShininess, useAO);
        SetFogMaterialUniform(pWireframeProgram, "fog", m_fogColor, m_useFog);
        SetWireframeUniform(pWireframeProgram, true, 0.15f);
        // Add Wireframe Lights
        RenderLight(pWireframeProgram, m_pCamera);
        
        RenderSphere(pWireframeProgram, m_pSpherePBR19, glm::vec3(950.0f, 0.0f, zfront), glm::vec3(0.0f), 30.0f);
        RenderModel(pWireframeProgram, m_teapot1, glm::vec3(950.0f, 0.0f, zback), 2.0f);
        
    }
    
    /// Skybox
    {
        // reset cubmap
        GLint cubemapTextureUnit = static_cast<GLint>(TextureType::CUBEMAP);
        m_pEnvSkybox->BindEnvSkyboxTo(cubemapTextureUnit);
        
        CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 1];
        SetMaterialUniform(pSkyBoxProgram, "material");
        SetFogMaterialUniform(pSkyBoxProgram, "fog", m_fogColor, m_useFog);
        
        if (m_currentPPFXMode == PostProcessingEffectMode::IBL) {
            SetHRDLightUniform(pSkyBoxProgram, "hrdlight", m_exposure, m_gama, m_HDR);
            RenderEnvSkyBox(pSkyBoxProgram);
        } else {
            RenderSkyBox(pSkyBoxProgram);
        }
    }
    
    /// Render Lamps
    CShaderProgram *pLampProgram = (*m_pShaderPrograms)[toLightSpace ? lightSpaceIndex : 4];
    for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
        SetMaterialUniform(pLampProgram, "material", m_pointLightColors[i], m_materialShininess, useAO);
        RenderLamp(pLampProgram, m_pointLightPositions[i], 10.0f);
    }
    
}
