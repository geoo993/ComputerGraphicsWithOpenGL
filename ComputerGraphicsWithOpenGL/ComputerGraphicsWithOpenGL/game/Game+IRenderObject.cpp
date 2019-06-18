//
//  Game+IRenderOeject.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                const GLfloat & scale, const GLboolean &useTexture, const GLboolean &bindTexture) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
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
            m_pSkybox->Release();
            m_pSkybox = new CSkybox;
            
            m_pSkybox->Create(m_mapSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::Default, nullptr, nullptr, TextureType::EMISSION, m_skyboxNumber);
            
            m_pEnvSkybox->Release();
            m_pEnvSkybox = new CSkybox;
            CShaderProgram *pEquirectangularCubeProgram = (*m_pShaderPrograms)[77];
            SetMaterialUniform(pEquirectangularCubeProgram, "material", glm::vec4(1.0f));
            m_pEnvSkybox->Create(m_mapSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::EnvironmentMap, nullptr, pEquirectangularCubeProgram, TextureType::EMISSION, m_skyboxNumber);
            
            m_pIrrSkybox->Release();
            m_pIrrSkybox = new CSkybox;
            CShaderProgram *pIrradianceProgram = (*m_pShaderPrograms)[78];
            SetMaterialUniform(pIrradianceProgram, "material", glm::vec4(1.0f));
            m_pIrrSkybox->Create(m_mapSize, m_gameManager->GetResourcePath(), TextureType::CUBEMAP, SkyboxType::IrradianceMap, pIrradianceProgram, pEquirectangularCubeProgram, TextureType::EMISSION, m_skyboxNumber);
            
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
    m_pEnvSkybox->Render(true, SkyboxType::EnvironmentMap);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void Game::RenderIrrSkyBox(CShaderProgram *pShaderProgram) {
    ResetSkyBox(pShaderProgram);
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseEnvCubemap", true);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrixWithoutTranslation", m_pCamera->GetViewWithoutTranslation());
    m_pIrrSkybox->Render(true, SkyboxType::EnvironmentMap);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void Game::RenderTerrain(CShaderProgram *pShaderProgram, const GLboolean &useHeightMap, const GLboolean &useTexture) {
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseHeightMap", useHeightMap);
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("fMinHeight", m_heightMapMinHeight);
    pShaderProgram->SetUniform("fMaxHeight", m_heightMapMaxHeight);
    
    // Set the projection matrix
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    if (useHeightMap == true) {
        // Render the height map terrain
        m_pHeightmapTerrain->Transform(glm::vec3(0.0f));
        glm::mat4 model = m_pHeightmapTerrain->Model();
        pShaderProgram->SetUniform("matrices.modelMatrix", model);
        pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
        m_pHeightmapTerrain->Render();
    } else {
        // Render the planar terrain
        //glEnable (GL_BLEND);
        //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_pPlanarTerrain->Transform(glm::vec3(0.0f));
        glm::mat4 terrainModel = m_pPlanarTerrain->Model();
        pShaderProgram->SetUniform("matrices.modelMatrix", terrainModel);
        pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(terrainModel));
        m_pPlanarTerrain->Render();
        //glDisable (GL_BLEND);
    }
    
}

void Game::RenderCrossBow(CShaderProgram *pShaderProgram, const glm::vec3 & position, const GLfloat & scale, const GLboolean &useTexture) {
    /*
    //m_barrelRotation += 0.01;
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    m_pBarrel->Transform(translation, glm::vec3(0.0f,m_barrelRotation, 0.0f), glm::vec3(scale));
    
    glm::mat4 model = m_pBarrel->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pBarrel->Render();
    */
}

void Game::RenderGrenade(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                         const GLfloat & scale, const GLboolean &useTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    m_pGrenade->Transform(translation, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(scale));
    
    glm::mat4 model = m_pGrenade->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pGrenade->Render(pShaderProgram);
}

void Game::RenderCube(CShaderProgram *pShaderProgram, const glm::vec3 & position, const GLfloat & scale, const GLboolean &useTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pCube->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pCube->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pCube->Render();
}

void Game::RenderInteriorBox(CShaderProgram *pShaderProgram, const glm::vec3 &position,
                       const float & scale, const bool &useTexture, const bool &bindTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pInteriorBox->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pInteriorBox->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    //glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
    pShaderProgram->SetUniform("bReverseNormals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
    m_pInteriorBox->Render();
    pShaderProgram->SetUniform("bReverseNormals", 0); // and of course disable it
    //glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
}

void Game::RenderParallaxCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                const GLfloat & scale, const GLboolean &useTexture){
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pParallaxCube->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pParallaxCube->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pParallaxCube->Render();
}

void Game::RenderChromaticAberrationCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                         const GLfloat & scale, const GLboolean &useTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pChromaticAberrationCube->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pShaderProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pChromaticAberrationCube->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pChromaticAberrationCube->Render();
}

void Game::RenderWoodenBox(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale,
                           const GLfloat & angle, const GLboolean &useTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    m_pWoodenBox->Transform(translation, glm::vec3(1.0f, angle, 1.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pWoodenBox->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pWoodenBox->Render();

}

void Game::RenderSphere(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                        const GLfloat & scale, const GLboolean &useTexture) {
    
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pSphere->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
        
    glm::mat4 model = m_pSphere->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pSphere->Render();
    
}

void Game::RenderFireBallSphere(CShaderProgram *pShaderProgram, const glm::vec3 & position, const GLfloat & scale) {
    
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pFireBallSphere->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    glm::mat4 model = m_pFireBallSphere->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pFireBallSphere->Render();
    
}

void Game::RenderTorus(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                       const GLfloat & scale, const GLboolean &useTexture) {
    
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pTorus->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pTorus->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pTorus->Render();
    
}
void Game::RenderTorusKnot(CShaderProgram *pShaderProgram, const glm::vec3 & position, const GLfloat & scale, const GLboolean &useTexture) {
    glm::vec3 translation = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        translation = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pTorusKnot->Transform(translation, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
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
    // Update the metaballs' positions
    float time = (float)m_deltaTime / 1000.0f * 2.0f * 3.14159f * 0.5f;
    m_pMetaballs->Update(time);
    
    glm::mat4 model = m_pMetaballs->Model();
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseTexture", useTexture);
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    // Render the metaballs
    m_pMetaballs->Transform(position, glm::vec3(0.0f), glm::vec3(scale));
    m_pMetaballs->Render(useTexture);
    
}
