//
//  Game+IRenderOeject.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderSkyBox(CShaderProgram *pShaderProgram, const int &cubeMapTextureUnit) {
    // start by deleting current skybox and create new one
    if (m_changeSkybox == true) {
        m_pSkybox->Release();
        m_pSkybox->Create(m_mapSize, m_gameManager.GetResourcePath(), m_skyboxNumber);
        //cout << "Changing skybox to " << m_skyboxNumber << endl;
        m_changeSkybox = false;
    }
    
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("cubeMapTex", cubeMapTextureUnit);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    
    m_pSkybox->Transform(m_pCamera->GetPosition());
    glm::mat4 skyBoxModel = m_pSkybox->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", skyBoxModel);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(skyBoxModel));
    m_pSkybox->Render(cubeMapTextureUnit);
    glDepthFunc(GL_LESS);
    
}

void Game::RenderTerrain(CShaderProgram *pShaderProgram, const bool &useHeightMap, const bool &useTexture) {
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("material.ambientMap", 0);
    pShaderProgram->SetUniform("material.normalMap", 1);
    pShaderProgram->SetUniform("material.diffuseMap", 2);
    pShaderProgram->SetUniform("material.specularMap", 3);
    pShaderProgram->SetUniform("bUseHeightMap", useHeightMap);
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("fMinHeight", m_heightMapMinHeight);
    pShaderProgram->SetUniform("fMaxHeight", m_heightMapMaxHeight);
    
    // Set the projection matrix
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (useHeightMap == true) {
        // Render the height map terrain
        m_pHeightmapTerrain->Transform(glm::vec3(0.0f));
        glm::mat4 model = m_pHeightmapTerrain->Model();
        pShaderProgram->SetUniform("matrices.modelMatrix", model);
        pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
        m_pHeightmapTerrain->Render();
    }else {
        // Render the planar terrain
        m_pPlanarTerrain->Transform(glm::vec3(0.0f));
        glm::mat4 terrainModel = m_pPlanarTerrain->Model();
        pShaderProgram->SetUniform("matrices.modelMatrix", terrainModel);
        pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(terrainModel));
        m_pPlanarTerrain->Render();
    }
    glDisable (GL_BLEND);
    
}

void Game::RenderBarrel(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) {
    
    m_barrelRotation += 0.01;
    glm::vec3 position = m_barrelPosition;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        position = glm::vec3(m_barrelPosition.x, m_barrelPosition.y+m_pHeightmapTerrain->ReturnGroundHeight(m_barrelPosition), m_barrelPosition.z);
    }
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("sampler0", 0);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    m_pBarrel->Transform(position, glm::vec3(0.0f,m_barrelRotation, 0.0f), glm::vec3(scale));
    
    glm::mat4 model = m_pBarrel->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pBarrel->Render();
    
}
void Game::RenderCube(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) {
    glm::vec3 position = m_cubePosition;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        position = glm::vec3(m_cubePosition.x, m_cubePosition.y+m_pHeightmapTerrain->ReturnGroundHeight(m_cubePosition), m_cubePosition.z);
    }
    
    m_pCube->Transform(position, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("sampler0", 0);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    glm::mat4 model = m_pCube->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pCube->Render();
}

void Game::RenderSphere(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) {
    
    glm::vec3 position = m_spherePosition;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        position = glm::vec3(m_spherePosition.x, m_spherePosition.y+m_pHeightmapTerrain->ReturnGroundHeight(m_spherePosition), m_spherePosition.z);
    }
    
    m_pSphere->Transform(position, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("sampler0", 0);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
        
    glm::mat4 model = m_pSphere->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pSphere->Render();
    
}

void Game::RenderTorus(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) {
    
    glm::vec3 position = m_torusPosition;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        position = glm::vec3(m_torusPosition.x, m_torusPosition.y+m_pHeightmapTerrain->ReturnGroundHeight(m_torusPosition), m_torusPosition.z);
    }
    
    m_pTorus->Transform(position, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("sampler0", 0);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    glm::mat4 model = m_pTorus->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pTorus->Render();
    
}
void Game::RenderTorusKnot(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) {
    glm::vec3 position = m_torusKnotPosition;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        position = glm::vec3(m_torusKnotPosition.x, m_torusKnotPosition.y+m_pHeightmapTerrain->ReturnGroundHeight(m_torusKnotPosition), m_torusKnotPosition.z);
    }
    
    m_pTorusKnot->Transform(position, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("sampler0", 0);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    glm::mat4 model = m_pTorusKnot->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pTorusKnot->Render();
}

void Game::RenderMetalBalls(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) {
    // Update the metaballs' positions
    float time = (float)m_deltaTime / 1000.0f * 2.0f * 3.14159f * 0.5f;
    m_pMetaballs->Update(time);
    
    glm::mat4 model = m_pMetaballs->Model();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    
    // Render the metaballs
    m_pMetaballs->Transform(m_metalballsPosition, glm::vec3(0.0f), glm::vec3(scale));
    m_pMetaballs->Render(useTexture);
    
}
