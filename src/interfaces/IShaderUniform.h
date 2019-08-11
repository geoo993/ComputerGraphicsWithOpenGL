//
//  IShaderUniform.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IShaderUniform_h
#define IShaderUniform_h

#include "../shaders/ShaderProgram.h"

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

#endif /* IShaderUniform_h */
