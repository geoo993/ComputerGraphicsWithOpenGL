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
#include "Lighting.h"

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
    virtual void SetCameraUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, CCamera *camera) = 0;
    virtual void UpdateCamera(const GLdouble & deltaTime, const GLuint & keyPressed, const GLboolean & mouseMove) = 0;
};

struct IMaterials {
    GLfloat m_materialShininess;
    virtual void SetMaterialUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                    const glm::vec3 &color, const GLfloat &shininess) = 0;
};

struct ITextures {
    std::vector<CTexture> m_textures;
    virtual void LoadTextures(const std::string &path) = 0;
    virtual CTexture AddTexture(const std::string &textureFile, const int &textureUnit, const bool &gammaCorrection) = 0;
};

struct IShaders {
    std::vector <CShaderProgram *> *m_pShaderPrograms;
    virtual void LoadShaderPrograms(const std::string &path) = 0;
};

struct ILights
{
    // Light
    GLfloat m_ambient;
    GLfloat m_diffuse;
    GLfloat m_specular;
    
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
    vector<glm::vec3> m_pointLightPositions;
    std::vector<glm::vec3> m_pointLightColors;
    
    // Spot Light
    GLboolean m_useSpot;
    GLboolean m_useSmoothSpot;
    glm::vec3 m_spotColor;
    GLfloat m_spotIntensity;
    GLfloat m_spotCutOff;
    GLfloat m_spotOuterCutOff;
    virtual void SetLightUniform(CShaderProgram *pShaderProgram, const GLboolean &useDir, const GLboolean &usePoint,
                                 const GLboolean &useSpot, const GLboolean &smoothSpot) = 0;
    virtual void SetBaseLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const BaseLight & baseLight) = 0;
    virtual void SetDirectionalLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName,
                                            const DirectionalLight& directionalLight, const glm::vec3& direction) = 0;
    virtual void SetPointLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const PointLight& pointLight) = 0;
    virtual void SetSpotLightUniform(CShaderProgram *pShaderProgram, const std::string &uniformName, const SpotLight& spotLight,
                                     CCamera *camera) = 0;
    virtual void RenderLight(CShaderProgram *pShaderProgram, CCamera * camera) = 0;
    virtual void RenderLamp(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale, const glm::vec3 & color) = 0;
};

struct IRenderer
{
    virtual void Render() = 0;
    virtual void RenderScene() = 0;
    virtual void RenderSkyBox(CShaderProgram *pShaderProgram, const int &cubeMapTextureUnit) = 0;
};

struct IRenderObject
{
    virtual void RenderSkyBox(CShaderProgram *pShaderProgram, const int &cubeMapTextureUnit) = 0;
    virtual void RenderTerrain(CShaderProgram *pShaderProgram, const bool &useHeightMap, const bool &useTexture) = 0;
    virtual void RenderBarrel(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) = 0;
    virtual void RenderCube(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) = 0;
    virtual void RenderWoodenBox(CShaderProgram *pShaderProgram, const glm::vec3 &position, const GLfloat & scale,
                                 const GLfloat & angle, const bool &useTexture) = 0;
    virtual void RenderSphere(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) = 0;
    virtual void RenderTorus(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) = 0;
    virtual void RenderTorusKnot(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) = 0;
    virtual void RenderMetalBalls(CShaderProgram *pShaderProgram, const GLfloat & scale, const bool &useTexture) = 0;
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
