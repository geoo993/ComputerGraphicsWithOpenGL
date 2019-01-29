#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"
#include "Transform.h"

// Class for generating a xz plane of a given size
class CPlane
{
public:
	CPlane();
	~CPlane();
    void Create(const std::string &fLocation, float fWidth, float fHeight, float fTextureRepeat, unsigned int fDivisions);
    void Create(float fWidth, float fHeight, float fTextureRepeat, unsigned int fDivisions);
	void Render(const bool &useTexture = true);
	void Release();
    
    CTransform transform;
    
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
	float m_width;
	float m_height;
    float m_totalVertices;
};


