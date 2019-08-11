//
//  Quad.h
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 28/09/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Quad_h
#define Quad_h

#include "../ObjectsBase.h"

// Class for generating a xz plane of a given size
class CQuad: public IGameObject
{
public:
    CQuad();
    ~CQuad();
    void Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames,
                const GLfloat &width = 1.0f, const GLfloat &height = 1.0f);
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    void Release();
private:
    
    GLuint m_vao;
    CVertexBufferObject m_vbo;
    
    std::map<std::string, TextureType>m_textureNames;
    std::vector<CTexture*> m_textures;
    
    GLint m_totalVertices;
};
#endif /* Quad_h */
