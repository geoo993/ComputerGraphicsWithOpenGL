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
    void Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames);
    GLfloat GetSize() const { return m_size; }
    
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    void Release();
    
private:
    GLuint m_vao, m_numTriangles;
    CVertexBufferObject m_vbo;
    
    std::map<std::string, TextureType> m_textureNames;
    std::vector<CTexture> m_textures;
    
    std::string m_directory;
    GLfloat m_size;
  
};




#endif /* Torus_h */
