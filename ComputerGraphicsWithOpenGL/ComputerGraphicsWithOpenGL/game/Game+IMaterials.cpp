//
//  Game+IMaterials.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 03/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                              const glm::vec3 &color, const GLfloat &shininess) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName+".ambientMap", 0);
    pShaderProgram->SetUniform(uniformName+".normalMap", 1);
    pShaderProgram->SetUniform(uniformName+".diffuseMap", 2);
    pShaderProgram->SetUniform(uniformName+".specularMap", 3);
    pShaderProgram->SetUniform(uniformName+".color", color);
    pShaderProgram->SetUniform(uniformName+".shininess", shininess);
    
}
