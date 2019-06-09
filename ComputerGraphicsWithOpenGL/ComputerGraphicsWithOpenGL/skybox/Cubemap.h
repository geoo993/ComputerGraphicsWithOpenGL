#pragma once

#include "TextureType.h"
#include "ShaderProgram.h"
#include "EquirectangularCube.h"

class CCubemap
{
public:
    CCubemap();
    ~CCubemap();
    
    void LoadCubemap(const std::vector<std::string> &cubemapFaces, const TextureType &type);
    void LoadHRDCubemap(const int &width, const int &height, const TextureType &type, CShaderProgram *equirectangularProgram,
    const std::string &equirectangularCubmapPath, const std::string &equirectangularCubmap, const TextureType &equirectangularTexturetype);
    void BindCubemapTexture(GLint iTextureUnit);
    void Release();
    TextureType GetType() const;
    
private:
    GLboolean LoadTexture(std::string filename, BYTE **bmpBytes, GLint &iWidth, GLint &iHeight);
	GLuint m_uiVAO;
	GLuint m_uiTexture, m_uiSampler;
    GLuint m_envFramebuffer, m_envRenderbuffer;
    
    std::vector<std::string> m_faces;
    TextureType m_type;
};
