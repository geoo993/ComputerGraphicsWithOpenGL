//
//  GUIType.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 10/03/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//
#pragma once

#ifndef GUIType_h
#define GUIType_h

enum class GUIMode {
    STATIC,
    DYNAMIC
};

enum class GUIType {
    BUTTON,
    LISTBOX,
    SLIDER,
    
    UNKNOWN,
};

struct GUIBoxData
{
    GLint x;
    GLint y;
    GLint width;
    GLint height;
    GLint textSize;
    GUIBoxData(): x(0), y(0), width(0), height(0), textSize(0)  {}
};


#endif /* GUIType_h */
