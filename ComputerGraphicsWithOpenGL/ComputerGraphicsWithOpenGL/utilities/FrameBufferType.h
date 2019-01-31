//
//  FrameBufferType.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef FrameBufferType_h
#define FrameBufferType_h


enum FrameBufferType {
    Default,
    MultiSampling,
    DirectionalShadowMapping,
    PointShadowMapping,
    HighDynamicRangeMapping,
    HighDynamicRangeMultipleRenderTargets, //(MRT)
    PingPongRendering,
    DeferredRendering,
    SSAORendering,
    SSAOProcessing,
    SSAOBlur,
};


#endif /* FrameBufferType_h */
