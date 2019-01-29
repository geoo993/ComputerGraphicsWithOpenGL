//
//  MeshEntry.h
//  OpenGL_DSP
//
//  Created by GEORGE QUENTIN on 01/05/2018.
//  Copyright © 2018 Geo Games. All rights reserved.
//

#ifndef MeshEntry_h
#define MeshEntry_h

#pragma once
#include "Common.h"
#include "Vertex.h"

struct MeshEntry {

    MeshEntry();

    ~MeshEntry();

    void Init(const std::vector<Vertex>& Vertices,
              const std::vector<GLuint>& Indices,
              const std::vector<glm::mat4> &mMatrices);
    GLuint vbo;
    GLuint mbo;
    GLuint ibo;
    GLuint m_numIndices;
    GLuint m_numFaces;
    std::vector<Vertex> m_vertices;
    GLuint m_materialIndex;
};


#endif /* MeshEntry_h */
