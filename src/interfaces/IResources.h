//
//  IResources.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IResources_h
#define IResources_h

struct IResources {
    virtual void InitialiseResources() = 0;
    virtual void LoadResources(const std::string &path) = 0;
};

#endif /* IResources_h */
