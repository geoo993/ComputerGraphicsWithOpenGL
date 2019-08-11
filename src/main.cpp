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
//    if (argc < 2)
//    {
//        fprintf(stdout,"%s Version %d.%d\n", argv[0],
//                ComputerGraphicsWithOpenGL_VERSION_MAJOR,
//                ComputerGraphicsWithOpenGL_VERSION_MINOR);
//        fprintf(stdout,"Usage: %s number\n",argv[0]);
//        return 1;
//    }
//
    srand((unsigned)time(nullptr));//time should write like this .
    //srand(glfwGetTime()); // initialize random seed
    

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
    
//    double inputValue = atof(argv[1]);
//    double outputValue = sqrt(inputValue);
//    fprintf(stdout,"The square root of %g is %g\n", inputValue, outputValue);
//    
    //start game
    Game game;
    game.Execute(filepath, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    return 0;
}

