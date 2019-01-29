//
//  main.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "game/Game.h"

int main(int argc, const char * argv[]) {
    
    srand((unsigned)time(nullptr));//time should write like this .
    //srand(glfwGetTime()); // initialize random seed
    
    // https://en.sfml-dev.org/forums/index.php?topic=1547.0
    // https://medium.com/@VIRAL/mac-xcode-and-c-file-handling-78ba999032df
    //char path[1024];
    //getcwd(path, sizeof(path) );
    //puts(path);
    
    char * path = getcwd(NULL, 0); // Platform-dependent, see reference link below
    
    //path from project working direction, which is set in Edit Scheme -> Run -> Option -> Working Directory (true)
    std::string filepath = std::string(path) + "/resources";
    
    std::cout << "Current dir: " << (path) << std::endl;
    std::cout << "full file path: " << (filepath) << std::endl;
    
    //start game
    Game game;
    game.Execute(filepath);
    
    return 0;
}
