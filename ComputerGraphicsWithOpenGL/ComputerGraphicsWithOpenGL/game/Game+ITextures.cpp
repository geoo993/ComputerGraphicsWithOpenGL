//
//  Game+ITextures.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::LoadTextures(const std::string &path)
{
     // start adding texture from in texture units from 20
    m_textures.push_back(AddTexture(path+"/textures/noise_texture_gray.png", TextureType::NOISE));     // NoiseTex
    m_textures.push_back(AddTexture(path+"/textures/night_vision_binoculars_mask.png", TextureType::MASK)); // MaskTex
    m_textures.push_back(AddTexture(path+"/textures/perlin_noise_texture.png", TextureType::NOISE)); // NoiseTex
    m_textures.push_back(AddTexture(path+"/textures/lensFlare/lensColor.jpg", TextureType::MASK)); // Lens Flare
    m_textures.push_back(AddTexture(path+"/textures/lensFlare/lensTexture.jpg", TextureType::LENS)); // Lens Flare
    m_textures.push_back(AddTexture(path+"/textures/lensFlare/lensDirt.png", TextureType::NOISE)); // Lens Flare
    m_textures.push_back(AddTexture(path+"/textures/lensFlare/lensStarburst.png", TextureType::GLOSSINESS)); // Lens Flare
    m_textures.push_back(AddTexture(4, 4, TextureType::NOISE, &m_ssaoNoise[0])); // SSAO Noise
    
}

CTexture * Game::AddTexture(const std::string &textureFile, const TextureType &type, const bool &gammaCorrection) {
    
    CTexture *texture = new CTexture;
    //texture->LoadTexture(textureFile, true, type, gammaCorrection);
    //texture->BindTexture2D(textureUnit);
    
    texture->LoadTexture(textureFile, type, true);
    texture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    texture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    return texture;
}

CTexture * Game::AddTexture(const GLfloat &width, const GLfloat &height, const TextureType &type, const GLvoid * data) {
    CTexture *texture = new CTexture;
    texture->LoadTexture(width, width, true, type, data);
    return texture;
}
