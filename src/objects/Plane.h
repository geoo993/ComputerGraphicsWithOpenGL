#pragma once

#include "../ObjectsBase.h"

// Class for generating a xz plane of a given size
class CPlane: public IGameObject
{
public:
	CPlane();
	~CPlane();
    void Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames,
                GLfloat fWidth, GLfloat fHeight, GLfloat fTextureRepeat, GLuint fDivisions);
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
    
    GLfloat m_width, m_height;
    GLint m_totalVertices;
};


