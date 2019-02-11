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
    pShaderProgram->SetUniform(uniformName+".ambientMap", 0);           // ambient map (albedo map)
    pShaderProgram->SetUniform(uniformName+".diffuseMap", 1);           // diffuse map (metallic map)
    pShaderProgram->SetUniform(uniformName+".specularMap", 2);          // specular map (roughness map)
    pShaderProgram->SetUniform(uniformName+".normalMap", 3);            // normal map
    pShaderProgram->SetUniform(uniformName+".heightMap", 4);            // height map
    pShaderProgram->SetUniform(uniformName+".emissionMap", 5);          // emission map
    pShaderProgram->SetUniform(uniformName+".displacementMap", 6);      // displacement map
    pShaderProgram->SetUniform(uniformName+".aoMap", 7);                // ambient oclusion map
    pShaderProgram->SetUniform(uniformName+".glossinessMap", 8);        // glossiness/shininess map
    pShaderProgram->SetUniform(uniformName+".opacityMap", 9);           // opacity map
    pShaderProgram->SetUniform(uniformName+".reflectionMap", 10);       // reflection map
    pShaderProgram->SetUniform(uniformName+".cubeMap", 11);             // sky box or environment mapping cube map
    pShaderProgram->SetUniform(uniformName+".color", color);
    pShaderProgram->SetUniform(uniformName+".shininess", shininess);
}
