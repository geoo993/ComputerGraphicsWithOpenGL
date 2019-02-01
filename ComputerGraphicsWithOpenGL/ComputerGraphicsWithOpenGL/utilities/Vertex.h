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
#include "Common.h"

class CVertex
{
public:
    CVertex(){};
    CVertex(glm::vec3 positionIn, glm::vec2 textureCoordIn, glm::vec3 normalIn)
    {
        position = positionIn;
        textureCoord = textureCoordIn;
        normal = normalIn;
    };
    
    glm::vec3 position;
    glm::vec2 textureCoord;
    glm::vec3 normal;
};


struct Vertex
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;
    glm::vec3 m_tangent;
    glm::vec3 m_bitangent;

    Vertex() {}

    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& bitangent)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
        m_tangent = tangent;
        m_bitangent = bitangent;
    }
};



#endif /* Vertex_h */
