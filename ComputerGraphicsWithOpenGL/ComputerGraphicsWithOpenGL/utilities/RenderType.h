//
//  RenderType.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef RenderType_h
#define RenderType_h

//screen and debuging
public enum RenderType {
    UsualRendering,
    InstancedRendering,
    DeferredRendering,
    DirectionalShadowMapping,
    OmnidirectionalShadowMapping,
    HDRToneMapping,
    HDRBloom,
    PorcupineRendering,
   
    NumberOfShaderOptions,
}

#endif /* RenderType_h */
