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
    /*
     // start adding texture from in texture units from 20
     m_textures.push_back(AddTexture(path+"/textures/terrain/sand.png", 20)); // height map 0
     m_textures.push_back(AddTexture(path+"/textures/terrain/patchygrass.png", 21)); // height map 1
     m_textures.push_back(AddTexture(path+"/textures/terrain/stone.png", 22)); // height map 2
     m_textures.push_back(AddTexture(path+"/textures/terrain/snow.png", 3)); // height map 4
     m_textures.push_back(AddTexture(path+"/textures/terrain/grassfloor.jpg", 4)); // height map 4
     */
}

CTexture Game::AddTexture(const std::string &textureFile, const int &textureUnit, const bool &gammaCorrection) {
    
    CTexture texture;
    texture.CreateTexture(textureFile,true, textureUnit, gammaCorrection);
    texture.BindTexture2D(textureUnit);
    
    return texture;
}
