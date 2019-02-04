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
     //m_textures.push_back(AddTexture(path+"/textures/woodenBox/woodDiffuse.jpg", 20)); // ambientMap 0
     //m_textures.push_back(AddTexture(path+"/textures/woodenBox/woodNormal.jpg", 21)); // normalMap 1
     //m_textures.push_back(AddTexture(path+"/textures/woodenBox/woodenBoxDiffuse.png", 22)); // diffuseMap 2
     //m_textures.push_back(AddTexture(path+"/textures/woodenBox/woodenBoxSpecular.png", 23)); // specularMap 4
}

CTexture Game::AddTexture(const std::string &textureFile, const int &textureUnit, const bool &gammaCorrection) {
    
    CTexture texture;
    texture.CreateTexture(textureFile,true, textureUnit, gammaCorrection);
    texture.BindTexture2D(textureUnit);
    
    return texture;
}
