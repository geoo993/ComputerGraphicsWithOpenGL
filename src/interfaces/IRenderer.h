//
//  IRenderer.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IRenderer_h
#define IRenderer_h

#include "../shaders/ShaderProgram.h"

struct IRenderer
{
    virtual void PreRendering() = 0;
    virtual void Render() = 0;
    virtual void PostRendering() = 0;
    virtual void RenderScene(const GLboolean &toCustomShader, const GLboolean &includeLampsAndSkybox, const GLint &toCustomShaderIndex) = 0;
    virtual void RenderPBRScene(CShaderProgram *pShaderProgram, const GLboolean &toCustomShader, const GLint &toCustomShaderIndex, const GLfloat zfront, const GLfloat zback) = 0;
    virtual void RenderRandomScene(CShaderProgram *pShaderProgram, const GLboolean &toCustomShader, const GLint &toCustomShaderIndex, const GLfloat zfront, const GLfloat zback) = 0;
    virtual void RenderTerrainScene(CShaderProgram *pShaderProgram, const GLfloat yPos) = 0;
};

#endif /* IRenderer_h */
