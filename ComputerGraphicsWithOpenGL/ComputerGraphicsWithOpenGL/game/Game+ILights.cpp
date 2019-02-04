//
//  ILights.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 02/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
                           const GLboolean &useSpot, const GLboolean &smoothSpot) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseDirectionalLight", useDir);
    pShaderProgram->SetUniform("bUsePointLight", usePoint);
    pShaderProgram->SetUniform("bUseSpotlight", useSpot);
    pShaderProgram->SetUniform("bUseSmoothSpot", smoothSpot);
    
}

void Game::SetBaseLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const BaseLight & baseLight) {
    pShaderProgram->SetUniform(uniformName + ".color", baseLight.color);
    pShaderProgram->SetUniform(uniformName + ".intensity", baseLight.intensity);
    
}

void Game::SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                      const DirectionalLight& directionalLight, const glm::vec3& direction)
{
    pShaderProgram->SetUniform(uniformName + ".base.color", directionalLight.color);
    pShaderProgram->SetUniform(uniformName + ".base.intensity", directionalLight.intensity);
    pShaderProgram->SetUniform(uniformName + ".base.ambient", m_ambient);
    pShaderProgram->SetUniform(uniformName + ".base.diffuse", m_diffuse);
    pShaderProgram->SetUniform(uniformName + ".base.specular", m_specular);
    pShaderProgram->SetUniform(uniformName + ".direction", direction);
    
}

void Game::SetPointLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const PointLight& pointLight)
{
    // POINT LIGHT
    pShaderProgram->SetUniform(uniformName + ".base.color", pointLight.color);
    pShaderProgram->SetUniform(uniformName + ".base.intensity", pointLight.intensity);
    pShaderProgram->SetUniform(uniformName + ".base.ambient", m_ambient);
    pShaderProgram->SetUniform(uniformName + ".base.diffuse", m_diffuse);
    pShaderProgram->SetUniform(uniformName + ".base.specular", m_specular);
    
    pShaderProgram->SetUniform(uniformName + ".attenuation.constant", pointLight.attenuation.constant);
    pShaderProgram->SetUniform(uniformName + ".attenuation.linear", pointLight.attenuation.linear);
    pShaderProgram->SetUniform(uniformName + ".attenuation.exponent", pointLight.attenuation.exponent);
    
    pShaderProgram->SetUniform(uniformName + ".position", pointLight.position);
    pShaderProgram->SetUniform(uniformName + ".range", pointLight.range);
}

void Game::SetSpotLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const SpotLight& spotLight, CCamera *camera)
{
    // SPOT LIGHT
    pShaderProgram->SetUniform(uniformName + ".pointLight.base.color", spotLight.color);
    pShaderProgram->SetUniform(uniformName + ".pointLight.base.intensity", spotLight.intensity);
    pShaderProgram->SetUniform(uniformName + ".pointLight.base.ambient", m_ambient);
    pShaderProgram->SetUniform(uniformName + ".pointLight.base.diffuse", m_diffuse);
    pShaderProgram->SetUniform(uniformName + ".pointLight.base.specular", m_specular);
    
    pShaderProgram->SetUniform(uniformName + ".pointLight.attenuation.constant", spotLight.attenuation.constant);
    pShaderProgram->SetUniform(uniformName + ".pointLight.attenuation.linear", spotLight.attenuation.linear);
    pShaderProgram->SetUniform(uniformName + ".pointLight.attenuation.exponent", spotLight.attenuation.exponent);
    
    pShaderProgram->SetUniform(uniformName + ".pointLight.position", spotLight.position);
    pShaderProgram->SetUniform(uniformName + ".pointLight.range", spotLight.range);
    pShaderProgram->SetUniform(uniformName + ".direction", camera->GetForward() );
    pShaderProgram->SetUniform(uniformName + ".cutOff", spotLight.cutOff);
    pShaderProgram->SetUniform(uniformName + ".outerCutOff", spotLight.outerCutOff);

}



void Game::RenderLight(CShaderProgram *pShaderProgram, CCamera * camera) {
    pShaderProgram->UseProgram();

    // Directional light
    DirectionalLight dirLight(m_dirColor, m_dirIntensity);
    SetDirectionalLightUniform(pShaderProgram, "R_directionallight", dirLight, m_directionalLightDirection);
    
    // Point Light
    for ( GLuint i = 0; i < m_pointLightPositions.size(); ++i){
        string uniformName = "R_pointlight[" + std::to_string(i) + "]";
        glm::vec3 position = m_pointLightPositions[i];
        glm::vec3 color = m_pointLightColors[i];
        PointLight pointLight(color, m_pointIntensity, Attenuation(m_constant, m_linear, m_exponent), position);
        SetPointLightUniform(pShaderProgram, uniformName, pointLight);
    }
    
    //Spot Light
    GLfloat cutOff = glm::cos(glm::radians(m_spotCutOff));
    GLfloat outerCutOff = glm::cos(glm::radians(m_spotOuterCutOff));
    SpotLight spotLight(m_spotColor, m_spotIntensity, Attenuation(m_constant, m_linear, m_exponent), camera->GetPosition(), cutOff, outerCutOff);
    SetSpotLightUniform(pShaderProgram, "R_spotlight", spotLight, m_pCamera);
    
}

void Game::RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale, const glm::vec3 & color) {
    glm::vec3 pos = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        pos = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pLamp->Transform(pos, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("color", color);
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    
    glm::mat4 model = m_pLamp->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pLamp->Render(false);
}
