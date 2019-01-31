//
//  Game+IRenderer.cpp
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

void Game::RenderScene(){
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    
    // uncomment if stencil buffer is not used
    glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor
    
    // render others
    // Use the main shader program
    CShaderProgram *pBasicProgram = (*m_pShaderPrograms)[2];
    pBasicProgram->UseProgram();
    pBasicProgram->SetUniform("bUseTexture", true);
    pBasicProgram->SetUniform("sampler0", 0);
    
    // Set the projection matrix
    pBasicProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pBasicProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    // Render the planar terrain
    m_pPlanarTerrain->Transform(m_terrainPosition);
    glm::mat4 terrainModel = m_pPlanarTerrain->Model();
    pBasicProgram->SetUniform("matrices.modelMatrix", terrainModel);
    pBasicProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(terrainModel));
    m_pPlanarTerrain->Render();
    
    
    // render skybox
    CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[1];
    RenderSkyBox(pSkyBoxProgram, CUBEMAPTEXTUREUNIT);
    
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
