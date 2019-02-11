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
#include "GameWindow.h"
#include "GameManager.h"
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

class Game: IGameTimer, IAudio, ICamera, IMaterials, ITextures, IShaders, IShaderUniform,
ILights, IRenderer, IRenderObject, IPostProcessing, IHud, IInput {
private:
    
    // Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
    void Initialise();
    void LoadResources(const std::string &path);
    void Update();
    void GameLoop();
    
// properties
private:
    CGameWindow m_gameWindow;
    CGameManager m_gameManager;
   
    // screens
    CPlane * m_pQuad;
    
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
    glm::vec3 m_spherePosition;
    
    //cube object
    CCube * m_pCube;
    CCube * m_pChromaticAberrationCube;
    CCube * m_pParallaxCube;
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
    
    // game timer
    void CalculateGameTime() override;
    
    // audio
    void InitialiseAudio(const std::string &path) override;
    void UpdateAudio() override;
    
    // camera
    void InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 &position) override;
    void SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) override;
    void UpdateCamera(const GLdouble & deltaTime, const GLuint & keyPressed, const GLboolean & mouseMove) override;
    
    // materials
    void SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                            const glm::vec3 &color = glm::vec3(1.0f), const GLfloat &shininess = 32.0f) override;
    // texture
    void LoadTextures(const std::string &path) override;
    CTexture AddTexture(const std::string &textureFile, const int &textureUnit, const bool &gammaCorrection = false) override;
    
    // shaders
    void LoadShaderPrograms(const std::string &path) override;
    
    // shader uniform
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
    
    // lights
    void SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
                         const GLboolean &useSpot, const GLboolean &useSmoothSpot, const GLboolean& useBlinn) override;
    void SetBaseLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const BaseLight & baseLight) override;
    void SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                    const DirectionalLight& directionalLight, const glm::vec3& direction) override;
    void SetPointLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const PointLight& pointLight) override;
    void SetSpotLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const SpotLight& spotLight, CCamera *camera) override;
    void RenderLight(CShaderProgram *pShaderProgram, CCamera * camera) override;
    void RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale, const glm::vec3 & color) override;
    
    // renderer
    void Render() override;
    void RenderScene() override;
    
    // render object
    void RenderQuad(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                    const GLfloat & scale, const GLboolean &useTexture) override;
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
    void RenderParallaxCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                            const GLfloat & scale, const GLboolean &useTexture);
    void RenderChromaticAberrationCube(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                                       const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderWoodenBox(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale,
                         const GLfloat & angle, const GLboolean &useTexture) override;
    void RenderSphere(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                      const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderTorus(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                     const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderTorusKnot(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                         const GLfloat & scale, const GLboolean &useTexture) override;
    void RenderMetalBalls(CShaderProgram *pShaderProgram, const glm::vec3 & position,
                          const GLfloat & scale, const GLboolean &useTexture) override;
    
    // post processing
    void InitialiseFrameBuffers(const GLuint &width, const GLuint &height) override;
    void RenderPPFXScene(const PostProcessingEffectMode &mode) override;
    void RenderToScreen(CShaderProgram *pShaderProgram, const bool & useQuad) override;
    void RenderPPFX(const PostProcessingEffectMode &mode) override;
    const char * const PostProcessingEffectToString(const PostProcessingEffectMode &mode) override;
    
    // HUD
    void RenderHUD() override;
    void DisplayFrameRate(CShaderProgram *fontProgram, const GLuint &framesPerSecond, const bool &enableHud) override;
    
    // inputs
    void KeyBoardControls(int &keyPressed, int &keyReleased, int &keyAction) override;
    void MouseControls(const int &button, const int &action) override;
    
};

#endif /* Game_h */
