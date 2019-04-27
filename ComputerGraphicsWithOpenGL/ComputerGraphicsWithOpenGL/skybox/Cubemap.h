#pragma once

#include "Common.h"
#include "TextureType.h"

class CCubemap
{
public:
    CCubemap();
    ~CCubemap();
    
	//void Create(std::string sPositiveX, std::string sNegativeX, std::string sPositiveY,
    //            std::string sNegativeY, std::string sPositiveZ, std::string sNegativeZ, const TextureType &type);
    void Create(const std::vector<std::string> &cubemapFaces, const TextureType &type);
    GLboolean LoadTexture(std::string filename, BYTE **bmpBytes, GLint &iWidth, GLint &iHeight);
    void Bind(GLint iTextureUnit = 0);
    void Release();
    TextureType GetType() const;
    
private:
	GLuint m_uiVAO;
	GLuint m_uiTexture, m_uiSampler;
    std::vector<std::string> m_faces;
    TextureType m_type;
};
