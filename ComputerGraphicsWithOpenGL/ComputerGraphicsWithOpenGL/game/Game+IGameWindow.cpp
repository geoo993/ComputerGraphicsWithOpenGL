//
//  Game+IGameWindow.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 14/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::InitialiseGameWindow(const std::string &name,
                                const std::string &filepath,
                                const GLuint &width,
                                const GLuint &height) {
    
    m_gameWindow->CreateWindow(name, width, height);
    m_gameManager->SetResourcePath(filepath);
    
}
