//
//  TextureType.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 07/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef TextureType_h
#define TextureType_h

//#include "Extensions.h"

// https://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header
enum class TextureType {
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
    EMISSION,
    DISPLACEMENT,
    AO,
    GLOSSINESS,
    OPACITY,
    REFLECTION,
    UNKNOWN
};

/*
// Then you just need to define their string values.
template<> char const* enumStrings<TextureType>::data[] = {
    "ambientMap",
    "diffuseMap",
    "specularMap",
    "normalMap",
    "heightMap",
    "emissionMap",
    "displacementMap",
    "glossinessMap",
    "opacityMap",
    "reflectionMap",
    "unknown"
};
*/
#endif /* TextureType_h */
