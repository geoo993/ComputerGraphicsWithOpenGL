//
//  Game+IMaterials.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 03/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                              const glm::vec4 &color, const GLfloat &shininess,
                              const GLfloat &uvTiling, const GLboolean &useAO, const glm::vec4 &guiColor) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName+".ambientMap", 0);           // ambient map
    pShaderProgram->SetUniform(uniformName+".diffuseMap", 1);           // diffuse map
    pShaderProgram->SetUniform(uniformName+".specularMap", 2);          // specular map
    pShaderProgram->SetUniform(uniformName+".normalMap", 3);            // normal map
    pShaderProgram->SetUniform(uniformName+".heightMap", 4);            // height map
    pShaderProgram->SetUniform(uniformName+".emissionMap", 5);          // emission map
    pShaderProgram->SetUniform(uniformName+".displacementMap", 6);      // displacement map
    pShaderProgram->SetUniform(uniformName+".aoMap", 7);                // ambient oclusion map
    pShaderProgram->SetUniform(uniformName+".glossinessMap", 8);        // glossiness/shininess map
    pShaderProgram->SetUniform(uniformName+".opacityMap", 9);           // opacity map
    pShaderProgram->SetUniform(uniformName+".shadowMap", 10);           // shadow cube map
    pShaderProgram->SetUniform(uniformName+".depthMap", 11);            // depth map
    pShaderProgram->SetUniform(uniformName+".noiseMap", 12);            // noise map
    pShaderProgram->SetUniform(uniformName+".maskMap", 13);             // mask map
    pShaderProgram->SetUniform(uniformName+".lensMap", 14);             // lens map
    pShaderProgram->SetUniform(uniformName+".albedoMap", 15);           // albedo map
    pShaderProgram->SetUniform(uniformName+".metallicMap", 16);        // metallic map
    pShaderProgram->SetUniform(uniformName+".roughnessMap", 17);        // roughness, smoothness map
    pShaderProgram->SetUniform(uniformName+".cubeMap", 18);             // sky box cube map
    pShaderProgram->SetUniform(uniformName+".irradianceMap", 19);       // sky box irradiance cube map
    pShaderProgram->SetUniform(uniformName+".color", color);
    pShaderProgram->SetUniform(uniformName+".guiColor", guiColor);
    pShaderProgram->SetUniform(uniformName+".shininess", shininess);
    pShaderProgram->SetUniform(uniformName+".uvTiling", uvTiling);
    pShaderProgram->SetUniform(uniformName+".bUseAO", useAO);
    pShaderProgram->SetUniform(uniformName+".bUseTexture", m_materialUseTexture);
    pShaderProgram->SetUniform(uniformName+".bUseColor", m_materialUseColor);
}

void Game::SetPBRMaterialUniform(CShaderProgram *pShaderProgram,  const std::string &uniformName,
                                 const GLfloat &albedo, const GLfloat &metallic, const GLfloat &roughness,
                                 const GLfloat &ao, const GLboolean &useIrradiance) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseIrradiance", useIrradiance);
    pShaderProgram->SetUniform(uniformName+".albedo", albedo);
    pShaderProgram->SetUniform(uniformName+".metallic", metallic);
    pShaderProgram->SetUniform(uniformName+".roughness", roughness);
    pShaderProgram->SetUniform(uniformName+".ao", ao);
}

void Game::SetFogMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                            const glm::vec3 &color, const GLboolean &bUseFog) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName+".bUseFog", bUseFog);
    pShaderProgram->SetUniform(uniformName+".minDist", 1.0f);
    pShaderProgram->SetUniform(uniformName+".maxDist", m_mapSize / 2.0f);
    pShaderProgram->SetUniform(uniformName+".color", color);
    
}
