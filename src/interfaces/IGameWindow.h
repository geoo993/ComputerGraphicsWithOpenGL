//
//  IGameWindow.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IGameWindow_h
#define IGameWindow_h

#include "../window/GameWindow.h"
#include "../manager/GameManager.h"

struct IGameWindow
{
    
    CGameWindow *m_gameWindow;
    CGameManager *m_gameManager;
    virtual void InitialiseGameWindow(const std::string &name, const std::string &filepath,
                                      const GLuint &width, const GLuint &height) = 0;
};

#endif /* IGameWindow_h */
