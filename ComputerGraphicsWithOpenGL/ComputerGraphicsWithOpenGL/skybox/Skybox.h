#pragma once

#include "Cubemap.h"
#include "GameObject.h"

class CCubemap;

// This is a class for creating and rendering a skybox
class CSkybox: public GameObject
{
public:
	CSkybox();
	~CSkybox();
	void Create(const float &size, const std::string &path, const unsigned int &skyboxNumber);
	void Render(const int &textureUnit);
    void BindSkybox(const int &textureUnit);

    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    void Release();
    
private:
    void Render(const bool &useTexture = true);
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CCubemap m_cubemapTexture;
	
};
