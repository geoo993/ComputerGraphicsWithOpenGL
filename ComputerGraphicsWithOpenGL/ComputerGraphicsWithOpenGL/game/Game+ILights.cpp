//
//  ILights.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 02/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
                           const GLboolean &useSpot, const GLboolean &useSmoothSpot, const GLboolean& useBlinn) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseDirectionalLight", useDir);
    pShaderProgram->SetUniform("bUsePointLight", usePoint);
    pShaderProgram->SetUniform("bUseSpotlight", useSpot);
    pShaderProgram->SetUniform("bUseSmoothSpot", useSmoothSpot);
    pShaderProgram->SetUniform("bUseBlinn", useBlinn);
}

void Game::SetHRDLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                        const GLfloat & exposure, const GLfloat & gamma, const GLboolean &useHDR) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName + ".exposure", exposure);
    pShaderProgram->SetUniform(uniformName + ".gamma", gamma);
    pShaderProgram->SetUniform(uniformName + ".bHDR", useHDR);
}

void Game::SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                      const DirectionalLight& directionalLight, const glm::vec3& direction) {
    pShaderProgram->UseProgram();
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
    pShaderProgram->UseProgram();
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
    pShaderProgram->UseProgram();
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

void Game::SetShadowUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const GLfloat &znear, const GLfloat &zfar) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName+".znear", znear);
    pShaderProgram->SetUniform(uniformName+".zfar", zfar);
}

void Game::RenderLight(CShaderProgram *pShaderProgram, CCamera * camera) {
    pShaderProgram->UseProgram();

    // HDR Light
    SetHRDLightUniform(pShaderProgram, "hrdlight", m_exposure, m_gama, m_HDR);
    
    // Directional light
    DirectionalLight dirLight(m_dirColor, m_dirIntensity);
    SetDirectionalLightUniform(pShaderProgram, "R_directionallight", dirLight, m_directionalLightDirection);
    
    // Point Light
    for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
        auto i = std::distance(m_pointLights.begin(), it);
        std::string uniformName = "R_pointlight[" + std::to_string(i) + "]";
        glm::vec3 position = std::get<0>(*it);
        if (m_pHeightmapTerrain->IsHeightMapRendered()) {
            position = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
        }
        glm::vec3 color = glm::vec3(std::get<1>(*it));
        PointLight pointLight(color, m_pointIntensity, Attenuation(m_constant, m_linear, m_exponent), position);
        SetPointLightUniform(pShaderProgram, uniformName, pointLight);
    }
    
    //Spot Light
    GLfloat cutOff = glm::cos(glm::radians(m_spotCutOff));
    GLfloat outerCutOff = glm::cos(glm::radians(m_spotOuterCutOff));
    SpotLight spotLight(m_spotColor, m_spotIntensity, Attenuation(m_constant, m_linear, m_exponent), camera->GetPosition(), cutOff, outerCutOff);
    SetSpotLightUniform(pShaderProgram, "R_spotlight", spotLight, m_pCamera);
    
    //Attenuation courtesy of http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    /*
     lights, with courtesy of Ogre3D's wiki:
     
     Distance    Constant      Linear      Quadratic
     7              1.0         0.7         1.8
     13             1.0         0.35        0.44
     20             1.0         0.22        0.20
     32             1.0         0.14        0.07
     50             1.0         0.09        0.032
     65             1.0         0.07        0.017
     100            1.0         0.045       0.0075
     160            1.0         0.027       0.0028
     200            1.0         0.022       0.0019
     325            1.0         0.014       0.0007
     600            1.0         0.007       0.0002
     3250           1.0         0.0014      0.000007

     */

}

void Game::RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale) {
    glm::vec3 pos = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        pos = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pLamp->Transform(pos, glm::vec3(0.0f), glm::vec3(scale));
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 lightSpaceMatrix = (*m_pCamera->GetOrthographicProjectionMatrix()) * m_pCamera->GetViewMatrix();
    pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
    
    glm::mat4 model = m_pLamp->Model();
    pShaderProgram->SetUniform("matrices.modelMatrix", model);
    pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(model));
    m_pLamp->Render(false);
}
