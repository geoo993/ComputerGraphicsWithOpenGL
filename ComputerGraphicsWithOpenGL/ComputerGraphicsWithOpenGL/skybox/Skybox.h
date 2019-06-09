#pragma once

#include "Texture.h"
#include "Cubemap.h"
#include "ShaderProgram.h"
#include "vertexBufferObject.h"
#include "GameObject.h"
#include "FrameBufferObject.h"

class CCubemap;

// This is a class for creating and rendering a skybox
class CSkybox: public GameObject
{
public:
	CSkybox();
	~CSkybox();
    void Create(const GLfloat &size, const std::string &hrdPath, const TextureType &type,
                CShaderProgram *equirectangularProgram, const GLuint &skyboxNumber);
	void Create(const GLfloat &size, const std::string &path, const TextureType &type, const GLuint &skyboxNumber);
    void CreateAttributes(const GLfloat &size = 1.0f);
    void BindSkyboxTo(const GLint &textureUnit);

    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    void Release();
    void Render(const GLboolean &useEnvCubemap = false);
    GLuint GetNumberOfSkyboxes() const;
    std::vector<std::string> GetSkyboxes() const;
private:
    void RenderCube();
private:
	GLuint m_vao, cubeVAO, cubeVBO, m_envCubemap, m_uiSampler;
    CFrameBufferObject *m_fbo;
	CVertexBufferObject m_vbo;
	CCubemap m_cubemapTexture;
    std::vector<std::string> m_skyboxes;
};
