//
//  Interfaces.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef Interfaces_h
#define Interfaces_h

#include "Camera.h"
#include "HighResolutionTimer.h"
#include "FreeTypeFont.h"
#include "Audio.h"
#include "PostProcessingEffectMode.h"

struct IGameTimer
{
    CHighResolutionTimer *m_pGameTimer;
    GLfloat m_timeInSeconds, m_timeInMilliSeconds, m_timePerSecond;
    GLdouble m_deltaTime, m_elapsedTime;
    GLint m_framesPerSecond, m_frameCount;
    virtual void CalculateGameTime() = 0;
};

struct IAudio {
    CAudio *m_pAudio;
    GLint m_audioNumber;
    GLboolean m_changeAudio;
    std::vector <std::string> m_audioFiles;
    virtual void InitialiseAudio(const std::string &path) = 0;
    virtual void UpdateAudio() = 0;
};

struct ICamera
{
    CCamera *m_pCamera;
    virtual void InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 &position) = 0;
    virtual void UpdateCamera(const GLdouble & deltaTime, const GLuint & keyPressed, const GLboolean & mouseMove) = 0;
};

struct IShaders {
    std::vector <CShaderProgram *> *m_pShaderPrograms;
    virtual void LoadShaderPrograms(const std::string &path) = 0;
};

struct IRenderer
{
    virtual void Render() = 0;
    virtual void RenderScene() = 0;
    virtual void RenderSkyBox(CShaderProgram *pShaderProgram, const int &cubeMapTextureUnit) = 0;
};

struct IPostProcessing {
    PostProcessingEffectMode m_currentPPFXMode;
    virtual void InitialiseFrameBuffers(const GLuint &width, const GLuint &height) = 0;
    virtual void RenderPPFXScene(const PostProcessingEffectMode &mode) = 0;
    virtual void RenderToScreen(CShaderProgram *pShaderProgram, const bool & useQuad) = 0;
    virtual void RenderPPFX(const PostProcessingEffectMode &mode) = 0;
    virtual const char * const PostProcessingEffectToString(const PostProcessingEffectMode &mode) = 0;
};

struct IHud
{
    GLboolean m_enableHud;
    CFreeTypeFont *m_pFtFont;
    virtual void RenderHUD() = 0;
    virtual void DisplayFrameRate(CShaderProgram *fontProgram, const GLuint &framesPerSecond, const bool &enableHud) = 0;
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
    virtual void KeyBoardControls(int &keyPressed, int &keyReleased, int &keyAction) = 0;
    virtual void MouseControls(const int &button, const int &action) = 0;
};

#endif /* Interfaces_h */
