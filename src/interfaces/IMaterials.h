//
//  IMaterials.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IMaterials_h
#define IMaterials_h

#include "../shaders/ShaderProgram.h"

struct IMaterials {
    glm::vec4 m_materialColor;
    GLfloat m_materialShininess, m_albedo, m_metallic, m_roughness, m_ao;
    GLboolean m_useIrradianceMap, m_materialUseTexture, m_materialUseColor, m_useIrradiance;
    GLboolean m_useFog;
    glm::vec3 m_fogColor;
    virtual void SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                    const glm::vec4 &color, const GLfloat &shininess,
                                    const GLfloat &uvTiling, const GLboolean &useAO, const glm::vec4 &guiColor) = 0;
    virtual void SetPBRMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                       const GLfloat &albedo, const GLfloat &metallic, const GLfloat &roughness,
                                       const GLfloat &ao, const GLboolean &useIrradiance) = 0;
    virtual void SetFogMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                        const glm::vec3 &color, const GLboolean &bUseFog) = 0;
};

#endif /* IMaterials_h */
