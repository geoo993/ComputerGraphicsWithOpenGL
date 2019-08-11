#pragma once

#include "../ObjectsBase.h"

// Class for rendering a grid-like terrain
class CGrid: public IGameObject
{
public:
	CGrid();
	~CGrid();
	void Create(float fWidth, float fHeight, int iLines);
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    void Release();
    
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	glm::vec4 m_vColour;	
	int m_iVertices;
};
