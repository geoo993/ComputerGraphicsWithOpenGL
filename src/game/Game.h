//
//  Game.hpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef Game_h
#define Game_h

#pragma once

// Setup includes
#include "../GameBase.h"

class CSkybox;
class CModel;
class CPlane;
class CHeightMapTerrain;
class CCube;
class CSphere;
class CTorus;
class CTorusKnot;
class CMetaballs;
class CQuad;
class Game:
IAudio,
ICamera,
IControls,
IGameTimer,
IGameWindow,
IHud,
ILights,
IMaterials,
IPostProcessing,
IRenderer,
IRenderObject,
IResources,
IShaders,
IShaderUniform,
ITextures
{
    
// properties
private:

    // screens
    CQuad * m_pQuad;
    
    // skybox
    CSkybox *m_pSkybox;
    CSkybox *m_pEnvSkybox;
    CSkybox *m_pIrrSkybox;
    GLfloat m_skyboxSize;
    GLboolean m_changeSkybox;
    GLuint m_skyboxNumber;
    
    // terrain
    GLfloat m_mapSize;
    GLboolean m_showTerrain;
    GLboolean m_useTerrain;
    CPlane *m_pPlanarTerrain;
    CHeightMapTerrain *m_pHeightmapTerrain;
    float m_heightMapMinHeight, m_heightMapMaxHeight;
    
    //models
    CModel * m_teapot1;
    CModel * m_teapot2;
    CModel * m_teapot3;
    CModel * m_teapot4;
    CModel * m_teapot5;
    CModel * m_teapot6;
    CModel * m_teapot7;
    CModel * m_teapot8;
    CModel * m_teapot9;
    CModel * m_teapot10;
    CModel * m_teapot15;
    CModel * m_teapot16;
    CModel * m_teapot17;
    CModel * m_teapot19;
    CModel * m_lamborginhi;
    CModel * m_trolley;
    
    //sphere objects
    GLfloat m_sphereRotation;
    CSphere *m_pSpherePBR1;
    CSphere *m_pSpherePBR2;
    CSphere *m_pSpherePBR3;
    CSphere *m_pSpherePBR4;
    CSphere *m_pSpherePBR5;
    CSphere *m_pSpherePBR6;
    CSphere *m_pSpherePBR7;
    CSphere *m_pSpherePBR8;
    CSphere *m_pSpherePBR9;
    CSphere *m_pSpherePBR10;
    CSphere *m_pSpherePBR11;
    CSphere *m_pSpherePBR12;
    CSphere *m_pSpherePBR13;
    CSphere *m_pSpherePBR14;
    CSphere *m_pSpherePBR15;
    CSphere *m_pSpherePBR16;
    CSphere *m_pSpherePBR17;
    CSphere *m_pSpherePBR18;
    CSphere *m_pSpherePBR19;
    
    //cube objects
    CCube * m_pCube12;
    CCube * m_pCube13;
    CCube * m_pCube14;
    CCube * m_pInteriorBox;
    
    // lamp
    CCube *m_pLamp;
    
    // woodenBox
    CCube *m_pWoodenBox;
    std::vector<glm::vec3> m_woodenBoxesPosition;
    
    //torus object
    CTorus * m_pTorus;
    
    //torus knot
    CTorusKnot * m_pTorusKnot;
    
    // metal ball
    CMetaballs *m_pMetaballs;

public:
    Game();
    Game(const Game &other);
    Game &operator=(const Game &other);
    ~Game();
    
    void GameLoop();
    void Execute(const std::string &filepath, const GLuint &width, const GLuint &height);
    
protected:
    
    /// Audio
    void InitialiseAudio(const std::string &path) override;
    void UpdateAudio() override;
    
    /// Camera
    void InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 &position) override;
    void SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) override;
    void UpdateCamera(const GLdouble & deltaTime, const MouseState &mouseState,
    const KeyboardState &keyboardState, const GLboolean & mouseMove) override;
    void ResetCamera(const GLdouble & deltaTime) override;
    
    /// Controls
    const char * const ControlTypeToString(const ControlType &type) override;
    void LoadControls() override;
    void UpdateControls() override;
    void RenderControls() override;
    void ClearControls() override;
    void RemoveControls() override;
    void UpdateKeyBoardControls(KeyboardState &state) override;
    void UpdateMouseControls(MouseState &state) override;
    
    /// Game timer
    void UpdateSystemTime() override;
    void UpdateGameTime() override;
    
    /// Game window
    void InitialiseGameWindow(const std::string &name, const std::string &filepath,
                                const GLuint &width,
                                const GLuint &height) override;
    
    /// HUD
    void RenderHUD() override;
    void RenderLabels(CFreeTypeFont *font, CShaderProgram *fontProgram,
                        const GLint &width, const GLint &height,
                        const GLuint &framesPerSecond, const bool &enableHud) override;
    
    /// Lights
    void SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
    const GLboolean &useSpot, const GLboolean &useSmoothSpot, const GLboolean& useBlinn) override;
    void SetHRDLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
    const GLfloat & exposure, const GLfloat & gamma, const GLboolean &useHDR) override;
    void SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
    const DirectionalLight& directionalLight, const glm::vec3& direction, const glm::vec3 &position) override;
    void SetPointLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const PointLight& pointLight) override;
    void SetSpotLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const SpotLight& spotLight, CCamera *camera) override;
    void SetShadowUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const GLfloat &bias) override;
    void SetShadowMatrix(CShaderProgram *pShaderProgram, const glm::vec3 &lightPosition) override;
    void RenderLight(CShaderProgram *pShaderProgram,
                        const std::string &dirName,
                        const std::string &pointName,
                        const std::string &spotName,
                        CCamera *camera,
                        const GLboolean &useShadowMatrix = false) override;
    void RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const glm::vec3 & scale) override;
    
    /// Materials
    void SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                            const glm::vec4 &color = glm::vec4(1.0f), const GLfloat &shininess = 32.0f,
                            const GLfloat &uvTiling = 1.0f, const GLboolean &useAO = false,
                            const glm::vec4 &guiColor = glm::vec4(0.5f)) override;
    void SetPBRMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                const GLfloat &albedo, const GLfloat &metallic, const GLfloat &roughness,
                                const GLfloat &ao, const GLboolean &useIrradiance) override;
    void SetFogMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                const glm::vec3 &color, const GLboolean &bUseFog) override;
    
    /// Post processing
    void InitialiseFrameBuffers(const GLuint &width, const GLuint &height) override;
    void LoadFrameBuffers(const GLuint &width, const GLuint &height) override;
    void BindPPFXFBO(const PostProcessingEffectMode &mode) override;
    void RenderPPFXScene(const PostProcessingEffectMode &mode) override;
    void ChangePPFXScene(PostProcessingEffectMode &mode) override;
    void RenderToScreen(CShaderProgram *pShaderProgram, const FrameBufferType &fboType = FrameBufferType::Default,
    const GLuint &bufferIndex = 0, const TextureType &textureType = TextureType::AMBIENT,
    const GLboolean &useAO = false) override;
    void RenderPPFX(const PostProcessingEffectMode &mode) override;
    void ResetFrameBuffer(const GLboolean &clearBuffers = true) override;
    const char * const PostProcessingEffectToString(const PostProcessingEffectMode &mode) override;
    FrameBufferType GetFBOtype(const PostProcessingEffectMode &mode) override;
    
    /// Renderer
    void PreRendering() override;
    void Render() override;
    void PostRendering() override;
    void RenderScene(const GLboolean &toCustomShader = false, const GLboolean &includeLampsAndSkybox = false, const GLint &toCustomShaderIndex = 4) override;
    void RenderPBRScene(CShaderProgram *pShaderProgram, const GLboolean &toCustomShader, const GLint &toCustomShaderIndex, const GLfloat zfront, const GLfloat zback) override;
    void RenderRandomScene(CShaderProgram *pShaderProgram, const GLboolean &toCustomShader, const GLint &toCustomShaderIndex, const GLfloat zfront, const GLfloat zback) override;
    void RenderTerrainScene(CShaderProgram *pShaderProgram, const GLfloat yPos) override;
    
    /// Render Object
    void RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position = glm::vec3(0.0f),
                    const glm::vec3 & scale = glm::vec3(1.0f), const GLboolean &bindTexture = false) override;
    void RenderSkyBox(CShaderProgram *pShaderProgram) override;
    void RenderEnvSkyBox(CShaderProgram *pShaderProgram) override;
    void ResetSkyBox(CShaderProgram *pShaderProgram) override;
    void RenderTerrain(CShaderProgram *pShaderProgram,
                        const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale,
                        const GLboolean &useHeightMap) override;
    void RenderPrimitive(CShaderProgram *pShaderProgram, IGameObject *object,
                            const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale,
                            const GLboolean &useTexture = true) override;
    void RenderMetalBalls(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                            const glm::vec3 & scale, const GLboolean &useTexture) override;
    void RenderModel(CShaderProgram *pShaderProgram, CModel * model,
                        const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale);
    
    /// Resources
    void InitialiseResources() override;
    void LoadResources(const std::string &path) override;
    
    /// Shaders
    void LoadShaderPrograms(const std::string &path) override;
    
    /// Shader Uniform
    void SetTerrainUniform(CShaderProgram *pShaderProgram, const GLboolean &useHeightMap) override;
    void SetEnvironmentMapUniform(CShaderProgram *pShaderProgram, const GLboolean &useRefraction) override;
    void SetParallaxMapUniform(CShaderProgram *pShaderProgram, const GLfloat &heightScale) override;
    void SetExplosionUniform(CShaderProgram *pShaderProgram,
                                const GLboolean &explode, const GLboolean &animate,
                                const GLfloat &time, const GLfloat &magnitude) override;
    void SetPorcupineRenderingUniform(CShaderProgram *pShaderProgram,
                                        const glm::vec3 &vertexNormalColor, const glm::vec3 &faceNormalColor,
                                        const GLfloat &magnitude) override;
    void SetWireframeUniform(CShaderProgram *pShaderProgram, const GLboolean &useWireframe, const GLfloat &thickness) override;
    void SetChromaticAberrationUniform(CShaderProgram *pShaderProgram, const glm::vec2 &fresnelValues) override;
    void SetFireBallUniform(CShaderProgram *pShaderProgram) override;
    void SetDisintegrationUniform(CShaderProgram *pShaderProgram) override;
    
    /// Shader Uniform - Post Processing
    void SetImageProcessingUniform(CShaderProgram *pShaderProgram, const GLboolean &bUseScreenQuad) override;
    void SetColorInversionUniform(CShaderProgram *pShaderProgram) override;
    void SetGrayScaleUniform(CShaderProgram *pShaderProgram) override;
    void SetKernelUniform(CShaderProgram *pShaderProgram) override;
    void SetKernelBlurUniform(CShaderProgram *pShaderProgram) override;
    void SetEdgeDetectionUniform(CShaderProgram *pShaderProgram) override;
    void SetScreenWaveUniform(CShaderProgram *pShaderProgram) override;
    void SetSwirlUniform(CShaderProgram *pShaderProgram) override;
    void SetNightVisionUniform(CShaderProgram *pShaderProgram) override;
    void SetLensCircleUniform(CShaderProgram *pShaderProgram) override;
    void SetPosterizationUniform(CShaderProgram *pShaderProgram) override;
    void SetDreamVisionUniform(CShaderProgram *pShaderProgram) override;
    void SetPixelateUniform(CShaderProgram *pShaderProgram) override;
    void SetPixelationUniform(CShaderProgram *pShaderProgram) override;
    void SetKnittedPixelationUniform(CShaderProgram *pShaderProgram) override;
    void SetFrostedGlassEffectUniform(CShaderProgram *pShaderProgram) override;
    void SetFrostedGlassUniform(CShaderProgram *pShaderProgram) override;
    void SetCrosshatchingUniform(CShaderProgram *pShaderProgram) override;
    void SetPredatorsThermalVisionUniform(CShaderProgram *pShaderProgram) override;
    void SetToonifyUniform(CShaderProgram *pShaderProgram) override;
    void SetShockwaveUniform(CShaderProgram *pShaderProgram) override;
    void SetFishEyeUniform(CShaderProgram *pShaderProgram) override;
    void SetBarrelDistortionUniform(CShaderProgram *pShaderProgram) override;
    void SetMultiScreenFishEyeUniform(CShaderProgram *pShaderProgram) override;
    void SetFishEyeLensUniform(CShaderProgram *pShaderProgram) override;
    void SetFishEyeAntiFishEyeUniform(CShaderProgram *pShaderProgram) override;
    void SetGaussianBlurUniform(CShaderProgram *pShaderProgram, const GLboolean &horizontal) override;
    void SetBlurUniform(CShaderProgram *pShaderProgram) override;
    void SetRadialBlurUniform(CShaderProgram *pShaderProgram) override;
    void SetMotionBlurUniform(CShaderProgram *pShaderProgram) override;
    void SetDepthMappingUniform(CShaderProgram *pShaderProgram) override;
    void SetVignettingUniform(CShaderProgram *pShaderProgram) override;
    void SetBrightPartsUniform(CShaderProgram *pShaderProgram) override;
    void SetBloomUniform(CShaderProgram *pShaderProgram) override;
    void SetHRDToneMappingUniform(CShaderProgram *pShaderProgram) override;
    void SetLensFlareGhostUniform(CShaderProgram *pShaderProgram) override;
    void SetLensFlareUniform(CShaderProgram *pShaderProgram) override;
    void SetFastApproximateAntiAliasingUniform(CShaderProgram *pShaderProgram) override;
    void SetDeferredRenderingUniform(CShaderProgram *pShaderProgram) override;
    void SetScreenSpaceAmbientOcclusionUniform(CShaderProgram *pShaderProgram) override;
    void SetScreenSpaceAmbientOcclusionBlurUniform(CShaderProgram *pShaderProgram) override;
    void SetScreenSpaceAmbientOcclusionLightingUniform(CShaderProgram *pShaderProgram) override;
    void SetRainDropsUniform(CShaderProgram *pShaderProgram) override;
    void SetPaletteQuantizationAndDitheringUniform(CShaderProgram *pShaderProgram) override;
    void SetDistortedTVUniform(CShaderProgram *pShaderProgram) override;
    void SetRGBDisplayUniform(CShaderProgram *pShaderProgram) override;
    void SetRetroParallaxUniform(CShaderProgram *pShaderProgram) override;
    void SetScaryRetroParallaxUniform(CShaderProgram *pShaderProgram) override;
    void SetMoneyFilterUniform(CShaderProgram *pShaderProgram) override;
    void SetMicroprismMosaicUniform(CShaderProgram *pShaderProgram) override;
    void SetBayerMatrixDitheringUniform(CShaderProgram *pShaderProgram) override;
    void SetJuliaFreakUniform(CShaderProgram *pShaderProgram) override;
    void SetHeartBlendUniform(CShaderProgram *pShaderProgram) override;
    void SetEMInterferenceUniform(CShaderProgram *pShaderProgram) override;
    void SetCubicLensDistortionUniform(CShaderProgram *pShaderProgram) override;
    void SetCelShaderishUniform(CShaderProgram *pShaderProgram) override;
    void SetCartoonVideoUniform(CShaderProgram *pShaderProgram) override;
    
    
    /// Textures
    void LoadTextures(const std::string &path) override;
    CTexture * AddTexture(const std::string &textureFile, const TextureType &type, const bool &gammaCorrection = false) override;
    CTexture * AddHDRTexture(const std::string &textureFile, const TextureType &type) override;
    CTexture * AddTexture(const GLfloat &width, const GLfloat &height, const TextureType &type, const GLvoid * data) override;
};

#endif /* Game_h */
