
#pragma once

#ifndef CEquirectangularCube_h
#define CEquirectangularCube_h

#include "../ObjectsBase.h"


// Class for generating a unit sphere
class CEquirectangularCube: public IGameObject
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
    
    GLuint m_vao, m_vbo, m_numTriangles;
    
    std::map<std::string, TextureType> m_textureNames;
    std::vector<CTexture*> m_textures;
    
    GLfloat size;
    std::vector<glm::vec3> m_cubeTangent;
    std::vector<glm::vec3> m_cubeBitangent;
};


#endif /* CEquirectangularCube_h */
