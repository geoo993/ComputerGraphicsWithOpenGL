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
                                      const DirectionalLight& directionalLight, const glm::vec3& direction, const glm::vec3 &position) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName + ".base.color", directionalLight.color);
    pShaderProgram->SetUniform(uniformName + ".base.intensity", directionalLight.intensity);
    pShaderProgram->SetUniform(uniformName + ".base.ambient", m_ambient);
    pShaderProgram->SetUniform(uniformName + ".base.diffuse", m_diffuse);
    pShaderProgram->SetUniform(uniformName + ".base.specular", m_specular);
    pShaderProgram->SetUniform(uniformName + ".direction", direction);
    pShaderProgram->SetUniform(uniformName + ".position", position);
    
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

void Game::SetShadowUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const GLfloat &bias) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName+".znear", (GLfloat)SHADOW_ZNEAR);
    pShaderProgram->SetUniform(uniformName+".zfar", (GLfloat)SHADOW_ZFAR);
    pShaderProgram->SetUniform(uniformName+".bias", bias);
    pShaderProgram->SetUniform(uniformName+".bFromLightOrCamera", m_fromLightPosition);
    pShaderProgram->SetUniform(uniformName+".bShowDepth", m_showDepth);
}

void Game::SetShadowMatrix(CShaderProgram *pShaderProgram, const glm::vec3 &lightPosition) {
    pShaderProgram->UseProgram();
    
    switch(m_currentPPFXMode) {
        case PostProcessingEffectMode::DepthMapping: {
            
            /*
             Perspective projections make more sense for light sources that have actual locations unlike directional lights.
             Perspective projections are thus most often used with spotlights and point lights while orthographic projections are used for directional lights.
             */
            glm::vec3 lightPos = m_fromLightPosition ? lightPosition : m_pCamera->GetPosition();
            GLfloat orthogonalBoxSize = (GLfloat)SHADOW_ORTH_BOX; // this is the cubic bounding box from the light in the scene, determines where the light is projected
            glm::mat4 orthoProjection = glm::ortho(-orthogonalBoxSize, orthogonalBoxSize, -orthogonalBoxSize, orthogonalBoxSize, (GLfloat)SHADOW_ZNEAR, (GLfloat)SHADOW_ZFAR);
            glm::mat4 persProjection = glm::perspective(glm::radians(m_pCamera->GetFieldOfView()), m_pCamera->GetAspectRatio(), (GLfloat)SHADOW_ZNEAR, (GLfloat)SHADOW_ZFAR);
            glm::mat4 lightProjection = m_isOrthographicCamera ? orthoProjection : persProjection;
            glm::mat4 lightView = glm::lookAt(lightPos,                     // The  eye is the position of the camera's viewpoint,
                                              glm::vec3(0.0f),              // The center is where you are looking at (a position which in this case is the center of the screen). If you want to use a direction vector D instead of a center position, you can simply use eye + D as the center position, where D can be a unit vector for example.
                                              glm::vec3(0.0f, 1.0f, 0.0f)); // The up vector is basically a vector defining your world's "upwards" direction. In almost all normal cases, this will be the vector (0, 1, 0) i.e. towards positive Y.
            glm::mat4 lightSpaceMatrix = lightProjection * (m_fromLightPosition ? lightView : m_pCamera->GetViewMatrix());
            CShaderProgram *pLightSpaceProgram = (*m_pShaderPrograms)[51];
            pLightSpaceProgram->UseProgram();
            pLightSpaceProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
            
            return;
        }
        case PostProcessingEffectMode::DirectionalShadowMapping: {
            // DirectionalShadowMapping
            /*
             Perspective projections make more sense for light sources that have actual locations unlike directional lights.
             Perspective projections are thus most often used with spotlights and point lights while orthographic projections are used for directional lights.
             */
            glm::vec3 lightPos = m_fromLightPosition ? lightPosition : m_pCamera->GetPosition();
            GLfloat orthogonalBoxSize = (GLfloat)SHADOW_ORTH_BOX; // this is the cubic bounding box from the light to the scene, determines where the light is projected
            glm::mat4 lightProjection = glm::ortho(-orthogonalBoxSize, orthogonalBoxSize, -orthogonalBoxSize, orthogonalBoxSize, (GLfloat)SHADOW_ZNEAR, (GLfloat)SHADOW_ZFAR);
            glm::mat4 lightView = glm::lookAt(lightPos,                     // The  eye is the position of the camera's viewpoint,
                                              glm::vec3(0.0f),  // THIS COULD BE OUR SHADOW PROBLEM             // The center is where you are looking at (a position which in this case is the center of the screen). If you want to use a direction vector D instead of a center position, you can simply use eye + D as the center position, where D can be a unit vector for example.
                                              glm::vec3(0.0f, 1.0f, 0.0f)); // The up vector is basically a vector defining your world's "upwards" direction. In almost all normal cases, this will be the vector (0, 1, 0) i.e. towards positive Y.
            glm::mat4 lightSpaceMatrix = lightProjection * (m_fromLightPosition ? lightView : m_pCamera->GetViewMatrix());
            pShaderProgram->SetUniform("matrices.lightSpaceMatrix", lightSpaceMatrix);
            return;
        }
        case PostProcessingEffectMode::OmnidirectionalShadowMapping: {
            
            // OmnidirectionalShadowMapping
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, (GLfloat)SHADOW_ZNEAR, (GLfloat)SHADOW_ZFAR);
            std::vector<glm::mat4> shadowTransforms;
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            
            for (unsigned int i = 0; i < 6; ++i) {
                pShaderProgram->SetUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
            }
            return;
        }
        default: return;
    }
    
}

void Game::RenderLight(CShaderProgram *pShaderProgram,
                        const std::string &dirName,
                        const std::string &pointName,
                        const std::string &spotName,
                        CCamera *camera,
                        const GLboolean &useShadowMatrix) {
    pShaderProgram->UseProgram();
    glm::vec3 dirLightPos = std::get<0>(m_pointLights.back());
                            
    // Directional light
    DirectionalLight dirLight(m_dirColor, m_dirIntensity);
    SetDirectionalLightUniform(pShaderProgram, dirName, dirLight, m_directionalLightDirection, dirLightPos);
    if (useShadowMatrix && m_useDir) SetShadowMatrix(pShaderProgram, dirLightPos);

    // Point Light
    for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
        auto i = std::distance(m_pointLights.begin(), it);
        std::string uniformName = pointName+"[" + std::to_string(i) + "]";
        glm::vec3 position = std::get<0>(*it);
        if (m_pHeightmapTerrain->IsHeightMapRendered()) {
            position = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
        }
        glm::vec3 color = glm::vec3(std::get<1>(*it));
        PointLight pointLight(color, m_pointIntensity, Attenuation(m_constant, m_linear, m_exponent), position);
        SetPointLightUniform(pShaderProgram, uniformName, pointLight);
        if (useShadowMatrix && m_usePoint) SetShadowMatrix(pShaderProgram, position);
    }
    
    //Spot Light
    GLfloat cutOff = glm::cos(glm::radians(m_spotCutOff));
    GLfloat outerCutOff = glm::cos(glm::radians(m_spotOuterCutOff));
    SpotLight spotLight(m_spotColor, m_spotIntensity, Attenuation(m_constant, m_linear, m_exponent), camera->GetPosition(), cutOff, outerCutOff);
    SetSpotLightUniform(pShaderProgram, spotName, spotLight, m_pCamera);
    if (useShadowMatrix && m_useSpot) SetShadowMatrix(pShaderProgram, camera->GetPosition());

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

void Game::RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const glm::vec3 & scale) {
    glm::vec3 pos = position;
    if (m_pHeightmapTerrain->IsHeightMapRendered()) {
        pos = glm::vec3(position.x, position.y+m_pHeightmapTerrain->ReturnGroundHeight(position), position.z);
    }
    
    m_pLamp->Transform(pos, glm::vec3(0.0f), scale);
    
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
