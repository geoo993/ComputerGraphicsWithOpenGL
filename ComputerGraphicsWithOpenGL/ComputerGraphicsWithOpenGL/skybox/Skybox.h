#pragma once

#include "Cubemap.h"
#include "vertexBufferObject.h"
#include "GameObject.h"

class CCubemap;

// This is a class for creating and rendering a skybox
class CSkybox: public GameObject
{
public:
	CSkybox();
	~CSkybox();
	void Create(const GLfloat &size, const std::string &path, const TextureType &type, const GLuint &skyboxNumber);
    void BindSkyboxTo(const GLint &textureUnit);

    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    void Release();
    void Render(const GLboolean &useTexture = true);
    GLuint GetNumberOfSkyboxes();
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CCubemap m_cubemapTexture;
    std::vector<std::string> m_skyboxes;
};
