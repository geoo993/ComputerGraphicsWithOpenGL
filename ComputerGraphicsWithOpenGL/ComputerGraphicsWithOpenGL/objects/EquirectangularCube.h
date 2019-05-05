//
//  EquirectangularCube.hpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 05/05/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#pragma once
#ifndef EquirectangularCube_h
#define EquirectangularCube_h

#include "Texture.h"
#include "VertexBufferObject.h"
#include "GameObject.h"


// Class for generating a unit sphere
class CEquirectangularCube: public GameObject
{
public:
    CEquirectangularCube(const GLfloat & size = 1.0f);
    ~CEquirectangularCube();
    
    void Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames);
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    GLfloat GetSize() const { return size; }
    void Release();
    
private:
    
    GLuint m_vao, m_numTriangles;
    CVertexBufferObject m_vbo;
    
    std::map<std::string, TextureType> m_textureNames;
    std::vector<CTexture*> m_textures;
    
    std::string m_directory;
    
    GLfloat size;
    std::vector<glm::vec3> m_cubeTangent;
    std::vector<glm::vec3> m_cubeBitangent;
};

#endif /* EquirectangularCube_h */
