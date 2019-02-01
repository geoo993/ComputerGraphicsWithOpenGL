#pragma once

#ifndef Torus_h
#define Torus_h

#include "Texture.h"
#include "VertexBufferObject.h"
#include "GameObject.h"

//https://www.3dgep.com/environment-mapping-with-cg-and-opengl/
class CTorus: public GameObject {
    
public:
    
    CTorus(const GLfloat & size);
    ~CTorus();
    
    void Create(const std::string &a_sDirectory, const std::string &textureName);
    GLfloat GetSize() const { return m_size; }
    
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const bool &useTexture = true);
    void Release();
    
private:
    GLuint m_vao, m_numTriangles;
    CVertexBufferObject m_vbo;
    std::string m_directory, m_textureName;
    CTexture m_texture;
    GLfloat m_size;
  
};




#endif /* Torus_h */
