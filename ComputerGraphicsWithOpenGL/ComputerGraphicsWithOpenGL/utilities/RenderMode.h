//
//  RenderMode.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef RenderMode_h
#define RenderMode_h

//shading
//screen and debuging
public enum RenderMode {
    UsualRendering,
    InstancedRendering,
    DeferredRendering,
    DirectionalShadowMapping,
    OmnidirectionalShadowMapping,
    HDRToneMapping,
    HDRBloom,
    
    PorcupineRendering,
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
    FXAA,
    SSAO,
    GaussianBlur,
    Blur,
    RadialBlur,
    MotionBlur,
    Vignetting,
    BrightParts,
    Bloom,
    LensFlarePartOne,
    LensFlarePartTwo,
    DefaultFrameBuffer,
    
    NumberOfShaderOptions,
};

#endif /* RenderMode_h */
