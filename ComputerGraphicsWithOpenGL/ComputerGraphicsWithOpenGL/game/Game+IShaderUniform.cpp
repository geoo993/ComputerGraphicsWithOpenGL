//
//  Game+IShaderUniform.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 08/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

/// Shaders Uniforms
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



/// Post Processing Uniforms
void Game::SetImageProcessingUniform(CShaderProgram *pShaderProgram, const GLboolean &bUseScreenQuad) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseScreenQuad", bUseScreenQuad);
}

void Game::SetColorInversionUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetGrayScaleUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetKernelUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetKernelBlurUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetEdgeDetectionUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetScreenWaveUniform(CShaderProgram *pShaderProgram){
    
    //float offset = (float)m_time / 1000.0f * 2.0f * 3.14159f * 0.75f;
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("offset", (m_timePerSecond * 0.1f));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetSwirlUniform(CShaderProgram *pShaderProgram){
    
    float time = (float)m_elapsedTime / 1000.0f * 2.0f * 3.14159f * 0.75f;
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow.GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow.GetHeight());
    pShaderProgram->SetUniform("offset", time);
    pShaderProgram->SetUniform("radius", 350.0f);
    pShaderProgram->SetUniform("angle", 0.8f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetNightVisionUniform(CShaderProgram *pShaderProgram) {
    
    m_textures[0]->BindTexture2DToTextureType(); // noiseTex
    m_textures[1]->BindTexture2DToTextureType(); // maskTex
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("elapsedTime", (m_timePerSecond * 0.05f) );
    pShaderProgram->SetUniform("luminanceThreshold", 0.2f);
    pShaderProgram->SetUniform("colorAmplification", 4.0f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    
}

void Game::SetLensCircleUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("lensColor", glm::vec3(0.2f, 0.6f, 0.7f));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetPosterizationUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("gamma", 0.6f);
    pShaderProgram->SetUniform("numColors", 8.0f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetPixelationUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow.GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow.GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    pShaderProgram->SetUniform("pixel_width", 15.0f);
    pShaderProgram->SetUniform("pixel_height", 10.0f);
}

void Game::SetDreamVisionUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFrostedGlassEffectUniform(CShaderProgram *pShaderProgram){
    
    m_textures[2]->BindTexture2DToTextureType(); // noiseTex
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow.GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow.GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    pShaderProgram->SetUniform("PixelX", 4.0f);
    pShaderProgram->SetUniform("PixelY", 4.0f);
    pShaderProgram->SetUniform("Freq", 0.115f);
}

void Game::SetFrostedGlassUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow.GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow.GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetCrosshatchingUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow.GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow.GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetPredatorsThermalVisionUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetToonifyUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetShockwaveUniform(CShaderProgram *pShaderProgram){
    
    float mX = (float)(m_mouseX / m_gameWindow.GetWidth());
    float mY = (float)(m_mouseY / m_gameWindow.GetHeight() );
    if (m_mouseButtonDown) {
        //std::cout << "shockwave time: " << m_shockWaveTime << ", Mouse x: " << mX << " and mouse y: " << mY << std::endl;
        m_shockWaveTime = 0.0f;
    }
    m_shockWaveTime += 0.001f;//m_deltaTime;
    
    glm::vec2 center(mX, (1.0f - mY) ); // or center screen with glm::vec2(0.5f, 0.5f));
    
    pShaderProgram->SetUniform("center", center);
    pShaderProgram->SetUniform("time", m_shockWaveTime);
    pShaderProgram->SetUniform("shockParams", glm::vec3(10.0f, 0.8f, 0.1f ) );
}

void Game::SetFishEyeUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("radius", 0.9f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBarrelDistortionUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("barrelPower", 2.0f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetMultiScreenFishEyeUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("barrelPower", 2.0f);
    pShaderProgram->SetUniform("wireframe", 0);
    pShaderProgram->SetUniform("xOffset", 0.5f);
    pShaderProgram->SetUniform("yOffset", 0.5f);
    pShaderProgram->SetUniform("lensRadius", 3.5f);
    pShaderProgram->SetUniform("signcurvature", 10.0f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFishEyeLensUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow.GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow.GetHeight());
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX,m_mouseY));
    pShaderProgram->SetUniform("lensSize", 0.35f);
    pShaderProgram->SetUniform("lensOutlineColor", glm::vec3(0.2f, 0.4f, 0.6f));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFishEyeAntiFishEyeUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow.GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow.GetHeight());
    pShaderProgram->SetUniform("iMouse", glm::vec2(m_mouseX,m_mouseY));
    
    // only one of four must be true
    pShaderProgram->SetUniform("isSixScreens", false);
    pShaderProgram->SetUniform("isFourScreens", false);
    pShaderProgram->SetUniform("isTwoScreens", false);
    pShaderProgram->SetUniform("isOneScreen", true);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    
}

void Game::SetGaussianBlurUniform(CShaderProgram *pShaderProgram, const GLboolean &horizontal){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bHorizontal", horizontal);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBlurUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow.GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow.GetHeight());
    pShaderProgram->SetUniform("blurColor", glm::vec4(1.0f));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetRadialBlurUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("radius", 0.4f);
    pShaderProgram->SetUniform("resolution", 2.0f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetMotionBlurUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("emptySampler0", 6);
    pShaderProgram->SetUniform("invMVP", m_pCamera->GetInverseMVP()); // inverse model>view>projection
    pShaderProgram->SetUniform("prevMVP", m_pCamera->GetPreviousMVP()); // previous model>view>projection
    
    pShaderProgram->SetUniform("currentFPS", (float)m_framesPerSecond);
    pShaderProgram->SetUniform("targetFPS", 50000.0f);
    
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
   
}

void Game::SetVignettingUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBrightPartsUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBloomUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetLensFlareUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}
