//
//  Game+IRenderer.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderPBRScene(CShaderProgram *pShaderProgram, const GLboolean &toCustomShader, const GLint &toCustomShaderIndex, const GLfloat zfront, const GLfloat zback) {
    GLfloat yPos = m_currentPPFXMode == PostProcessingEffectMode::SSAO ? (m_useTerrain ? -100.0f : -800.0f) : 0.0f;
    
    // 1 - 10
    RenderPrimitive(pShaderProgram, m_pSpherePBR1, glm::vec3(50.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot1, glm::vec3(50.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR2, glm::vec3(-50.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot2, glm::vec3(-50.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR3, glm::vec3(150.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot3, glm::vec3(150.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR4, glm::vec3(-150.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot4, glm::vec3(-150.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR5, glm::vec3(250.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot5, glm::vec3(250.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR6, glm::vec3(-250.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot6, glm::vec3(-250.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR7, glm::vec3(350.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot7, glm::vec3(350.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR8, glm::vec3(-350.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot8, glm::vec3(-350.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR9, glm::vec3(450.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot9, glm::vec3(450.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
    RenderPrimitive(pShaderProgram, m_pSpherePBR10, glm::vec3(-450.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    RenderModel(pShaderProgram, m_teapot10, glm::vec3(-450.0f, yPos+10, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    
}


void Game::RenderRandomScene(CShaderProgram *pShaderProgram, const GLboolean &toCustomShader, const GLint &toCustomShaderIndex, const GLfloat zfront, const GLfloat zback)  {
    GLfloat yPos = m_currentPPFXMode == PostProcessingEffectMode::SSAO ? (m_useTerrain ? -100.0f : -800.0f) : 0.0f;
    const GLboolean useAO = m_currentPPFXMode == PostProcessingEffectMode::SSAO;
    GLboolean isPBR =
    m_currentPPFXMode == PostProcessingEffectMode::PBR
    || m_currentPPFXMode == PostProcessingEffectMode::IBL;
    
    /// Bump Mapping
    {
        CShaderProgram *pBumpMappingProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 7];
        SetCameraUniform(pBumpMappingProgram, "camera", m_pCamera);
        SetLightUniform(pBumpMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pBumpMappingProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetFogMaterialUniform(pBumpMappingProgram, "fog", m_fogColor, m_useFog);
        SetHRDLightUniform(pBumpMappingProgram, m_hdrName, m_exposure, m_gama, m_HDR);
        RenderLight(pBumpMappingProgram, m_dirName, m_pointName, m_spotName, m_pCamera);
        
        // 11
        RenderPrimitive(pBumpMappingProgram, m_pSpherePBR11, glm::vec3(50.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
        //RenderPrimitive(pBumpMappingProgram, m_pTorus, glm::vec3(50.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(10.0f));
        RenderPrimitive(pBumpMappingProgram, m_pTorusKnot, glm::vec3(50.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    }
    
    
    /// Environment Mapping
    {
        CShaderProgram *pEnvironmentMapProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 9];
        SetCameraUniform(pEnvironmentMapProgram, "camera", m_pCamera);
        SetMaterialUniform(pEnvironmentMapProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetHRDLightUniform(pEnvironmentMapProgram, "hrdlight", m_exposure, m_gama, m_HDR);
        SetEnvironmentMapUniform(pEnvironmentMapProgram, m_useRefraction);
        SetFogMaterialUniform(pEnvironmentMapProgram, "fog", m_fogColor, m_useFog);
        
        // 12
        RenderPrimitive(pEnvironmentMapProgram, m_pSpherePBR12, glm::vec3(-50.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f), false);
        RenderPrimitive(pEnvironmentMapProgram, m_pCube12, glm::vec3(-50.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(30.0f), false);
    }
    
    
    /// Parallax Normal Mapping
    {
        CShaderProgram *pParallaxNormalMappingProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 8];
        SetCameraUniform(pParallaxNormalMappingProgram, "camera", m_pCamera);
        SetLightUniform(pParallaxNormalMappingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pParallaxNormalMappingProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetParallaxMapUniform(pParallaxNormalMappingProgram, m_parallaxHeightScale);
        SetFogMaterialUniform(pParallaxNormalMappingProgram, "fog", m_fogColor, m_useFog);
        SetHRDLightUniform(pParallaxNormalMappingProgram, m_hdrName, m_exposure, m_gama, m_HDR);
        RenderLight(pParallaxNormalMappingProgram, m_dirName, m_pointName, m_spotName, m_pCamera);
        
        // 13
        RenderPrimitive(pParallaxNormalMappingProgram, m_pSpherePBR13, glm::vec3(150.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
        RenderPrimitive(pParallaxNormalMappingProgram, m_pCube13, glm::vec3(150.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(30.0f));
    }
    
    /// Chromatic Aberration Mapping
    {
        CShaderProgram *pChromaticAberrationProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 10];
        SetCameraUniform(pChromaticAberrationProgram, "camera", m_pCamera);
        SetMaterialUniform(pChromaticAberrationProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetFogMaterialUniform(pChromaticAberrationProgram, "fog", m_fogColor, m_useFog);
        SetHRDLightUniform(pChromaticAberrationProgram, "hrdlight", m_exposure, m_gama, m_HDR);
        SetChromaticAberrationUniform(pChromaticAberrationProgram, glm::vec2(0.3f, 1.5f));
        
        // 14
        RenderPrimitive(pChromaticAberrationProgram, m_pSpherePBR14, glm::vec3(-150.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
        RenderPrimitive(pChromaticAberrationProgram, m_pCube14, glm::vec3(-150.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(30.0f));
    }
    
    /// Discard
    {
        CShaderProgram *pDiscardProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 81];
        glEnable (GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        SetCameraUniform(pDiscardProgram, "camera", m_pCamera);
        SetLightUniform(pDiscardProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pDiscardProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetDisintegrationUniform(pDiscardProgram);
        SetFogMaterialUniform(pDiscardProgram, "fog", m_fogColor, m_useFog);
        SetHRDLightUniform(pDiscardProgram, m_hdrName, m_exposure, m_gama, m_HDR);
        RenderLight(pDiscardProgram, m_dirName, m_pointName, m_spotName, m_pCamera);

        // 15
        RenderPrimitive(pDiscardProgram, m_pSpherePBR15, glm::vec3(250.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
        RenderModel(pDiscardProgram, m_teapot15, glm::vec3(250.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(1.0f));

        // Add Discard Lights
        glDisable (GL_BLEND);
    }
    
    /// Toon / Cell Program
    {
        CShaderProgram *pToonProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 14];
        SetCameraUniform(pToonProgram, "camera", m_pCamera);
        SetMaterialUniform(pToonProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetFogMaterialUniform(pToonProgram, "fog", m_fogColor, m_useFog);
        SetLightUniform(pToonProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetHRDLightUniform(pToonProgram, m_hdrName, m_exposure, m_gama, m_HDR);
        RenderLight(pToonProgram, m_dirName, m_pointName, m_spotName, m_pCamera);
        
        // 16
        RenderPrimitive(pToonProgram, m_pSpherePBR16, glm::vec3(-250.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
        RenderModel(pToonProgram, m_teapot16, glm::vec3(-250.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    }
    
    
    /// Porcupine Rendering
    {
        CShaderProgram *pPorcupineRenderingProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 12];
        SetMaterialUniform(pPorcupineRenderingProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetPorcupineRenderingUniform(pPorcupineRenderingProgram, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), m_magnitude);
        // 17
        RenderPrimitive(pPorcupineRenderingProgram, m_pSpherePBR17, glm::vec3(350.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f), false);
        RenderModel(pPorcupineRenderingProgram, m_teapot17, glm::vec3(350.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(1.0f));

    }
    
    /// FireBall
    {
        CShaderProgram *pFireBallProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 54];
        SetMaterialUniform(pFireBallProgram, "material", glm::vec4(0.3f, 0.1f, 0.7f, 1.0f), m_materialShininess, m_uvTiling, useAO);
        SetFireBallUniform(pFireBallProgram);
        
        // 18
        RenderPrimitive(pFireBallProgram, m_pSpherePBR18, glm::vec3(-380.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
    }
    
    /// MetalBalls
    {
        CShaderProgram *pEnvironmentMapProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 9];
        SetMaterialUniform(isPBR ? pShaderProgram : pEnvironmentMapProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        RenderMetalBalls(isPBR ? pShaderProgram : pEnvironmentMapProgram, glm::vec3(-380.0f, 0.0f, zback), glm::vec3(100.0f), isPBR ? m_materialUseTexture : false);
    }
    
    /// Wireframe Rendering
    {
        CShaderProgram *pWireframeProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 13];
        SetCameraUniform(pWireframeProgram, "camera", m_pCamera);
        SetMaterialUniform(pWireframeProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetFogMaterialUniform(pWireframeProgram, "fog", m_fogColor, m_useFog);
        SetWireframeUniform(pWireframeProgram, true, 0.15f);
        SetLightUniform(pWireframeProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetHRDLightUniform(pWireframeProgram, m_hdrName, m_exposure, m_gama, m_HDR);
        RenderLight(pWireframeProgram, m_dirName, m_pointName, m_spotName, m_pCamera);
        
        // 19
        RenderPrimitive(pWireframeProgram, m_pSpherePBR19, glm::vec3(450.0f, yPos+30, zfront), glm::vec3(0.0f, m_sphereRotation, 0.0f), glm::vec3(30.0f));
        RenderModel(pWireframeProgram, m_teapot19, glm::vec3(450.0f, 0.0f, zback), glm::vec3(0.0f), glm::vec3(1.0f));
    }
}


void Game::RenderTerrainScene(CShaderProgram *pShaderProgram, const GLfloat yPos) {
    const GLboolean useAO = m_currentPPFXMode == PostProcessingEffectMode::SSAO;
    GLboolean isPBR =
    m_currentPPFXMode == PostProcessingEffectMode::PBR
    || m_currentPPFXMode == PostProcessingEffectMode::IBL;
    
    if (m_showTerrain) {

        if (m_useTerrain)
        {
            /// Terrain
            
            pShaderProgram->UseProgram();
            if (isPBR) {
                SetMaterialUniform(pShaderProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
            }
            
            RenderTerrain(pShaderProgram, glm::vec3(0.0f, yPos, 0), glm::vec3(0.0f), glm::vec3(1.0f), false);
            
            if (isPBR) {
                SetMaterialUniform(pShaderProgram, "material", m_materialColor, m_materialShininess, 1.0f, useAO);
            }
        } else {
            /// InterioBox
            
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            //glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
            pShaderProgram->UseProgram();
            pShaderProgram->SetUniform("bReverseNormals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
            SetMaterialUniform(pShaderProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
            RenderPrimitive(pShaderProgram, m_pInteriorBox, glm::vec3(0.0f,  0.0f,  0.0f ), glm::vec3(0.0f), glm::vec3(100.0f, 80.0f, 80.0f)); // Render Big cube underneath
            pShaderProgram->SetUniform("bReverseNormals", 0); // and of course disable it
            //glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            
        }
    }
    
    
}

void Game::RenderScene(const GLboolean &toCustomShader, const GLboolean &includeLampsAndSkybox, const GLint &toCustomShaderIndex) {
    const GLboolean useAO = m_currentPPFXMode == PostProcessingEffectMode::SSAO;
    const GLfloat zfront = -200.0f;
    const GLfloat zback = 300.0f;
    m_sphereRotation += m_deltaTime * 0.02f;
   
    /// Skybox
    {
        // reset cubmap
        GLint cubemapTextureUnit = static_cast<GLint>(TextureType::CUBEMAP);
        m_pEnvSkybox->BindEnvSkyboxTo(cubemapTextureUnit);
        
        GLint shyboxIndex = includeLampsAndSkybox ? (toCustomShader ? toCustomShaderIndex : 1) : 1;
        CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[shyboxIndex];
        SetMaterialUniform(pSkyBoxProgram, "material");
        SetFogMaterialUniform(pSkyBoxProgram, "fog", m_fogColor, m_useFog);
        SetHRDLightUniform(pSkyBoxProgram, "hrdlight", m_exposure, m_gama, m_HDR);
        
        if (m_currentPPFXMode == PostProcessingEffectMode::IBL) {
            RenderEnvSkyBox(pSkyBoxProgram);
        } else {
            RenderSkyBox(pSkyBoxProgram);
        }
    }

    
    GLboolean isPBR =
    m_currentPPFXMode == PostProcessingEffectMode::PBR
    || m_currentPPFXMode == PostProcessingEffectMode::IBL;
    CShaderProgram *pShaderProgram;
    if (isPBR) {
        ///  Physically Based Rendering
        pShaderProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 3];
        SetCameraUniform(pShaderProgram, "camera", m_pCamera);
        SetMaterialUniform(pShaderProgram, "material", m_materialColor, m_materialShininess, 1.0f, useAO);
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
        SetLightUniform(pShaderProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetHRDLightUniform(pShaderProgram, m_hdrName, m_exposure, m_gama, m_HDR);
        RenderLight(pShaderProgram, m_dirName, m_pointName, m_spotName, m_pCamera);
        
    } else {
        
        /// Blinn Phong Lighting
        pShaderProgram = (*m_pShaderPrograms)[toCustomShader ? toCustomShaderIndex : 5];
        SetCameraUniform(pShaderProgram, "camera", m_pCamera);
        SetLightUniform(pShaderProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
        SetMaterialUniform(pShaderProgram, "material", m_materialColor, m_materialShininess, m_uvTiling, useAO);
        SetPBRMaterialUniform(pShaderProgram, "material", m_albedo, m_metallic, m_roughness, m_ao, m_useIrradiance);
        SetFogMaterialUniform(pShaderProgram, "fog", m_fogColor, m_useFog);
        SetHRDLightUniform(pShaderProgram, m_hdrName, m_exposure, m_gama, m_HDR);
        RenderLight(pShaderProgram, m_dirName, m_pointName, m_spotName, m_pCamera);
        /*
         // Render Wooden Boxes
         for ( GLuint i = 0; i < m_woodenBoxesPosition.size(); ++i){
         GLfloat angle = 20.0f * (GLfloat)i;
         glm::vec3 position = m_woodenBoxesPosition[i];
         RenderPrimitive(pShaderProgram, m_pWoodenBox, position, glm::vec3(0.0f, angle, 0.0f), 20, m_materialUseTexture);
         }
         */
    }
    
    ///*
    {
        RenderModel(pShaderProgram, m_lamborginhi, glm::vec3(0.0f, m_useTerrain ? -100.0f : -800.0f, 0.0f),
                    glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.5f));
    }
    
    RenderTerrainScene(pShaderProgram, -100.0f);
    RenderPBRScene(pShaderProgram, toCustomShader, toCustomShaderIndex, zfront, zback);
    //RenderRandomScene(pShaderProgram, toCustomShader, toCustomShaderIndex, zfront-200, zback+200);
    
    /// Render Lamps
    {
        GLint lampsIndex = includeLampsAndSkybox ? (toCustomShader ? toCustomShaderIndex : 4) : 4;
        CShaderProgram *pLampProgram = (*m_pShaderPrograms)[lampsIndex];
        SetCameraUniform(pLampProgram, "camera", m_pCamera);
        for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
            glm::vec3 position = std::get<0>(*it);
            glm::vec4 color = std::get<1>(*it);
            SetMaterialUniform(pLampProgram, "material", color, m_materialShininess, m_uvTiling, useAO);
            RenderLamp(pLampProgram, position, glm::vec3(10.0f));
        }
    }
    
}
