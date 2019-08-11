//
//  IGameObject.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 29/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef IGameObject_h
#define IGameObject_h

#include "../utilities/Transform.h"

class IGameObject {
public:
    //GameObject();
    //virtual ~GameObject() = default;
    virtual void Render(const GLboolean &useTexture) = 0;
    virtual void Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) = 0;
    
    glm::mat4 Model() const { return transform.GetModel(); }
protected:
    virtual void Release() = 0;
    CTransform transform;
};

#endif /* IGameObject_h */
