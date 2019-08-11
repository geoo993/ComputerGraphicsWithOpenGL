//
//  ILights.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef ILights_h
#define ILights_h

#include "../lighting/Lighting.h"

struct ILights
{
  
    // Light
    GLfloat m_ambient;
    GLfloat m_diffuse;
    GLfloat m_specular;
    GLboolean m_useBlinn;
    
    // HDR
    GLfloat m_exposure, m_gama;
    GLboolean m_HDR;
    std::string m_hdrName;
    
    // Attenuation
    GLfloat m_constant;
    GLfloat m_linear;
    GLfloat m_exponent;
    
    // Directional Light
    std::string m_dirName;
    GLboolean m_useDir;
    glm::vec3 m_dirColor;
    GLfloat m_dirIntensity;
    glm::vec3 m_directionalLightDirection;
    
    // Point Light
    std::string m_pointName;
    GLboolean m_usePoint;
    GLfloat m_pointIntensity;
    GLuint m_pointLightIndex = 0;
    std::vector<std::tuple<glm::vec3, glm::vec4>> m_pointLights;
    
    // Spot Light
    std::string m_spotName;
    GLboolean m_useSpot;
    GLboolean m_useSmoothSpot;
    glm::vec3 m_spotColor;
    GLfloat m_spotIntensity;
    GLfloat m_spotCutOff;
    GLfloat m_spotOuterCutOff;
    
    // Uniform
    virtual void SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
                                 const GLboolean &useSpot, const GLboolean &useSmoothSpot, const GLboolean& useBlinn) = 0;
    virtual void SetHRDLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                    const GLfloat & exposure, const GLfloat & gamma, const GLboolean &useHDR) = 0;
    virtual void SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                            const DirectionalLight& directionalLight, const glm::vec3& direction, const glm::vec3 &position) = 0;
    virtual void SetPointLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const PointLight& pointLight) = 0;
    virtual void SetSpotLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const SpotLight& spotLight,
                                     CCamera *camera) = 0;
    virtual void SetShadowUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const GLfloat &bias) = 0;
    virtual void SetShadowMatrix(CShaderProgram *pShaderProgram, const glm::vec3 &lightPosition) = 0;
    virtual void RenderLight(CShaderProgram *pShaderProgram,
                                const std::string &dirName,
                                const std::string &pointName,
                                const std::string &spotName,
                                CCamera *camera,
                                const GLboolean &useShadowMatrix) = 0;
    virtual void RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const glm::vec3 & scale) = 0;
};

#endif /* ILights_h */
