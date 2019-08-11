//
//  IPostProcessing.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IPostProcessing_h
#define IPostProcessing_h

#include "../utilities/PostProcessingEffectMode.h"
#include "../shaders/ShaderProgram.h"
#include "../buffers/FrameBufferObject.h"

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
    GLboolean m_isOrthographicCamera, m_fromLightPosition, m_showDepth;
    GLfloat m_dirShadowBias, m_orthShadowBias;
    
    virtual void InitialiseFrameBuffers(const GLuint &width, const GLuint &height) = 0;
    virtual void LoadFrameBuffers(const GLuint &width , const GLuint &height) = 0;
    virtual void BindPPFXFBO(const PostProcessingEffectMode &mode) = 0;
    virtual void RenderPPFXScene(const PostProcessingEffectMode &mode) = 0;
    virtual void ChangePPFXScene(PostProcessingEffectMode &mode) = 0;
    virtual void RenderToScreen(CShaderProgram *pShaderProgram, const FrameBufferType &fboType,
                                const GLuint &bufferIndex, const TextureType &textureType, const GLboolean &useAO) = 0;
    virtual void RenderPPFX(const PostProcessingEffectMode &mode) = 0;
    virtual void ResetFrameBuffer(const GLboolean &clearBuffers) = 0;
    virtual const char * const PostProcessingEffectToString(const PostProcessingEffectMode &mode) = 0;
    virtual FrameBufferType GetFBOtype(const PostProcessingEffectMode &mode) = 0;
};

#endif /* IPostProcessing_h */
