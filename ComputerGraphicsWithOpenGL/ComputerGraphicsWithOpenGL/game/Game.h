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
#include "Camera.h"
#include "GameManager.h"
#include "HighResolutionTimer.h"
#include "MatrixStack.h"
#include "ShaderProgram.h"
#include "Audio.h"
#include "OpenAssetImportMesh.h"
#include "FreeTypeFont.h"
#include "PostProcessingEffectMode.h"
#include "Skybox.h"
#include "Plane.h"


// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp,
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to
// delete the object in the destructor.
class CHighResolutionTimer;
class CCamera;
class CShaderProgram;
class CFreeTypeFont;
class CAudio;
class CSkybox;
class COpenAssetImportMesh;
class CPlane;

class Game {
private:
    
    // Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
    void Initialise();
    void InitialiseFrameBuffers(
                                const GLuint &width = SCREEN_WIDTH,
                                const GLuint &height = SCREEN_HEIGHT);
    void InitialiseCamera(
                          const GLuint &width = SCREEN_WIDTH,
                          const GLuint &height = SCREEN_HEIGHT,
                          const glm::vec3 &position = glm::vec3(0.0f) );
    void InitialiseAudio(const std::string &path);
    
    void Update();
    void GameLoop();
    void LoadFromResources(const std::string &path);
    void CreateShaderPrograms(const std::string &path);
    
private:
    CGameWindow m_gameWindow;
    CGameManager m_gameManager;
    
    // shader program
    std::vector <CShaderProgram *> *m_pShaderPrograms;
    
    // timer
    CHighResolutionTimer *m_pGameTimer;
    
    // hud
    GLboolean m_enableHud;
    CFreeTypeFont *m_pFtFont;
    
    // game timers
    GLfloat m_timeInSeconds, m_timeInMilliSeconds, m_timePerSecond;
    GLdouble m_deltaTime, m_elapsedTime;
    GLint m_framesPerSecond, m_frameCount;
    
    // skybox
    CSkybox *m_pSkybox;
    GLfloat m_mapSize;
    GLfloat m_changeSkybox = true;
    GLint m_skyboxNumber;
    
    // textures
    std::vector<CTexture> m_textures;
    
    //camera
    CCamera *m_pCamera;
    
    //audio
    CAudio *m_pAudio;
    GLint m_audioNumber;
    GLboolean m_changeAudio;
    std::vector <std::string> m_audioFiles;
    
    // terrain
    CPlane *m_pPlanarTerrain;
    glm::vec3 m_terrainPosition;
    
    //models
    COpenAssetImportMesh * m_pBarrel;
    glm::vec3 m_barrelPosition;
    GLfloat m_barrelRotation;
    
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
   
    // post processing effects
    PostProcessingEffectMode m_currentPPFXMode;
    
public:
    Game();
    Game(const Game &other);
    Game &operator=(const Game &other);
    ~Game();
    
    void Execute(const std::string &filepath,
                 const GLuint &width = SCREEN_WIDTH,
                 const GLuint &height = SCREEN_HEIGHT);
    
    void RenderScene();
    void RenderPPFX(const PostProcessingEffectMode &mode);
    void Render();
    
    // inputs
    void KeyBoardControls(int &keyPressed, int &keyReleased, int &keyAction);
    void MouseControls(const int &button, const int &action);
    
    // font
    void RenderHUD();
    void DisplayFrameRate(bool enableHud, CShaderProgram *fontProgram);
    
    // conversions
    inline const char * const BoolToString(bool b){ return b ? "true" : "false"; }
    
};

#endif /* Game_h */
