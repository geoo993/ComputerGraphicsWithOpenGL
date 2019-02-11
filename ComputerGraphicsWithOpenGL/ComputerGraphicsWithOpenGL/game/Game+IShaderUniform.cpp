//
//  Game+IShaderUniform.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 08/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::SetPBRMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                 const glm::vec3 &albedo, const GLfloat &metallic, const GLfloat &roughness) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform(uniformName+".albedo", albedo);
    pShaderProgram->SetUniform(uniformName+".metallic", metallic);
    pShaderProgram->SetUniform(uniformName+".roughness", roughness);
    pShaderProgram->SetUniform(uniformName+".fresnel", 0.6f);
    pShaderProgram->SetUniform(uniformName+".ao", 1.0f);
    
}

void Game::SetEnvironmentMapUniform(CShaderProgram *pShaderProgram, const GLboolean &useRefraction) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bRefraction", useRefraction);
}

void Game::SetParallaxMapUniform(CShaderProgram *pShaderProgram, const GLfloat &heightScale) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("heightScale", heightScale);
}

void Game::SetBumpMapUniform(CShaderProgram *pShaderProgram, const GLfloat &uvTiling) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("uvTiling", uvTiling);
}

void Game::SetExplosionUniform(CShaderProgram *pShaderProgram,
                         const GLboolean &explode, const GLboolean &animate,
                         const GLfloat &time, const GLfloat &magnitude) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bExplode", explode);
    pShaderProgram->SetUniform("bAnimate", animate);
    pShaderProgram->SetUniform("time", time);
    pShaderProgram->SetUniform("magnitude", magnitude);
}

void Game::SetPorcupineRenderingUniform(CShaderProgram *pShaderProgram,
                                        const glm::vec3 &vertexNormalColor, const glm::vec3 &faceNormalColor,
                                        const GLfloat &magnitude) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("vertexNormalColor", vertexNormalColor);
    pShaderProgram->SetUniform("faceNormalColor", faceNormalColor);
    pShaderProgram->SetUniform("magnitude", magnitude);
}

void Game::SetWireframeUniform(CShaderProgram *pShaderProgram, const GLboolean &useWireframe, const GLfloat &thickness) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bWireFrame", useWireframe);
    pShaderProgram->SetUniform("thickness", thickness);
}

void Game::SetChromaticAberrationUniform(CShaderProgram *pShaderProgram, const glm::vec2 &fresnelValues) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("fresnelValues", fresnelValues);
    pShaderProgram->SetUniform("IoR_Values", glm::vec3(0.4f, 0.8f, 1.3f));
}
