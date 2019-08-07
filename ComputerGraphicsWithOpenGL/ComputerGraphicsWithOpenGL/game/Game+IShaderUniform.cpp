//
//  Game+IShaderUniform.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 08/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

/// Shaders Uniforms
void Game::SetTerrainUniform(CShaderProgram *pShaderProgram, const GLboolean &useHeightMap) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseHeightMap", useHeightMap);
    pShaderProgram->SetUniform("fMinHeight", m_heightMapMinHeight);
    pShaderProgram->SetUniform("fMaxHeight", m_heightMapMaxHeight);
    
}

void Game::SetEnvironmentMapUniform(CShaderProgram *pShaderProgram, const GLboolean &useRefraction) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bRefraction", useRefraction);
}

void Game::SetParallaxMapUniform(CShaderProgram *pShaderProgram, const GLfloat &heightScale) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("heightScale", heightScale);
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

void Game::SetFireBallUniform(CShaderProgram *pShaderProgram){
    
    //// https://www.clicktorelease.com/blog/vertex-displacement-noise-3d-webgl-glsl-three-js/
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("time", (float)(m_timeInSeconds * 0.1f));
    pShaderProgram->SetUniform("frequency", 0.8f);
    pShaderProgram->SetUniform("explosion", 0.001f);
}

void Game::SetDisintegrationUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("minThreshold", 0.25f);
    pShaderProgram->SetUniform("maxThreshold", m_parallaxHeightScale / 2.0f);
}

/// Post Processing Uniforms
void Game::SetImageProcessingUniform(CShaderProgram *pShaderProgram, const GLboolean &bUseScreenQuad) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseScreenQuad", bUseScreenQuad);
    /*
    pShaderProgram->SetUniform("time", m_timeInSeconds);
    pShaderProgram->SetUniform("date", m_date);
    pShaderProgram->SetUniform("channelTime", m_channelTime);
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX, m_mouseY));
    pShaderProgram->SetUniform("mouseDown", (int)m_mouseButtonDown);
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
     */
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
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
    
    GLfloat move = m_timeInSeconds * m_screenWaveOffset * 3.14159f * 0.75f;
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("offset", move);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetSwirlUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("radius", m_swirlRadius); // 0 to 360
    pShaderProgram->SetUniform("angle", m_swirlAngle);  //0.8f); 0 to pi
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetNightVisionUniform(CShaderProgram *pShaderProgram) {
    
    m_textures[0]->BindTexture2DToTextureType(); // noiseTex
    m_textures[1]->BindTexture2DToTextureType(); // maskTex
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("elapsedTime", (m_timePerSecond * 0.05f) );
    pShaderProgram->SetUniform("luminanceThreshold", m_nightVisionluminanceThreshold);  // 0.0f  to 1.0f
    pShaderProgram->SetUniform("colorAmplification", m_nightVisionColorAmplification); // 0.0f to 10.0f
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetLensCircleUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("lensColor", glm::vec3(0.2f, 0.6f, 0.7f));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetPosterizationUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("gamma", m_posterizationGama);          // 0 to 1
    pShaderProgram->SetUniform("numColors", m_posterizationColors);      // 0 to 20
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetPixelateUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX, m_mouseY));
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("pixelSize", m_pixelateSize); // 0 to 40
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetPixelationUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("pixel_width", m_pixelWidth); // 0 to 20
    pShaderProgram->SetUniform("pixel_height", m_pixelHeight); // 0 to 20
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}
void Game::SetKnittedPixelationUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetDreamVisionUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFrostedGlassEffectUniform(CShaderProgram *pShaderProgram){
    
    m_textures[2]->BindTexture2DToTextureType(); // noiseTex
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    pShaderProgram->SetUniform("PixelX", m_frostedGlassPixelX); // 4 = 0 to 20
    pShaderProgram->SetUniform("PixelY", m_frostedGlassPixelY); // 4 = 0 to 20
    pShaderProgram->SetUniform("Freq", m_frostedGlassFrequency); // 0.115f = 0.0f to 1.0f
}

void Game::SetFrostedGlassUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("randomFactor", m_frostedGlassRandomFactor); // effect param, default value: 0.05
    pShaderProgram->SetUniform("randomScale", m_frostedGlassRandomScale);  // effect param, default value: 5.1
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetCrosshatchingUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("hatch_y_offset", m_crosshatchingOffset); // between 0 and 10.0f
    pShaderProgram->SetUniform("lum_threshold_1", m_crosshatchingThreshold_1); // between 0 and 1.0f
    pShaderProgram->SetUniform("lum_threshold_2", m_crosshatchingThreshold_2); // between 0 and 1.0f
    pShaderProgram->SetUniform("lum_threshold_3", m_crosshatchingThreshold_3); // between 0 and 1.0f
    pShaderProgram->SetUniform("lum_threshold_4", m_crosshatchingThreshold_4); // between 0 and 1.0f
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1.0f
}

void Game::SetPredatorsThermalVisionUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetToonifyUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("lower_edge_thres", m_toonifyLowerTres); // between 0 and 1
    pShaderProgram->SetUniform("upper_edge_thres", m_toonifyUpperTres); // between 0 and 10
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
    pShaderProgram->SetUniform("shockParams", glm::vec3(10.0f, 0.8f, 0.1f) );
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFishEyeUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("radius", m_fishEyeRadius);         // 0 to 1
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBarrelDistortionUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("barrelPower", m_barrelDistortionPower); // 0 to 10
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetMultiScreenFishEyeUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("xOffset", m_multiScreenFishEyeOffsetX);
    pShaderProgram->SetUniform("yOffset", m_multiScreenFishEyeOffsetY);
    pShaderProgram->SetUniform("lensRadius", m_multiScreenFishEyeRadius);
    pShaderProgram->SetUniform("signcurvature", m_multiScreenFishEyeCurvature);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFishEyeLensUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX, m_mouseY));
    pShaderProgram->SetUniform("lensSize", m_fishEyeLensSize);
    pShaderProgram->SetUniform("lensOutlineColor", glm::vec3(0.2f, 0.4f, 0.6f));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetFishEyeAntiFishEyeUniform(CShaderProgram *pShaderProgram){
    
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX,m_mouseY));
    
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
    pShaderProgram->SetUniform("intensity", m_gaussianBlurIntensity);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBlurUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("blurColor", glm::vec4(1.0f));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetRadialBlurUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("radius", m_radialBlurRadius);
    pShaderProgram->SetUniform("resolution", m_radialBlurResolution);
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
    pShaderProgram->SetUniform("targetFps", m_motionBlurTargetFps); // between 200 and 300
    pShaderProgram->SetUniform("nSamples", GLint(m_motionBlurSamples));
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetDepthMappingUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetVignettingUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bTint", m_vignettingTint);
    pShaderProgram->SetUniform("bUseSepia", m_vignettingSepia);
    pShaderProgram->SetUniform("radius", m_vignettingRadius);
    pShaderProgram->SetUniform("softness", m_vignettingSoftness);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBrightPartsUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bSmoothGradient", m_brightPartsSmoothGradient);
    pShaderProgram->SetUniform("intensity", m_brightPartsIntensity);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBloomUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    // HDR Light
    SetHRDLightUniform(pShaderProgram, "hrdlight", m_exposure, m_gama, m_HDR);
}

void Game::SetHRDToneMappingUniform(CShaderProgram *pShaderProgram){
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
    // HDR Light
    SetHRDLightUniform(pShaderProgram, "hrdlight", m_exposure, m_gama, m_HDR);
}

void Game::SetLensFlareGhostUniform(CShaderProgram *pShaderProgram){
    
    m_textures[3]->BindTexture2DToTextureType();  //lensColor.jpg
    m_textures[4]->BindTexture2DToTextureType();  //lensTexture.jpg

    // http://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUselensTexture", true);
    pShaderProgram->SetUniform("lensType", (int)m_lensFlareNumber);
    pShaderProgram->SetUniform("ghostCount", (int)m_lensFlareGhostCount);            // between 4 and 32
    pShaderProgram->SetUniform("ghostDispersal", m_lensFlareGhostDispersal);     // between 0.0f and  2.0f
    pShaderProgram->SetUniform("ghostThreshold", m_lensFlareGhostThreshold);     // between 0.0f and 20.0f
    pShaderProgram->SetUniform("ghostDistortion", m_lensFlareGhostDistortion);    // between 0 and 1
    pShaderProgram->SetUniform("haloRadius", m_lensFlareHaloRadius);         // between 0.0f and 2.0f
    pShaderProgram->SetUniform("haloThickness", 0.1f);      // between 0.0f and 0.5f
    pShaderProgram->SetUniform("haloThreshold", m_lensFlareHaloThreshold);      // between 0.0f and 20.0f
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
    pShaderProgram->SetUniform("bUseDirt", m_lensFlareUseDirt);
    pShaderProgram->SetUniform("globalBrightness", 0.05000000074505806f);
    pShaderProgram->SetUniform("starburstOffset", starburstOffset);
    pShaderProgram->SetUniform("lensStarMatrix", lensStarMatrix);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1

}

void Game::SetFastApproximateAntiAliasingUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("spanOffset", m_ffaaOffset);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetDeferredRenderingUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetScreenSpaceAmbientOcclusionUniform(CShaderProgram *pShaderProgram) {
    m_textures[7]->BindCustomTexture2DToTextureType();        // bind SSAO noise texture

    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("projection", *m_pCamera->GetPerspectiveProjectionMatrix());
    pShaderProgram->SetUniform("radius", m_ssaoRadius);
    pShaderProgram->SetUniform("bias", m_ssaoBias);
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("noiseSize", m_ssaoNoiseSize);
    
    // Send kernel + rotation
    for (GLuint i = 0; i < m_ssaoKernelSamples; ++i) {
        pShaderProgram->SetUniform("samples[" + std::to_string(i) + "]", m_ssaoKernel[i]);
    }
}

void Game::SetScreenSpaceAmbientOcclusionBlurUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("noiseSize", m_ssaoNoiseSize);
}

void Game::SetScreenSpaceAmbientOcclusionLightingUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("bUseLight", m_ssaoNoiseUseLight);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetRainDropsUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("time", m_timeInSeconds);
    pShaderProgram->SetUniform("channelTime", m_channelTime);
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX, m_mouseY));
    pShaderProgram->SetUniform("mouseDown", (int)m_mouseButtonDown);
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetPaletteQuantizationAndDitheringUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX, m_mouseY));
    pShaderProgram->SetUniform("date", m_date);
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetDistortedTVUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("time", m_timeInSeconds);
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetRGBDisplayUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetRetroParallaxUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("time", m_timeInSeconds);
    pShaderProgram->SetUniform("pixelSize", m_pixelWidth); // 0 to 20
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetScaryRetroParallaxUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("time", m_timeInSeconds);
    pShaderProgram->SetUniform("channelTime", m_channelTime);
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetMoneyFilterUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX, m_mouseY));
    pShaderProgram->SetUniform("mouseDown", (int)m_mouseButtonDown);
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetMicroprismMosaicUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("time", m_timeInSeconds);
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX, m_mouseY));
    pShaderProgram->SetUniform("mouseDown", (int)m_mouseButtonDown);
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetBayerMatrixDitheringUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetJuliaFreakUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetHeartBlendUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetEMInterferenceUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("time", m_timeInSeconds);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetCubicLensDistortionUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("time", m_timeInSeconds);
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetCelShaderishUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

void Game::SetCartoonVideoUniform(CShaderProgram *pShaderProgram) {
    pShaderProgram->UseProgram();
    pShaderProgram->SetUniform("mouse", glm::vec2(m_mouseX, m_mouseY));
    pShaderProgram->SetUniform("width", (float)m_gameWindow->GetWidth());
    pShaderProgram->SetUniform("height", (float)m_gameWindow->GetHeight());
    pShaderProgram->SetUniform("coverage", m_coverage); // between 0 and 1
}

