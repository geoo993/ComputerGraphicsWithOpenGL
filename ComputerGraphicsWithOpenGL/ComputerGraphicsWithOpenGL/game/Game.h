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

#include "Common.h"

// Setup includes
#include "Extensions.h"
#include "MatrixStack.h"
#include "Interfaces.h"
#include "CModel.h"
#include "Skybox.h"
#include "Plane.h"
#include "HeightMapTerrain.h"
#include "Cube.h"
#include "Sphere.h"
#include "Torus.h"
#include "TorusKnot.h"
#include "Metaballs.h"
#include "Quad.h"


// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp,
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to
// delete the object in the destructor.
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

class Game: IGameWindow, IGameTimer, IAudio, ICamera, IMaterials, ITextures, IShaders, IShaderUniform,
ILights, IRenderer, IRenderObject, IPostProcessing, IHud, IInput {
private:
    
    // Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
    void Initialise();
    void LoadResources(const std::string &path);
    void Update();
    void GameLoop();
    
// properties
private:
    
    // screens
    CQuad * m_pQuad;
    
    // skybox
    CSkybox *m_pSkybox;
    GLfloat m_mapSize;
    GLfloat m_changeSkybox = true;
    GLint m_skyboxNumber;
    
    // terrain
    CPlane *m_pPlanarTerrain;
    CHeightMapTerrain *m_pHeightmapTerrain;
    float m_heightMapMinHeight, m_heightMapMaxHeight;
    
    //models
    CModel * m_pCrossbow;
    CModel * m_pHorn;
    CModel * m_pSchofield;
    CModel * m_pMusket;
    CModel * m_pGrenade;
    CModel * m_pNanosuit;
    CModel * m_pFlashlight;
    
    //sphere object
    CSphere *m_pSphere;
    CSphere *m_pFireBallSphere;
    glm::vec3 m_spherePosition;
    
    //cube object
    CCube * m_pCube;
    CCube * m_pChromaticAberrationCube;
    CCube * m_pParallaxCube;
    CCube * m_pInteriorBox;
    glm::vec3 m_cubePosition;
    
    // woodenBox
    CCube *m_pWoodenBox;
    GLboolean m_woodenBoxesUseTexture;
    glm::vec3 m_woodenBoxesColor;
    std::vector<glm::vec3> m_woodenBoxesPosition;
    
    // lamp
    CCube *m_pLamp;
    
    //torus object
    CTorus * m_pTorus;
    glm::vec3 m_torusPosition;
    
    //torus knot
    CTorusKnot * m_pTorusKnot;
    glm::vec3 m_torusKnotPosition;
    
    // metal ball
    CMetaballs *m_pMetaballs;
    glm::vec3 m_metalballsPosition;

public:
    Game();
    Game(const Game &other);
    Game &operator=(const Game &other);
    ~Game();
    
    void Execute(const std::string &filepath, const GLuint &width, const GLuint &height);
   
protected:
    
    /// Game window
    void InitialiseGameWindow(const std::string &name, const std::string &filepath,
                              const GLuint &width,
                              const GLuint &height) override;
    
    /// Game timer
    void UpdateGameTimer() override;
    
    /// Audio
    void InitialiseAudio(const std::string &path) override;
    void UpdateAudio() override;
    
    /// Camera
    void InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 &position) override;
    void SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) override;
    void UpdateCamera(const GLdouble & deltaTime, const GLuint & keyPressed,
                      const GLfloat &mouseXoffset, const GLfloat &mouseYoffset, const GLboolean & mouseMove) override;
    void UpdateCameraEndFrame(const GLdouble & deltaTime) override;
    
    /// Materials
    void SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                            const glm::vec3 &color = glm::vec3(1.0f), const GLfloat &shininess = 32.0f) override;
    /// Textures
    void LoadTextures(const std::string &path) override;
    CTexture * AddTexture(const std::string &textureFile, const TextureType &type, const bool &gammaCorrection = false) override;
    
    /// Shaders
    void LoadShaderPrograms(const std::string &path) override;
    
    /// Shader uniform
    void SetPBRMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                               const glm::vec3 &albedo, const GLfloat &metallic, const GLfloat &roughness) override;
    void SetEnvironmentMapUniform(CShaderProgram *pShaderProgram, const GLboolean &useRefraction) override;
    void SetParallaxMapUniform(CShaderProgram *pShaderProgram, const GLfloat &heightScale) override;
    void SetBumpMapUniform(CShaderProgram *pShaderProgram, const GLfloat &uvTiling) override;
    void SetExplosionUniform(CShaderProgram *pShaderProgram,
                             const GLboolean &explode, const GLboolean &animate,
                             const GLfloat &time, const GLfloat &magnitude) override;
    void SetPorcupineRenderingUniform(CShaderProgram *pShaderProgram,
                                      const glm::vec3 &vertexNormalColor, const glm::vec3 &faceNormalColor,
                                      const GLfloat &magnitude) override;
    void SetWireframeUniform(CShaderProgram *pShaderProgram, const GLboolean &useWireframe, const GLfloat &thickness) override;
    void SetChromaticAberrationUniform(CShaderProgram *pShaderProgram, const glm::vec2 &fresnelValues) override;
    void SetFireBallUniform(CShaderProgram *pShaderProgram) override;
    
    /// Post Processing Unifom
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
    void SetPixelationUniform(CShaderProgram *pShaderProgram) override;
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
    void SetDeferredRenderingUniform(CShaderProgram *pShaderProgram, const GLboolean &useTexture) override;
    
    /// Lights
    void SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
                         const GLboolean &useSpot, const GLboolean &useSmoothSpot, const GLboolean& useBlinn) override;
    void SetBaseLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const BaseLight & baseLight) override;
    void SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                    const DirectionalLight& directionalLight, const glm::vec3& direction) override;
    void SetPointLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const PointLight& pointLight) override;
    void SetSpotLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const SpotLight& spotLight, CCamera *camera) override;
    void RenderLight(CShaderProgram *pShaderProgram, CCamera * camera) override;
    void RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale) override;
    
    /// Renderer
    void PreRendering() override;
    void Render() override;
    void PostRendering() override;
    void RenderScene(const GLboolean &toLightSpace = false) override;
    
    /// Render object
    void RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position = glm::vec3(0.0f),
                    const GLfloat & scale = 1.0f, const GLboolean &useTexture = true,
                    const GLboolean &bindTexture = false) override;
    void RenderSkyBox(CShaderProgram *pShaderProgram) override;
    void RenderTerrain(CShaderProgram *pShaderProgram, const GLboolean &useHeightMap, const GLboolean &useTexture) override;
    void RenderCrossBow(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                        const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderGrenade(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                       const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderNanosuit(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                        const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                    const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderInteriorBox(CShaderProgram *pShaderProgram, const glm::vec3 &position,
                           const float & scale, const bool &useTexture, const bool &bindTexture) override;
    void RenderParallaxCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                            const GLfloat & scale, const GLboolean &useTexture);
    void RenderChromaticAberrationCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                       const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderWoodenBox(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale,
                         const GLfloat & angle, const GLboolean &useTexture) override;
    void RenderSphere(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                      const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderFireBallSphere(CShaderProgram *pShaderProgram, const glm::vec3 & position, const GLfloat & scale) override;
    void RenderTorus(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                     const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderTorusKnot(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                         const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderMetalBalls(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                          const GLfloat & scale, const GLboolean &useTexture) override;
    
    /// Post processing
    void InitialiseFrameBuffers(const GLuint &width, const GLuint &height) override;
    void LoadFrameBuffers(const GLuint &width, const GLuint &height) override;
    void ActivateFBO(const PostProcessingEffectMode &mode) override;
    void RenderPPFXScene(const PostProcessingEffectMode &mode) override;
    void RenderToScreen(CShaderProgram *pShaderProgram, const FrameBufferType &fboType = FrameBufferType::Default,
                        const GLuint &bufferIndex = 0, const TextureType &textureType = TextureType::AMBIENT) override;
    void RenderPPFX(const PostProcessingEffectMode &mode) override;
    void ResetFrameBuffer(const GLboolean &clearBuffers = true) override;
    const char * const PostProcessingEffectToString(const PostProcessingEffectMode &mode) override;
    FrameBufferType GetFBOtype(const PostProcessingEffectMode &mode) override;
    
    /// HUD
    void RenderHUD() override;
    void DisplayFrameRate(CShaderProgram *fontProgram, const GLint &width, const GLint &height,
                          const GLuint &framesPerSecond, const bool &enableHud) override;
    
    /// Inputs
    void UpdateKeyBoardControls(int &keyPressed, int &keyReleased, int &keyAction) override;
    void UpdateMouseControls(const int &button, const int &action) override;
    
};

#endif /* Game_h */
