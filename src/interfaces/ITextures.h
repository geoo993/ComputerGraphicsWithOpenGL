//
//  ITextures.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//
#pragma once

#ifndef ITextures_h
#define ITextures_h

#include "../texture/Texture.h"

struct ITextures {
    std::vector<CTexture*> m_textures;
    virtual void LoadTextures(const std::string &path) = 0;
    virtual CTexture *AddTexture(const std::string &textureFile, const TextureType &type, const bool &gammaCorrection) = 0;
    virtual CTexture *AddHDRTexture(const std::string &textureFile, const TextureType &type) = 0;
    virtual CTexture *AddTexture(const GLfloat &width, const GLfloat &height, const TextureType &type, const GLvoid * data) = 0;
};

#endif /* ITextures_h */
