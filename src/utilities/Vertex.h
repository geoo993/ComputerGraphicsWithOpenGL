//
//  Vertex.h
//  OpenGL_DSP
//
//  Created by GEORGE QUENTIN on 01/05/2018.
//  Copyright © 2018 Geo Games. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#pragma once
#include "../UtilitiesBase.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    Vertex() {};

    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& bitangent)
    {
        this->position      = pos;
        this->texture       = tex;
        this->normal        = normal;
        this->tangent       = tangent;
        this->bitangent     = bitangent;
    }
};

#endif /* Vertex_h */
