//
//  main.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

/* SOLID Principles
 
 Single Responsability Principle
 - A class should have a single reason to change.

 Open Closed Principle
 - The goal of the Open Closed Principle is to avoid having to jump into code that you already written.
    open for extension meaning that you can always make a new specification by extending or inheriting
    and closed for modification means that you should never need to add further fucntionality into an already defined specification.
    Therfore you would extend them by inheritance and then create the new functionality of a speficication.
 
 Liskov Substituion Principle
 - The Liskov Substituion Principle is named after Barbara Liskov.
    The goal of this principle is to specify that subtypes should be immidiatly substitutable for their base types.
    The idea is that you should be able to substitue a derived class where a base class is expected, without any side effects.
    Meaning that the derived class should not break the functionality of the base class.

 Interface Segregation Principle
 - The idea of the Interface Segregation Principle is to get you to not create interfaces with which are too large.
    And which require the implementers to maybe implement too much element from the interface, which it does not need.
 
 Dependency Inversion Principle
 - Dependency Inversion Principle is not about injection, it is simply specifying the best way
    to form dependencies between different objects. It is split into two different ideas.
 - A) High-level modules should not depend on low level modules.
    Both should depend on abstractions.
 - B) Abstractions should not depend on details.
    Details should depend on abstractions.
 
*/
#include "game/Game.h"

int main(int argc, const char * argv[])
{
    
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
    game.Execute(filepath, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    return 0;
}
