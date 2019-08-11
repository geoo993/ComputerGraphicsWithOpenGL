#pragma once

#include "Cubemap.h"

class CCubemap;
class CEquirectangularCube;

// This is a class for creating and rendering a skybox
class CSkybox: public IGameObject
{
public:
	CSkybox();
	~CSkybox();
    void Create(const GLfloat &size, const std::string &path, const TextureType &textureType, const SkyboxType &skyboxType, std::vector <CShaderProgram *> *shaderPrograms, IMaterials *mat, const TextureType &equirectangularTexturetype, const GLuint &skyboxNumber);
    void CreateAttributes(const GLfloat &size = 1.0f);
    void BindSkyboxTo(const GLint &textureUnit);
    void BindEnvSkyboxTo(const GLint &textureUnit);
    void BindIrrSkyboxTo(const GLint &textureUnit);
    void BindPrefilterSkyboxTo(const GLint &textureUnit);
    void BindBRDFLUTTextureTo(const GLint &textureUnit);
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    void Release();
    void Clear();
    void Render(const GLboolean &useTexture = true);
    void Render(const GLboolean &useTexture = true, const SkyboxType &skyboxType = SkyboxType::Default);
    GLuint GetNumberOfSkyboxes() const;
    std::vector<std::string> GetSkyboxes() const;
    
private:
    GLuint m_vao;
    CVertexBufferObject m_vbo;
    CCubemap *m_cubemapTexture;
    std::vector<std::string> m_skyboxes;
};
