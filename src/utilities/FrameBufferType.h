//
//  FrameBufferType.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef FrameBufferType_h
#define FrameBufferType_h


enum class FrameBufferType {
    Default,
    DepthMapping,
    DirectionalShadowMapping,
    OmnidirectionalShadowMapping,
    PingPongRendering,
    HighDynamicRangeLighting,
    HighDynamicRangeRendering,
    DeferredRendering,
    GeometryBuffer,
    SSAO,
};


#endif /* FrameBufferType_h */
