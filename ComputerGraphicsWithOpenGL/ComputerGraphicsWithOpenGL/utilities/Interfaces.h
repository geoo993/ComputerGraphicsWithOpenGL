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

struct IGameWindow
{
    
    CGameWindow *m_gameWindow;
    CGameManager *m_gameManager;
    virtual void InitialiseGameWindow(const std::string &name, const std::string &filepath,
                                      const GLuint &width, const GLuint &height) = 0;
};

struct IGameTimer
{
    CHighResolutionTimer *m_pGameTimer;
    GLfloat m_timeInSeconds, m_timeInMilliSeconds, m_timePerSecond;
    GLdouble m_deltaTime, m_elapsedTime;
    GLint m_framesPerSecond, m_frameCount;
    virtual void UpdateGameTimer() = 0;
};

struct IAudio {
    CAudio *m_pAudio;
    GLint m_audioNumber;
    GLboolean m_changeAudio;
    std::vector <std::string> m_audioFiles;
    virtual void InitialiseAudio(const std::string &path) = 0;
    virtual void UpdateAudio() = 0;
};

struct ICamera {
    CCamera *m_pCamera;
    virtual void InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 &position) = 0;
    virtual void SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) = 0;
    virtual void UpdateCamera(const GLdouble & deltaTime, const GLuint & keyPressed, const GLboolean & mouseMove) = 0;
    virtual void UpdateCameraEndFrame(const GLdouble & deltaTime) = 0;
};

struct IMaterials {
    GLfloat m_materialShininess;
    virtual void SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                    const glm::vec3 &color, const GLfloat &shininess) = 0;
};

struct ITextures {
    std::vector<CTexture*> m_textures;
    virtual void LoadTextures(const std::string &path) = 0;
    virtual CTexture *AddTexture(const std::string &textureFile, const TextureType &type, const bool &gammaCorrection) = 0;
};

struct IShaders {
    std::vector <CShaderProgram *> *m_pShaderPrograms;
    virtual void LoadShaderPrograms(const std::string &path) = 0;
};

struct IShaderUniform {
    GLboolean m_useRefraction;
    GLfloat m_parallaxHeightScale, m_uvTiling, m_magnitude;
    virtual void SetPBRMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                       const glm::vec3 &albedo, const GLfloat &metallic, const GLfloat &roughness) = 0;
    virtual void SetEnvironmentMapUniform(CShaderProgram *pShaderProgram, const GLboolean &useRefraction) = 0;
    virtual void SetParallaxMapUniform(CShaderProgram *pShaderProgram, const GLfloat &heightScale) = 0;
    virtual void SetBumpMapUniform(CShaderProgram *pShaderProgram, const GLfloat &uvTiling) = 0;
    virtual void SetExplosionUniform(CShaderProgram *pShaderProgram,
                                     const GLboolean &explode, const GLboolean &animate,
                                     const GLfloat &time, const GLfloat &magnitude) = 0;
    virtual void SetPorcupineRenderingUniform(CShaderProgram *pShaderProgram,
                                              const glm::vec3 &vertexNormalColor, const glm::vec3 &faceNormalColor,
                                              const GLfloat &magnitude) = 0;
    virtual void SetWireframeUniform(CShaderProgram *pShaderProgram, const GLboolean &useWireframe, const GLfloat &thickness) = 0;
    virtual void SetChromaticAberrationUniform(CShaderProgram *pShaderProgram, const glm::vec2 &fresnelValues) = 0;
    
    
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
    virtual void SetPixelationUniform(CShaderProgram *pShaderProgram) = 0;
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
    virtual void SetLensFlareUniform(CShaderProgram *pShaderProgram) = 0;
};

struct ILights
{
    // Light
    GLfloat m_ambient;
    GLfloat m_diffuse;
    GLfloat m_specular;
    GLboolean m_useBlinn;
    
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
    GLuint m_pointLightPositionsIndex = 0;
    std::vector<glm::vec3> m_pointLightPositions;
    std::vector<glm::vec3> m_pointLightColors;
    
    // Spot Light
    GLboolean m_useSpot;
    GLboolean m_useSmoothSpot;
    glm::vec3 m_spotColor;
    GLfloat m_spotIntensity;
    GLfloat m_spotCutOff;
    GLfloat m_spotOuterCutOff;
    virtual void SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
                                 const GLboolean &useSpot, const GLboolean &useSmoothSpot, const GLboolean& useBlinn) = 0;
    virtual void SetBaseLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const BaseLight & baseLight) = 0;
    virtual void SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                            const DirectionalLight& directionalLight, const glm::vec3& direction) = 0;
    virtual void SetPointLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const PointLight& pointLight) = 0;
    virtual void SetSpotLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const SpotLight& spotLight,
                                     CCamera *camera) = 0;
    virtual void RenderLight(CShaderProgram *pShaderProgram, CCamera * camera) = 0;
    virtual void RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale) = 0;
};

struct IRenderer
{
    virtual void PreRendering() = 0;
    virtual void Render() = 0;
    virtual void PostRendering() = 0;
    virtual void RenderScene(const GLboolean &toLightSpace) = 0;
};

struct IRenderObject
{
    virtual void RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                            const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderSkyBox(CShaderProgram *pShaderProgram) = 0;
    virtual void RenderTerrain(CShaderProgram *pShaderProgram, const GLboolean &useHeightMap, const GLboolean &useTexture) = 0;
    virtual void RenderCrossBow(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderGrenade(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                               const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderNanosuit(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                               const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                            const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderParallaxCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                    const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderChromaticAberrationCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                               const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderWoodenBox(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale,
                                 const GLfloat & angle, const GLboolean &useTexture) = 0;
    virtual void RenderSphere(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                              const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderTorus(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                             const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderTorusKnot(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                 const GLfloat & scale, const GLboolean &useTexture) = 0;
    virtual void RenderMetalBalls(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                  const GLfloat & scale, const GLboolean &useTexture) = 0;
};

struct IPostProcessing {
    PostProcessingEffectMode m_currentPPFXMode;
    CFrameBufferObject *currentFBO;
    std::vector<CFrameBufferObject*> m_pFBOs;
    GLboolean m_changePPFXMode;
    GLuint m_PPFXOption;
    GLfloat m_coverage, m_shockWaveTime;
    virtual void InitialiseFrameBuffers(const GLuint &width, const GLuint &height) = 0;
    virtual void LoadFrameBuffers(const GLuint &width , const GLuint &height) = 0;
    virtual void ActivateFBO(const PostProcessingEffectMode &mode) = 0;
    virtual void RenderPPFXScene(const PostProcessingEffectMode &mode) = 0;
    virtual void RenderToScreen(CShaderProgram *pShaderProgram, const PostProcessingEffectMode &mode, const bool & useQuad) = 0;
    virtual void RenderPPFX(const PostProcessingEffectMode &mode) = 0;
    virtual const char * const PostProcessingEffectToString(const PostProcessingEffectMode &mode) = 0;
    virtual FrameBufferType GetFBOtype(const PostProcessingEffectMode &mode) = 0;
};

struct IHud
{
    GLboolean m_enableHud;
    CFreeTypeFont *m_pFtFont;
    virtual void RenderHUD() = 0;
    virtual void DisplayFrameRate(CShaderProgram *fontProgram, const GLint &width, const GLint &height,
                                  const GLuint &framesPerSecond, const bool &enableHud) = 0;
};

struct IInput
{
    // inputs
    GLboolean m_mouseButtonDown;
    GLboolean m_enableMouseMovement;
    GLboolean m_isMouseCursorVisible;
    GLboolean m_mouseMouseMoveClickSwitch;
    GLdouble m_mouseX;
    GLdouble m_mouseY;
    GLdouble m_keyPressTime;
    GLdouble m_lastKeyPressTime;
    GLint m_lastKeyPress;
    GLboolean m_isKeyPressRestriction;
    virtual void UpdateKeyBoardControls(int &keyPressed, int &keyReleased, int &keyAction) = 0;
    virtual void UpdateMouseControls(const int &button, const int &action) = 0;
};

#endif /* Interfaces_h */