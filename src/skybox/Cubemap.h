#pragma once

#include "../SkyboxBase.h"

class CCubemap
{
public:
    CCubemap();
    ~CCubemap();
    
    void LoadCubemap(const std::vector<std::string> &cubemapFaces, const TextureType &type);
    void LoadHRDCubemap(const int &width, const int &height, const TextureType &type, std::vector <CShaderProgram *> *shaderPrograms, IMaterials *mat, const std::string &equirectangularCubmapPath, const std::string &equirectangularCubmap, const TextureType &equirectangularTexturetype);
    void LoadIrradianceCubemap(const int &width, const int &height, const TextureType &type, std::vector <CShaderProgram *> *shaderPrograms, IMaterials *mat, const std::string &equirectangularCubmapPath, const std::string &equirectangularCubmap, const TextureType &equirectangularTexturetype);

    void BindCubemapTexture(GLint iTextureUnit);
    void BindEnvCubemapTexture(GLint iTextureUnit);
    void BindIrrCubemapTexture(GLint iTextureUnit);
    void BindPrefilterCubemapTexture(GLint iTextureUnit);
    void BindBRDFLUTTexture(GLint iTextureUnit);
    
    void Release();
    void Clear();
    TextureType GetType() const;
    
private:
    GLboolean LoadTexture(std::string filename, BYTE **bmpBytes, GLint &iWidth, GLint &iHeight);
	GLuint m_skyTexture, m_skySampler, m_envTexture, m_envSampler, m_irrTexture, m_irrSampler, m_prefilterTexture, m_prefilterSampler;
    GLuint m_brdfLUTTexture, m_brdfLUTSampler;
    GLuint m_envFramebuffer, m_envRenderbuffer;
    
    CShaderProgram * m_shaderProgram;
    CEquirectangularCube * m_pEquirectangularCube;
    CEquirectangularCube * m_irradianceCube;
    CEquirectangularCube * m_prefilterCube;
    CEquirectangularCube * m_brdfLUTCube;
    std::vector<std::string> m_faces;
    TextureType m_type;
};
