//
//  IRenderObject.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IRenderObject_h
#define IRenderObject_h

#include "../shaders/ShaderProgram.h"
#include "../interfaces/IGameObject.h"

struct IRenderObject
{
    virtual void RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                            const glm::vec3 & scale, const GLboolean &bindTexture) = 0;
    virtual void RenderSkyBox(CShaderProgram *pShaderProgram) = 0;
    virtual void RenderEnvSkyBox(CShaderProgram *pShaderProgram) = 0;
    virtual void ResetSkyBox(CShaderProgram *pShaderProgram) = 0;
    virtual void RenderTerrain(CShaderProgram *pShaderProgram, const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale, const GLboolean &useHeightMap) = 0;
    virtual void RenderPrimitive(CShaderProgram *pShaderProgram, IGameObject *object, const glm::vec3 & position,
                                const glm::vec3 & rotation, const glm::vec3 & scale, const GLboolean &useTexture) = 0;
    virtual void RenderMetalBalls(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                  const glm::vec3 & scale, const GLboolean &useTexture) = 0;
};

#endif /* IRenderObject_h */
