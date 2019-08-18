//
//  ICamera.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef ICamera_h
#define ICamera_h

#include "../camera/Camera.h"
#include "../shaders/ShaderProgram.h"

struct ICamera {
    CCamera *m_pCamera;
    GLfloat m_viewPointAngle;
    virtual void InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 &position) = 0;
    virtual void SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) = 0;
    virtual void UpdateCamera(const GLdouble & deltaTime, const MouseState &mouseState, const KeyboardState &keyboardState,
                            const GLboolean & mouseMove) = 0;
    virtual void ResetCamera(const GLdouble & deltaTime) = 0;
};

#endif /* ICamera_h */
