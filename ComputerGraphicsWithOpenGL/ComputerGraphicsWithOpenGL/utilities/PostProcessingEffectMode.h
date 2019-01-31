//
//  PostProcessingEffectMode.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 29/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef PostProcessingEffectMode_h
#define PostProcessingEffectMode_h

// shader types
enum PostProcessingEffectMode {
    DefaultFrameBuffer,
    ColorInversion,
    GrayScale,
    Kernel,
    EdgeDetection,
    ScreenWave,
    Swirl,
    NightVision,
    LensCircle,
    Posterization,
    DreamVision,
    Pixelation,
    FrostedGlass,
    FrostedGlassExtra,
    Crosshatching,
    PredatorsThermalVision,
    Toonify,
    Shockwave,
    FishEye,
    BarrelDistortion,
    MultiScreenFishEye,
    FishEyeLens,
    FishEyeAntiFishEye,
    GaussianBlur,
    Blur,
    RadialBlur,
    MotionBlur,
    Vignetting,
    BrightParts,
    Bloom,
    LensFlare,
    FXAA,
    SSAO,
    
    NumberOfPPFX,
};


#endif /* PostProcessingEffectMode_h */
