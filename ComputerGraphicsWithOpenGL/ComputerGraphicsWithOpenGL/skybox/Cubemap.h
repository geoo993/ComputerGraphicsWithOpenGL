#pragma once

#include "Common.h"
#include "TextureType.h"

class CCubemap
{
public:
    CCubemap();
    ~CCubemap();
    
    void LoadCubemap(const std::vector<std::string> &cubemapFaces, const TextureType &type);
    void LoadHRDCubemap(const std::vector<std::string> &cubemapFaces, const TextureType &type);
    void Bind(GLint iTextureUnit = 0);
    void Release();
    TextureType GetType() const;
    
private:
    GLboolean LoadTexture(std::string filename, BYTE **bmpBytes, GLint &iWidth, GLint &iHeight);
	GLuint m_uiVAO;
	GLuint m_uiTexture, m_uiSampler;
    std::vector<std::string> m_faces;
    TextureType m_type;
};
