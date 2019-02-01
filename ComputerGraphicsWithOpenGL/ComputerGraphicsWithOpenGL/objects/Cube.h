
#pragma once

#ifndef Cube_h
#define Cube_h

#include "Extensions.h"
#include "Texture.h"
#include "VertexBufferObject.h"
#include "GameObject.h"


// Class for generating a unit sphere
class CCube: public GameObject
{
public:
    CCube(const GLfloat & size = 1.0f);
    ~CCube();

    void Create(const std::string &a_sDirectory, const std::vector<std::string> &a_textureNames);
    void Create(const std::string &a_sDirectory, const std::string &a_textureName);
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const bool &useTexture = true);
    void Release();
    GLfloat GetSize() const { return size; }
    
private:
    void Draw();
    
    GLuint m_vao, m_numTriangles;
    CVertexBufferObject m_vbo;
    
    std::vector<std::string> m_textureNames;
    std::vector<CTexture> m_textures;
    
    std::string m_directory;
    std::string m_textureName;
    CTexture m_texture;
    
    GLfloat size;
    bool m_isMultiTexture;
    std::vector<glm::vec3> m_cubeTangent;
    std::vector<glm::vec3> m_cubeBitangent;
};


#endif /* Cube_h */
