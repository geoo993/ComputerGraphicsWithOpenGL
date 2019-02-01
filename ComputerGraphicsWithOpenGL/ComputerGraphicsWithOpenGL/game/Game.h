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
#include "GameWindow.h"
#include "GameManager.h"
#include "MatrixStack.h"
#include "Interfaces.h"
#include "OpenAssetImportMesh.h"
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
class COpenAssetImportMesh;
class CPlane;
class CHeightMapTerrain;
class CCube;
class CSphere;
class CTorus;
class CTorusKnot;
class CMetaballs;

class Game: IGameTimer, IAudio, ICamera, ITextures, IShaders, IRenderer, IRenderObject, IPostProcessing, IHud, IInput {
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
    COpenAssetImportMesh * m_pBarrel;
    glm::vec3 m_barrelPosition;
    GLfloat m_barrelRotation;
    
    //sphere object
    CSphere *m_pSphere;
    glm::vec3 m_spherePosition;
    
    //cube object
    CCube * m_pCube;
    glm::vec3 m_cubePosition;
    
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
    
    // boolean to string conversions
    inline const char * const BoolToString(bool b){ return b ? "true" : "false"; }

protected:
    
    // game timer
    void CalculateGameTime() override;
    
    // audio
    void InitialiseAudio(const std::string &path) override;
    void UpdateAudio() override;
    
    // camera
    void InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 &position) override;
    void UpdateCamera(const GLdouble & deltaTime, const GLuint & keyPressed, const GLboolean & mouseMove) override;
    
    // texture
    void LoadTextures(const std::string &path) override;
    CTexture AddTexture(const std::string &textureFile, const int &textureUnit, const bool &gammaCorrection = false) override;
    
    // shaders
    void LoadShaderPrograms(const std::string &path) override;
    
    // renderer
    void Render() override;
    void RenderScene() override;
    
    // render object
    void RenderSkyBox(CShaderProgram *pShaderProgram, const int &cubeMapTextureUnit) override;
    void RenderTerrain(CShaderProgram *pShaderProgram, const bool &useHeightMap, const bool &useTexture) override;
    void RenderBarrel(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) override;
    void RenderCube(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) override;
    void RenderSphere(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) override;
    void RenderTorus(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) override;
    void RenderTorusKnot(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) override;
    void RenderMetalBalls(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) override;
    
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
