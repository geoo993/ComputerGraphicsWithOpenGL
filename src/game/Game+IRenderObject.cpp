//
//  Game+IRenderOeject.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                const glm::vec3 & scale, const GLboolean &bindTexture) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    m_pQuad->Transform(position, glm::vec3(0.0f, 0.0f, 0.0f), scale);
    
    glm::mat4 model = m_pQuad->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pQuad->Render(bindTexture);
}

void Game::ResetSkyBox(CShaderProgram *pShaderProgram) {
    // start by deleting current skybox and create new one
    if (m_changeSkybox == true) {
        
        // Create the skybox
        // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9

        m_pSkybox->Clear();
        m_pSkybox->Create(m_skyboxSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::Default, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
        
        m_pEnvSkybox->Clear();
        m_pEnvSkybox->Create(m_skyboxSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::EnvironmentMap, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);

        m_pIrrSkybox->Clear();
        m_pIrrSkybox->Create(m_skyboxSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::IrradianceMap, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
        
        m_changeSkybox = false;
    }
    
}

void Game::RenderSkyBox(CShaderProgram *pShaderProgram) {

    ResetSkyBox(pShaderProgram);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseEnvCubemap", false);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrixWithoutTranslation", m_pCamera->GetViewWithoutTranslation());
    
    m_pSkybox->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    
    glm::mat4 model = m_pSkybox->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pSkybox->Render(true, SkyboxType::Default);
    glDepthFunc(GL_LESS); // set depth function back to default
    
}

void Game::RenderEnvSkyBox(CShaderProgram *pShaderProgram) {
    ResetSkyBox(pShaderProgram);
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseEnvCubemap", true);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrixWithoutTranslation", m_pCamera->GetViewWithoutTranslation());
    
    m_pIrrSkybox->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    
    glm::mat4 model = m_pIrrSkybox->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    
    m_pIrrSkybox->Render(true, m_useIrradianceMap ? SkyboxType::PrefilterMap : SkyboxType::EnvironmentMap);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void Game::RenderTerrain(CShaderProgram *pShaderProgram, const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale, const GLboolean &useHeightMap) {
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    if (useHeightMap == true) {
        // Render the height map terrain
        m_pHeightmapTerrain->Transform(position, rotation, scale);
        glm::mat4 model = m_pHeightmapTerrain->Model();
        pShaderProgram->SetUniform("matrices.modelMatrix", model);
        pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
        m_pHeightmapTerrain->Render();
    } else {
        // Render the planar terrain
//        glEnable (GL_BLEND);
//        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_pPlanarTerrain->Transform(position, rotation, glm::vec3(scale));
        
        glm::mat4 terrainModel = m_pPlanarTerrain->Model();
        pShaderProgram->SetUniform("matrices.modelMatrix", terrainModel);
        pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(terrainModel));
        m_pPlanarTerrain->Render();
//        glDisable (GL_BLEND);
    }
    
}

void  Game::RenderPrimitive(CShaderProgram *pShaderProgram, IGameObject *object, const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale, const GLboolean &useTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    
    object->Transform(translation, rotation, scale);
    
    glm::mat4 model = object->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    object->Render(useTexture);
}

void Game::RenderModel(CShaderProgram *pShaderProgram, CModel * model, const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    
    model->Transform(translation, rotation, scale);
    
    glm::mat4 m = model->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", m);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(m));
    model->Render(pShaderProgram);
}

void Game::RenderMetalBalls(CShaderProgram *pShaderProgram, const glm::vec3 & position, const glm::vec3 & scale, const GLboolean &useTexture) {
    
    pShaderProgram->UseProgram();
    
    // Update the metaballs' positions
    float time = (float)m_deltaTime / 1000.0f * 2.0f * 3.14159f * 0.5f;
    m_pMetaballs->Update(time);
    
    // Render the metalballs
    RenderPrimitive(pShaderProgram, m_pMetaballs, position, glm::vec3(0.0f), scale, useTexture);
}

