//
//  IShaders.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IShaders_h
#define IShaders_h

#include "../shaders/ShaderProgram.h"

struct IShaders {
    std::vector <CShaderProgram *> *m_pShaderPrograms;
    virtual void LoadShaderPrograms(const std::string &path) = 0;
};

#endif /* IShaders_h */
