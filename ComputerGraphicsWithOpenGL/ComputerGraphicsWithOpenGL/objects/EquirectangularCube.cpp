//
//  EquirectangularCube.cpp
//  OpenGL_Course_Work
//
//  Created by GEORGE QUENTIN on 15/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//

#include "EquirectangularCube.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

CEquirectangularCube::CEquirectangularCube(const GLfloat & size){
    this->size = size;
}

CEquirectangularCube::~CEquirectangularCube()
{
    Release();
}

void CEquirectangularCube::Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames) {
    m_textureNames = textureNames;
    m_textures.reserve(textureNames.size());
    
    // Iterate through all elements in std::map
    for (auto it = textureNames.begin(); it != textureNames.end(); ++it) {
        // if the current index is needed:
        auto i = std::distance(textureNames.begin(), it);
        
        // access element as *it
        m_textures.push_back(new CTexture);
        m_textures[i]->LoadHDRTexture((directory+it->first).c_str(), it->second, true);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        // any code including continue, break, return
    }
    
    
    float vertices[] = {
        // back face
        -size, -size, -size,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,// bottom-left
        size,  size, -size,   1.0f, 1.0f, 0.0f,  0.0f, -1.0f,// top-right
        size, -size, -size,   1.0f, 0.0f, 0.0f,  0.0f, -1.0f,// bottom-right
        size,  size, -size,   1.0f, 1.0f, 0.0f,  0.0f, -1.0f,// top-right
        -size, -size, -size,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,// bottom-left
        -size,  size, -size,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,// top-left
        // front face
        -size, -size,  size,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,// bottom-left
        size, -size,  size,   1.0f, 0.0f, 0.0f,  0.0f,  1.0f,// bottom-right
        size,  size,  size,   1.0f, 1.0f, 0.0f,  0.0f,  1.0f,// top-right
        size,  size,  size,   1.0f, 1.0f, 0.0f,  0.0f,  1.0f,// top-right
        -size,  size,  size,   0.0f, 1.0f, 0.0f,  0.0f,  1.0f,// top-left
        -size, -size,  size,   0.0f, 0.0f, 0.0f,  0.0f,  1.0f,// bottom-left
        // left face
        -size,  size,  size,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,// top-right
        -size,  size, -size,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,// top-left
        -size, -size, -size,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,// bottom-left
        -size, -size, -size,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,// bottom-left
        -size, -size,  size,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,// bottom-right
        -size,  size,  size,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,// top-right
        // right face
        size,  size,  size,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f,// top-left
        size, -size, -size,   0.0f, 1.0f, 1.0f,  0.0f,  0.0f,// bottom-right
        size,  size, -size,   1.0f, 1.0f, 1.0f,  0.0f,  0.0f,// top-right
        size, -size, -size,   0.0f, 1.0f, 1.0f,  0.0f,  0.0f,// bottom-right
        size,  size,  size,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f,// top-left
        size, -size,  size,   0.0f, 0.0f, 1.0f,  0.0f,  0.0f,// bottom-left
        // bottom face
        -size, -size, -size,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,// top-right
        size, -size, -size,   1.0f, 1.0f, 0.0f, -1.0f,  0.0f,// top-left
        size, -size,  size,   1.0f, 0.0f, 0.0f, -1.0f,  0.0f,// bottom-left
        size, -size,  size,   1.0f, 0.0f, 0.0f, -1.0f,  0.0f,// bottom-left
        -size, -size,  size,   0.0f, 0.0f, 0.0f, -1.0f,  0.0f,// bottom-right
        -size, -size, -size,   0.0f, 1.0f, 0.0f, -1.0f,  0.0f,// top-right
        // top face
        -size,  size, -size,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,// top-left
        size,  size , size,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,// bottom-right
        size,  size, -size,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,// top-right
        size,  size,  size,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,// bottom-right
        -size,  size, -size,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,// top-left
        -size,  size,  size,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,// bottom-left
    };
    m_numTriangles = 36;
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void CEquirectangularCube::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

// Render the cube as a set of triangles
void CEquirectangularCube::Render(const GLboolean &useTexture)
{
    // bind the VAO (the triangle)
    glBindVertexArray(m_vao);
    
    if (useTexture){
        for (GLuint i = 0; i < m_textures.size(); ++i){
            m_textures[i]->BindHDRTexture2DToTextureType();
        }
    }
    glDrawArrays( GL_TRIANGLES, 0, m_numTriangles ); // draw the vertixes
    
    glBindVertexArray(0);
    
}

// Release memory on the GPU 
void CEquirectangularCube::Release()
{
    for (GLuint i = 0; i < m_textures.size(); ++i){
        m_textures[i]->Release();
        delete m_textures[i];
    }
    m_textures.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    glDeleteVertexArrays(1, &m_vbo);
    
}
