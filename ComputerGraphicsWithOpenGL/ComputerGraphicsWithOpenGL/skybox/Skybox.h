#pragma once

#include "Cubemap.h"
#include "Transform.h"

class CCubemap;

// This is a class for creating and rendering a skybox
class CSkybox
{
public:
	CSkybox();
	~CSkybox();
	void Create(const float &size, const std::string &path, const unsigned int &skyboxNumber);
	void Render(const int &textureUnit);
    void BindSkybox(const int &textureUnit);
	void Release();

    CTransform transform;
    
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CCubemap m_cubemapTexture;
	
};
