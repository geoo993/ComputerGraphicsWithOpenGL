#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"
#include "GameObject.h"

// Class for generating a xz plane of a given size
class CPlane: public GameObject
{
public:
	CPlane();
	~CPlane();
    void Create(const std::string &fLocation, float fWidth, float fHeight, float fTextureRepeat, unsigned int fDivisions);
    void Create(float fWidth, float fHeight, float fTextureRepeat, unsigned int fDivisions);
    
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
   
    void Render(const bool &useTexture = true);
    void Release();
private:
    
    GLuint m_vao;
    CVertexBufferObject m_vbo;
    CTexture m_texture;
	float m_width;
	float m_height;
    float m_totalVertices;
};


