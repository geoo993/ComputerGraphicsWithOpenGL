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
    pShaderProgram->SetUniform("width", m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow->GetHeight());
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
    pShaderProgram->SetUniform("width", m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow->GetHeight());
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
    pShaderProgram->SetUniform("width", m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    pShaderProgram->SetUniform("PixelX", 4.0f);
    pShaderProgram->SetUniform("PixelY", 4.0f);
    pShaderProgram->SetUniform("Freq", 0.115f);
}

void Game::SetFrostedGlassUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetCrosshatchingUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow->GetHeight());
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

void Game::SetShockwaveUniform(CShaderProgram *pShaderProgram) {
    
    float mX = (float)(m_mouseX / m_gameWindow->GetWidth());
    float mY = (float)(m_mouseY / m_gameWindow->GetHeight() );
    glm::vec2 center = glm::vec2(mX, (1.0f - mY) ); // or center screen with glm::vec2(0.5f, 0.5f));
    if (m_mouseButtonDown) {
        m_shockWaveTime = 0.0f;
    }
    m_shockWaveTime += m_timePerSecond;

    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("center", center);
    pShaderProgram->SetUniform("time", m_shockWaveTime);
    pShaderProgram->SetUniform("shockParams", glm::vec3(10.0f, 0.8f, 0.1f ) );
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
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
    pShaderProgram->SetUniform("xOffset", 0.5f);
    pShaderProgram->SetUniform("yOffset", 0.5f);
    pShaderProgram->SetUniform("lensRadius", 3.5f);
    pShaderProgram->SetUniform("signcurvature", 10.0f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFishEyeLensUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX,m_mouseY));
    pShaderProgram->SetUniform("lensSize", 0.35f);
    pShaderProgram->SetUniform("lensOutlineColor", glm::vec3(0.2f, 0.4f, 0.6f));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFishEyeAntiFishEyeUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
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
    pShaderProgram->SetUniform("intensity", 1.0f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBlurUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", m_gameWindow->GetHeight());
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
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = m_pCamera->GetViewMatrix();
    glm::mat4 modelViewMatrix = view * model;
    glm::mat4 projMatrix = *(m_pCamera->GetPerspectiveProjectionMatrix());
    glm::mat4 inverseProj = glm::inverse(projMatrix);
    glm::mat4 inverseModelView = glm::inverse(modelViewMatrix);
    
    m_pCamera->SetModelMatrix(model); // set the model matrix so that we can have a previous MVP matrix
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("inverseModelView", inverseModelView);
    pShaderProgram->SetUniform("inverseProjection", inverseProj);
    pShaderProgram->SetUniform("prevMVP", m_pCamera->GetPreviousMVP()); // previous model>view>projection
    pShaderProgram->SetUniform("currentFps", (float)m_framesPerSecond);
    pShaderProgram->SetUniform("targetFps", 200.0f); // between 200 and 300
    pShaderProgram->SetUniform("nSamples", 11);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
   
}

void Game::SetDepthMappingUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bLinearized", true);
    pShaderProgram->SetUniform("near_plane", m_pCamera->GetNearPlane()); // shadows near plane
    pShaderProgram->SetUniform("far_plane", m_pCamera->GetFarPlane());   // shadows far plane
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    
}

void Game::SetVignettingUniform(CShaderProgram *pShaderProgram){
    bool tint = true;
    bool useGray = (tint==true) ? false : false ;
    bool useSepia = (tint==true) ? !useGray : false ;
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bTint", tint);
    pShaderProgram->SetUniform("bUseGray", useGray);
    pShaderProgram->SetUniform("bUseSepia", useSepia);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBrightPartsUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bSmoothGradient", true);
    pShaderProgram->SetUniform("intensity", 1.0f);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBloomUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bHDR", m_HDR);
    pShaderProgram->SetUniform("exposure", m_exposure);
    pShaderProgram->SetUniform("gamma", m_gama);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetHRDToneMappingUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bHDR", m_HDR);
    pShaderProgram->SetUniform("exposure", m_exposure);
    pShaderProgram->SetUniform("gamma", m_gama);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetLensFlareGhostUniform(CShaderProgram *pShaderProgram){
    
    m_textures[3]->BindTexture2DToTextureType();  //lensColor.jpg
    m_textures[4]->BindTexture2DToTextureType();  //lensTexture.jpg

    // http://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUselensTexture", true);
    pShaderProgram->SetUniform("lensType", 3);
    pShaderProgram->SetUniform("ghostCount", 5);            // between 4 and 32
    pShaderProgram->SetUniform("ghostDispersal", 0.39f);     // between 0.0f and  2.0f
    pShaderProgram->SetUniform("ghostThreshold", 10.0f);     // between 0.0f and 20.0f
    pShaderProgram->SetUniform("ghostDistortion", 4.3f);    // between 0 and 1
    pShaderProgram->SetUniform("haloRadius", 0.3f);         // between 0.0f and 2.0f
    pShaderProgram->SetUniform("haloThickness", 0.1f);      // between 0.0f and 0.5f
    pShaderProgram->SetUniform("haloThreshold", 9.0f);      // between 0.0f and 20.0f
    pShaderProgram->SetUniform("haloAspectRatio", 1.0f);    // between 0.0f and 2.0f
}

void Game::SetLensFlareUniform(CShaderProgram *pShaderProgram){
    m_textures[5]->BindTexture2DToTextureType();  //lensDirt.jpg
    m_textures[6]->BindTexture2DToTextureType();  //lensStarburst.jpg
    
    // http://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
    glm::vec3 viewVec = m_pCamera->GetView();
    float starburstOffset = viewVec.x + viewVec.y + viewVec.z;
    
    /*
     The transformation matrix uLensStarMatrix is based on a value derived from the camera's orientation as follows:
     */
    glm::vec4 camx = m_pCamera->GetViewMatrix()[0]; // camera x (left) vector
    glm::vec4 camz = m_pCamera->GetViewMatrix()[1]; // camera z (forward) vector
    float camrot = dot(glm::vec3(camx), glm::vec3(0.0f, 0.0f, 1.0f)) + dot(glm::vec3(camz), glm::vec3(0.0f, 1.0f, 0.0f));
    /*
    There are other ways of obtaining the camrot value; it just needs to change continuously as the camera rotates.
    The matrix itself is constructed as follows:
    */
    glm::mat3 scaleBias1 = glm::mat3(
                       2.0f,   0.0f,  -1.0f,
                       0.0f,   2.0f,  -1.0f,
                       0.0f,   0.0f,   1.0f
                       );
    glm::mat3 rotation = glm::mat3(
                     cos(camrot), -sin(camrot), 0.0f,
                     sin(camrot), cos(camrot),  0.0f,
                     0.0f,        0.0f,         1.0f
                     );
    glm::mat3 scaleBias2 = glm::mat3(
                       0.5f,   0.0f,   0.5f,
                       0.0f,   0.5f,   0.5f,
                       0.0f,   0.0f,   1.0f
                       );
    
    glm::mat3 lensStarMatrix = scaleBias2 * rotation * scaleBias1;
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseDirt", m_HDR);
    pShaderProgram->SetUniform("globalBrightness", 0.05000000074505806f);
    pShaderProgram->SetUniform("starburstOffset", starburstOffset);
    pShaderProgram->SetUniform("lensStarMatrix", lensStarMatrix);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1

}
