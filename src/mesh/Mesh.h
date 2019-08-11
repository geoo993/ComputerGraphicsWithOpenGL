//
//  Mesh.h
//  OpenGL_DSP
//
//  Created by GEORGE QUENTIN on 01/05/2018.
//  Copyright © 2018 Geo Games. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h

#pragma once

#include "../MeshBase.h"

#define INVALID_OGL_VALUE 0xFFFFFFFF
#define INVALID_MATERIAL 0xFFFFFFFF
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }


struct Mesh {
public:
    Mesh();
    ~Mesh();
    Mesh(const Mesh &other);
    Mesh &operator=(const Mesh &other);
    Mesh(const std::vector<Vertex>& Vertices,
          const std::vector<GLuint>& Indices,
          const std::vector<CTexture*> &Textures,
          const GLuint & materialIndex,
          const GLuint & numFaces
    );
    void Render(CShaderProgram *pShaderProgram, const GLboolean &useTexture = true);
    void Release();

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_numIndices;
    GLuint m_numFaces;
    std::vector<Vertex> m_vertices;
    std::vector<CTexture*> m_textures; // mesh textures of the current model
    GLuint m_materialIndex;
    
};


#endif /* Mesh_h */
