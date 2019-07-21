//
//  Interfaces.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef Interfaces_h
#define Interfaces_h

#include "GameWindow.h"
#include "GameManager.h"
#include "Camera.h"
#include "HighResolutionTimer.h"
#include "FreeTypeFont.h"
#include "Audio.h"
#include "PostProcessingEffectMode.h"
#include "Lighting.h"
#include "FrameBufferObject.h"
#include "Cube.h"
#include "Sphere.h"

struct IGameWindow
{
    
    CGameWindow *m_gameWindow;
    CGameManager *m_gameManager;
    virtual void InitialiseGameWindow(const std::string &name, const std::string &filepath,
                                      const GLuint &width, const GLuint &height) = 0;
};

struct IResources {
    virtual void InitialiseResources() = 0;
    virtual void LoadResources(const std::string &path) = 0;
};

struct IGameTimer
{
    CHighResolutionTimer *m_pGameTimer;
    glm::vec4 m_date;
    GLfloat m_timeInSeconds, m_timeInMilliSeconds, m_timePerSecond, m_channelTime;
    GLdouble m_deltaTime, m_elapsedTime;
    GLint m_framesPerSecond, m_frameCount;
    virtual void UpdateSystemTime() = 0;
    virtual void UpdateGameTime() = 0;
};

struct IAudio {
    CAudio *m_pAudio;
    GLfloat m_audioNumber;
    GLboolean m_changeAudio;
    std::vector <std::string> m_audioFiles;
    virtual void InitialiseAudio(const std::string &path) = 0;
    virtual void UpdateAudio() = 0;
};

struct ICamera {
    CCamera *m_pCamera;
    virtual void InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 &position) = 0;
    virtual void SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) = 0;
    virtual void UpdateCamera(const GLdouble & deltaTime, const MouseState &mouseState, const KeyboardState &keyboardState,
                            const GLboolean & mouseMove) = 0;
    virtual void ResetCamera(const GLdouble & deltaTime) = 0;
};

struct IMaterials {
    glm::vec4 m_materialColor;
    GLfloat m_materialShininess, m_albedo, m_metallic, m_roughness, m_ao;
    GLboolean m_useIrradianceMap, m_materialUseTexture, m_materialUseColor, m_useIrradiance;
    GLboolean m_useFog;
    glm::vec3 m_fogColor;
    virtual void SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                    const glm::vec4 &color, const GLfloat &shininess,
                                    const GLfloat &uvTiling, const GLboolean &useAO) = 0;
    virtual void SetPBRMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                       const GLfloat &albedo, const GLfloat &metallic, const GLfloat &roughness,
                                       const GLfloat &ao, const GLboolean &useIrradiance) = 0;
    virtual void SetFogMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                        const glm::vec3 &color, const GLboolean &bUseFog) = 0;
};

struct ITextures {
    std::vector<CTexture*> m_textures;
    virtual void LoadTextures(const std::string &path) = 0;
    virtual CTexture *AddTexture(const std::string &textureFile, const TextureType &type, const bool &gammaCorrection) = 0;
    virtual CTexture *AddHDRTexture(const std::string &textureFile, const TextureType &type) = 0;
    virtual CTexture *AddTexture(const GLfloat &width, const GLfloat &height, const TextureType &type, const GLvoid * data) = 0;
};

struct IShaders {
    std::vector <CShaderProgram *> *m_pShaderPrograms;
    virtual void LoadShaderPrograms(const std::string &path) = 0;
};

struct IShaderUniform {
    GLboolean m_useRefraction;
    GLfloat m_parallaxHeightScale, m_uvTiling, m_magnitude;
    virtual void SetTerrainUniform(CShaderProgram *pShaderProgram, const GLboolean &useHeightMap) = 0;
    virtual void SetEnvironmentMapUniform(CShaderProgram *pShaderProgram, const GLboolean &useRefraction) = 0;
    virtual void SetParallaxMapUniform(CShaderProgram *pShaderProgram, const GLfloat &heightScale) = 0;
    virtual void SetExplosionUniform(CShaderProgram *pShaderProgram,
                                     const GLboolean &explode, const GLboolean &animate,
                                     const GLfloat &time, const GLfloat &magnitude) = 0;
    virtual void SetPorcupineRenderingUniform(CShaderProgram *pShaderProgram,
                                              const glm::vec3 &vertexNormalColor, const glm::vec3 &faceNormalColor,
                                              const GLfloat &magnitude) = 0;
    virtual void SetWireframeUniform(CShaderProgram *pShaderProgram, const GLboolean &useWireframe, const GLfloat &thickness) = 0;
    virtual void SetChromaticAberrationUniform(CShaderProgram *pShaderProgram, const glm::vec2 &fresnelValues) = 0;
    virtual void SetFireBallUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetDisintegrationUniform(CShaderProgram *pShaderProgram) = 0;
    
    ///Post Processing Uniform
    virtual void SetImageProcessingUniform(CShaderProgram *pShaderProgram, const GLboolean &bUseScreenQuad) = 0;
    virtual void SetColorInversionUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetGrayScaleUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetKernelUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetKernelBlurUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetEdgeDetectionUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetScreenWaveUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetSwirlUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetNightVisionUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetLensCircleUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetPosterizationUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetDreamVisionUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetPixelateUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetPixelationUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetKnittedPixelationUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetFrostedGlassEffectUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetFrostedGlassUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetCrosshatchingUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetPredatorsThermalVisionUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetToonifyUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetShockwaveUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetFishEyeUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetBarrelDistortionUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetMultiScreenFishEyeUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetFishEyeLensUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetFishEyeAntiFishEyeUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetGaussianBlurUniform(CShaderProgram *pShaderProgram, const GLboolean &horizontal) = 0;
    virtual void SetBlurUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetRadialBlurUniform(CShaderProgram *pShaderProgram) =0;
    virtual void SetMotionBlurUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetDepthMappingUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetVignettingUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetBrightPartsUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetBloomUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetHRDToneMappingUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetLensFlareGhostUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetLensFlareUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetFastApproximateAntiAliasingUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetDeferredRenderingUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetScreenSpaceAmbientOcclusionUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetScreenSpaceAmbientOcclusionBlurUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetScreenSpaceAmbientOcclusionLightingUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetRainDropsUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetPaletteQuantizationAndDitheringUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetDistortedTVUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetRGBDisplayUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetRetroParallaxUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetScaryRetroParallaxUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetMoneyFilterUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetMicroprismMosaicUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetBayerMatrixDitheringUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetJuliaFreakUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetHeartBlendUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetEMInterferenceUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetCubicLensDistortionUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetCelShaderishUniform(CShaderProgram *pShaderProgram) = 0;
    virtual void SetCartoonVideoUniform(CShaderProgram *pShaderProgram) = 0;
};

struct ILights
{
  
    // Light
    GLfloat m_ambient;
    GLfloat m_diffuse;
    GLfloat m_specular;
    GLfloat m_exposure;
    GLfloat m_gama;
    GLboolean m_useBlinn, m_HDR;
    
    // Attenuation
    GLfloat m_constant;
    GLfloat m_linear;
    GLfloat m_exponent;
    
    // Directional Light
    GLboolean m_useDir;
    glm::vec3 m_dirColor;
    GLfloat m_dirIntensity;
    glm::vec3 m_directionalLightDirection;
    
    // Point Light
    GLboolean m_usePoint;
    GLfloat m_pointIntensity;
    GLuint m_pointLightIndex = 0;
    std::vector<std::tuple<glm::vec3, glm::vec4>> m_pointLights;
    
    // Spot Light
    GLboolean m_useSpot;
    GLboolean m_useSmoothSpot;
    glm::vec3 m_spotColor;
    GLfloat m_spotIntensity;
    GLfloat m_spotCutOff;
    GLfloat m_spotOuterCutOff;
    
    // Uniform
    virtual void SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
                                 const GLboolean &useSpot, const GLboolean &useSmoothSpot, const GLboolean& useBlinn) = 0;
    virtual void SetHRDLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                    const GLfloat & exposure, const GLfloat & gamma, const GLboolean &useHDR) = 0;
    virtual void SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                            const DirectionalLight& directionalLight, const glm::vec3& direction) = 0;
    virtual void SetPointLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const PointLight& pointLight) = 0;
    virtual void SetSpotLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const SpotLight& spotLight,
                                     CCamera *camera) = 0;
    virtual void SetShadowUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const GLfloat &znear, const GLfloat &zfar) = 0;
    virtual void RenderLight(CShaderProgram *pShaderProgram, CCamera * camera) = 0;
    virtual void RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale) = 0;
};

struct IRenderer
{
    virtual void PreRendering() = 0;
    virtual void Render() = 0;
    virtual void PostRendering() = 0;
    virtual void RenderScene(const GLboolean &toCustomShader, const GLboolean &includeLampsAndSkybox, const GLint &toCustomShaderIndex) = 0;
};

struct IRenderObject
{
    virtual void RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                            const GLfloat & scale, const GLboolean &bindTexture) = 0;
    virtual void RenderSkyBox(CShaderProgram *pShaderProgram) = 0;
    virtual void RenderEnvSkyBox(CShaderProgram *pShaderProgram) = 0;
    virtual void ResetSkyBox(CShaderProgram *pShaderProgram) = 0;
    virtual void RenderTerrain(CShaderProgram *pShaderProgram, const glm::vec3 & position, const glm::vec3 & rotation, const GLfloat & scale, const GLboolean &useHeightMap) = 0;
    virtual void RenderPrimitive(CShaderProgram *pShaderProgram, GameObject *object, const glm::vec3 & position,
                                const glm::vec3 & rotation, const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderMetalBalls(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                  const GLfloat & scale, const GLboolean &useTexture) = 0;
};

struct IPostProcessing {
    PostProcessingEffectMode m_currentPPFXMode;
    CFrameBufferObject *currentFBO;
    std::vector<CFrameBufferObject*> m_pFBOs;
    GLboolean m_changePPFXMode, m_prevPPFXMode, m_nextPPFXMode;
    GLuint m_PPFXOption;
    GLfloat m_coverage;
    
    // Screen Wave
    GLfloat m_screenWaveOffset;
    
    // Swirl
    GLfloat m_swirlOffset, m_swirlRadius, m_swirlAngle;
    
    //Night Vision
    GLfloat m_nightVisionluminanceThreshold, m_nightVisionColorAmplification;
    
    // Posterization
    GLfloat m_posterizationGama, m_posterizationColors;
    
    // Pixelate
    GLfloat m_pixelateSize;

    // Pixelation
    GLfloat m_pixelWidth, m_pixelHeight;
    
    // Frosted Glass Effect
    GLfloat m_frostedGlassPixelX, m_frostedGlassPixelY, m_frostedGlassFrequency;
    
    // Frosted Glass
    GLfloat m_frostedGlassRandomFactor, m_frostedGlassRandomScale;
    
    // Crosshatching
    GLfloat m_crosshatchingOffset;
    GLfloat m_crosshatchingThreshold_1, m_crosshatchingThreshold_2, m_crosshatchingThreshold_3, m_crosshatchingThreshold_4;
    
    // Toonify
    GLfloat m_toonifyLowerTres, m_toonifyUpperTres;
    
    //shockwave
    GLfloat m_shockWaveTime;
    
    // FishEye
    GLfloat m_fishEyeRadius;
    
    // Barrel Distortion
    GLfloat m_barrelDistortionPower;
    
    // MultiScreenFishEye
    GLfloat m_multiScreenFishEyeOffsetX, m_multiScreenFishEyeOffsetY, m_multiScreenFishEyeRadius, m_multiScreenFishEyeCurvature;
    
    // Fish Eye Lens
    GLfloat m_fishEyeLensSize;
    
    // Gaussian Blur
    GLfloat m_gaussianBlurIntensity;
    
    // Radial Blur
    GLfloat m_radialBlurRadius, m_radialBlurResolution;
    
    // Motion Blur
    GLfloat m_motionBlurTargetFps, m_motionBlurSamples;
    
    // Vignetting
    GLboolean m_vignettingTint, m_vignettingSepia;
    GLfloat m_vignettingRadius, m_vignettingSoftness;
    
    // BrightParts
    GLboolean m_brightPartsSmoothGradient;
    GLfloat m_brightPartsIntensity;
    
    // Lens Flare
    GLuint m_lensFlareNumber;
    GLboolean m_lensFlareUseDirt;
    GLfloat m_lensFlareGhostCount, m_lensFlareGhostDispersal, m_lensFlareGhostThreshold, m_lensFlareGhostDistortion;
    GLfloat m_lensFlareHaloRadius, m_lensFlareHaloThreshold;
    
    // Fast Approximate Anti Aliasing
    GLfloat m_ffaaOffset;
    
    // SSAO
    std::vector<glm::vec3> m_ssaoKernel;
    std::vector<glm::vec3> m_ssaoNoise;
    GLuint m_ssaoKernelSamples, m_ssaoNoiseSamples;
    GLfloat m_ssaoBias, m_ssaoRadius, m_ssaoNoiseSize;
    GLboolean m_ssaoNoiseUseLight;
    
    // Depth and Shadow Mapping
    GLboolean m_isOrthographicCamera, m_fromLightPosition;
    
    virtual void InitialiseFrameBuffers(const GLuint &width, const GLuint &height) = 0;
    virtual void LoadFrameBuffers(const GLuint &width , const GLuint &height) = 0;
    virtual void BindPPFXFBO(const PostProcessingEffectMode &mode) = 0;
    virtual void RenderPPFXScene(const PostProcessingEffectMode &mode) = 0;
    virtual void ChangePPFXScene(PostProcessingEffectMode &mode) = 0;
    virtual void RenderToScreen(CShaderProgram *pShaderProgram, const FrameBufferType &fboType,
                                const GLuint &bufferIndex, const TextureType &textureType) = 0;
    virtual void RenderPPFX(const PostProcessingEffectMode &mode) = 0;
    virtual void ResetFrameBuffer(const GLboolean &clearBuffers) = 0;
    virtual const char * const PostProcessingEffectToString(const PostProcessingEffectMode &mode) = 0;
    virtual FrameBufferType GetFBOtype(const PostProcessingEffectMode &mode) = 0;
};

struct IHud
{
    glm::vec4 m_textColor, m_guiColor, m_guiBackgroundColor, m_guiHighlightedColor;
    GLboolean m_enableHud;
    CFreeTypeFont *m_pFtFont;
    virtual void RenderHUD() = 0;
    virtual void RenderLabels(CFreeTypeFont *font, CShaderProgram *fontProgram,
                              const GLint &width, const GLint &height,
                              const GLuint &framesPerSecond, const bool &enableHud) = 0;
};

struct IControls
{
    // inputs
    GLboolean m_mouseButtonDown;
    GLdouble m_mouseX;
    GLdouble m_mouseY;
    virtual const char * const ControlTypeToString(const ControlType &type) = 0;
    virtual void LoadControls() = 0;
    virtual void RenderControls() = 0;
    virtual void UpdateControls() = 0;
    virtual void ClearControls() = 0;
    virtual void RemoveControls() = 0;
    virtual void UpdateKeyBoardControls(KeyboardState &state) = 0;
    virtual void UpdateMouseControls(MouseState &state) = 0;
};

#endif /* Interfaces_h */
