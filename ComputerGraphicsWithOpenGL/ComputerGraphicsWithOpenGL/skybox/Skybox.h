#pragma once

#include "Texture.h"
#include "Cubemap.h"
#include "ShaderProgram.h"
#include "vertexBufferObject.h"
#include "GameObject.h"
#include "EquirectangularCube.h"

class CCubemap;
class CEquirectangularCube;

// This is a class for creating and rendering a skybox
class CSkybox: public GameObject
{
public:
	CSkybox();
	~CSkybox();
    void Create(const GLfloat &size, const std::string &path, const TextureType &type, const GLboolean &useEnvCubemap, CShaderProgram *equirectangularProgram, const TextureType &equirectangularTexturetype, const GLuint &skyboxNumber);
    void CreateAttributes(const GLfloat &size = 1.0f);
    void BindSkyboxTo(const GLint &textureUnit);

    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    void Release();
    void Render(const GLboolean &useTexture = true);
    GLuint GetNumberOfSkyboxes() const;
    GLboolean IsEnvCubemap() const;
    std::vector<std::string> GetSkyboxes() const;
    
private:
    GLboolean m_useEnvCubemap;
    GLuint m_vao;
    CVertexBufferObject m_vbo;
    CCubemap m_cubemapTexture;
    std::vector<std::string> m_skyboxes;
    GLuint m_envFramebuffer, m_envCubemap, m_envRenderbuffer;
};
