//
//  Game+IRenderOeject.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                const GLfloat & scale, const GLboolean &bindTexture) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    m_pQuad->Transform(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(scale));
    
    glm::mat4 model = m_pQuad->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pQuad->Render(bindTexture);
}

void Game::ResetSkyBox(CShaderProgram *pShaderProgram) {
    if (m_changeSkybox == true) {
        // start by deleting current skybox and create new one
        if (m_changeSkybox == true) {
            m_pSkybox->Clear();
            m_pSkybox->Create(m_skyboxSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::Default, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
            
            m_pEnvSkybox->Clear();
            m_pEnvSkybox->Create(m_skyboxSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::EnvironmentMap, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
            
            m_pIrrSkybox->Clear();
            m_pIrrSkybox->Create(m_skyboxSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::IrradianceMap, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
            
            m_changeSkybox = false;
        }
    }
}

void Game::RenderSkyBox(CShaderProgram *pShaderProgram) {

    ResetSkyBox(pShaderProgram);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseEnvCubemap", false);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrixWithoutTranslation", m_pCamera->GetViewWithoutTranslation());
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
    pShaderProgram->SetUniform("matrices.viewMatrixWithoutTranslation", m_pCamera->GetViewWithoutTranslation());
    m_pIrrSkybox->Render(true, m_useIrradianceMap ? SkyboxType::PrefilterMap : SkyboxType::EnvironmentMap);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void Game::RenderTerrain(CShaderProgram *pShaderProgram, const glm::vec3 & position, const glm::vec3 & rotation, const GLfloat & scale, const GLboolean &useHeightMap) {
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    if (useHeightMap == true) {
        // Render the height map terrain
        m_pHeightmapTerrain->Transform(position, rotation, glm::vec3(scale));
        glm::mat4 model = m_pHeightmapTerrain->Model();
        pShaderProgram->SetUniform("matrices.modelMatrix", model);
        pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
        m_pHeightmapTerrain->Render();
    } else {
        // Render the planar terrain
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_pPlanarTerrain->Transform(position, rotation, glm::vec3(scale));
        
        glm::mat4 terrainModel = m_pPlanarTerrain->Model();
        pShaderProgram->SetUniform("matrices.modelMatrix", terrainModel);
        pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(terrainModel));
        m_pPlanarTerrain->Render();
        glDisable (GL_BLEND);
    }
    
}

void Game::RenderModel(CShaderProgram *pShaderProgram, CModel * model, const glm::vec3 & position, const glm::vec3 & rotation, const GLfloat & scale) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    model->Transform(translation, rotation, glm::vec3(scale));
    
    glm::mat4 m = model->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", m);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(m));
    model->Render(pShaderProgram);
}

void Game::RenderCube(CShaderProgram *pShaderProgram, CCube *cube, const glm::vec3 & position, const glm::vec3 & rotation, const GLfloat & scale, const GLboolean &useTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    cube->Transform(translation, rotation, glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = cube->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    cube->Render(useTexture);
}

void Game::RenderSphere(CShaderProgram *pShaderProgram, CSphere *sphere, const glm::vec3 & position, const glm::vec3 & rotation, const GLfloat & scale, const GLboolean &useTexture) {
    
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    sphere->Transform(translation, rotation, glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
        
    glm::mat4 model = sphere->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    sphere->Render(useTexture);
    
}

void Game::RenderTorus(CShaderProgram *pShaderProgram, const glm::vec3 & position, const GLfloat & scale) {
    
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pTorus->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pTorus->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pTorus->Render();
    
}

void Game::RenderTorusKnot(CShaderProgram *pShaderProgram, const glm::vec3 & position, const GLfloat & scale) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pTorusKnot->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pTorusKnot->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pTorusKnot->Render();
}

void Game::RenderMetalBalls(CShaderProgram *pShaderProgram, const glm::vec3 & position, const GLfloat & scale, const GLboolean &useTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    // Update the metaballs' positions
    float time = (float)m_deltaTime / 1000.0f * 2.0f * 3.14159f * 0.5f;
    m_pMetaballs->Update(time);
    
    // Render the metaballs
    m_pMetaballs->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pMetaballs->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pMetaballs->Render(useTexture);
    
}
