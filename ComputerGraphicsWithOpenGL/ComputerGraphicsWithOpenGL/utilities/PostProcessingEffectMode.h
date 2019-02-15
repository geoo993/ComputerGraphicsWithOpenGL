//
//  PostProcessingEffectMode.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 29/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef PostProcessingEffectMode_h
#define PostProcessingEffectMode_h

// https://stackoverflow.com/questions/18335861/why-is-enum-class-preferred-over-plain-enum
// https://docs.microsoft.com/en-us/cpp/cpp/enumerations-cpp?view=vs-2017
// https://docs.microsoft.com/en-us/cpp/windows/enum-class-cpp-component-extensions?view=vs-2017
// shader types
enum class PostProcessingEffectMode {
    DefaultFrameBuffer,
    ColorInversion,
    GrayScale,
    Kernel,
    KernelBlur,
    EdgeDetection,
    SobelEdgeDetection,
    FreiChenEdgeDetection,
    ScreenWave,
    Swirl,
    NightVision,
    LensCircle,
    Posterization,
    DreamVision,
    Pixelation,
    FrostedGlassEffect,
    FrostedGlass,
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
    DepthMapping,
    Vignetting,
    BrightParts,
    Bloom,
    LensFlare,
    FXAA,
    SSAO,
    
    NumberOfPPFX,
};


#endif /* PostProcessingEffectMode_h */
