//
//  main.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.

// A simple program that computes the square root of a number
#include "Common.h"
#include "../build/ComputerGraphicsWithOpenGLConfig.h"
#include "game/Game.h"

int main(int argc, const char * argv[])
{

    std::cout << "Project Major Version: " << ComputerGraphicsWithOpenGL_VERSION_MAJOR << std::endl;
    std::cout << "Project Minor Version: " << ComputerGraphicsWithOpenGL_VERSION_MINOR << std::endl;
   
    /*
     This path is set in Edit scheme => Use custom working direcotry.
     Add the path by enabling Use project working direction,
     which is set in Edit Scheme -> Run -> Option -> Use Custom Working Directory (true) and set the working directory.
     see the links below for reference.
     https://en.sfml-dev.org/forums/index.php?topic=1547.0
     https://medium.com/@VIRAL/mac-xcode-and-c-file-handling-78ba999032df
     //char * path = getcwd(NULL, 0);
     
     OR
     */

    std::string path = ComputerGraphicsWithOpenGL_RESOURCES; // from project configuration file
    std::string filepath = std::string(path) + "/resources";
    
    std::cout << "Current working directory: " << (path) << std::endl;
    std::cout << "full working direcotry of resources: " << (filepath) << std::endl;

    //start game
    Game game;
    game.Execute(filepath, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    return 0;
}

