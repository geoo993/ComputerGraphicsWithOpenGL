#pragma once

#ifndef Torus_h
#define Torus_h

#include "../ObjectsBase.h"

//https://www.3dgep.com/environment-mapping-with-cg-and-opengl/
class CTorus: public IGameObject {
    
public:
    
    CTorus();
    ~CTorus();
    void Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames,
                const int &radialSeg,
                const int &circularSeg,
                const float &outerRadius,           // radius
                const float &innerRadius             //tick
    );
    
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    void Release();
    
private:
    GLuint m_vao, m_numTriangles;
    CVertexBufferObject m_vbo;
    
    std::map<std::string, TextureType> m_textureNames;
    std::vector<CTexture*> m_textures;
    
};




#endif /* Torus_h */
