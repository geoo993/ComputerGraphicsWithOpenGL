//
//  Game+ICamera.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 & position){
    m_viewPointAngle = 0.0f;
    
    // Set the orthographic and perspective projection matrices based on the image size
    m_pCamera->Create(position,                // position
                      glm::vec3(0.0f, 0.0f, -1.0f),     // front
                      glm::vec3(0.0f, 1.0f, 0.0f),      // up
                      glm::vec3(0.0f, 1.0f, 0.0f),      // worldUp
                      (GLfloat)FOV,                     // fieldOfView
                      (GLfloat)width,                   // width
                      (GLfloat)height,                  // height
                      (GLfloat)ZNEAR,                   // zNear
                      (GLfloat)ZFAR                     // zFar
                      );
}

void Game::SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName+".position", camera->GetPosition());
    pShaderProgram->SetUniform(uniformName+".front", camera->GetForward());
    pShaderProgram->SetUniform(uniformName+".znear", camera->GetNearPlane());
    pShaderProgram->SetUniform(uniformName+".zfar", camera->GetFarPlane());
    pShaderProgram->SetUniform(uniformName+".isOrthographic", m_isOrthographicCamera);
    pShaderProgram->SetUniform(uniformName+".isMoving", camera->IsMoving());
}

void Game::UpdateCamera(const GLdouble & deltaTime, const MouseState &mouseState, const KeyboardState &keyboardState, const GLboolean & mouseMove) {
    m_viewPointAngle ++;
    // Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
    m_pCamera->Update(m_gameWindow->GetWindow(), deltaTime, mouseState, keyboardState, true);
    //m_pCamera->RotateAroundPoint(50.0f, glm::vec3(0, 50, 50), m_viewPointAngle, 100.0f);
}

void Game::ResetCamera(const GLdouble & deltaTime) {
    m_pCamera->Reset(m_gameWindow->GetWindow(), deltaTime);
}
