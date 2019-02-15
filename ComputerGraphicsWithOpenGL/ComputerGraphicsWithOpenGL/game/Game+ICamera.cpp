//
//  Game+ICamera.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 & position){
    
    // Set the orthographic and perspective projection matrices based on the image size
    m_pCamera->Create(position,                // position
                      glm::vec3(0.0f, 1.0f, 0.0f),       // worldUp
                      PITCH,                            // pitch
                      YAW,                              // yaw
                      FOV,                              // fieldOfView
                      (GLfloat)width,                   // width
                      (GLfloat)height,                  // height
                      ZNEAR,                            // zNear
                      ZFAR                              // zFar
                      );
    
    glm::vec3 pos = position;
    glm::vec3 view = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upV = glm::vec3(0.0f, 1.0f, 0.0f);
    m_pCamera->Set(pos, view, upV);
    
}

void Game::SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName+".position", camera->GetPosition());
    pShaderProgram->SetUniform(uniformName+".front", camera->GetForward());
}

void Game::UpdateCamera(const GLdouble & deltaTime, const GLuint & keyPressed, const GLboolean & mouseMove) {
    // Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
    m_pCamera->Update(m_gameWindow->GetWindow(), deltaTime, keyPressed, true, mouseMove);
}

void Game::UpdateCameraEndFrame(const GLdouble & deltaTime) {
    m_pCamera->UpdateEndFrame(m_gameWindow->GetWindow(), deltaTime);
}
