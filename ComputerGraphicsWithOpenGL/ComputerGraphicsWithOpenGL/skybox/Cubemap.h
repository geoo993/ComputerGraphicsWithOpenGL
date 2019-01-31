#pragma once

#include "Texture.h"
#include "vertexBufferObject.h"

class CCubemap
{
public:
    CCubemap();
    ~CCubemap();
    
	void Create(std::string sPositiveX, std::string sNegativeX, std::string sPositiveY, std::string sNegativeY, std::string sPositiveZ, std::string sNegativeZ);
    bool LoadTexture(std::string filename, BYTE **bmpBytes, int &iWidth, int &iHeight);
    void Bind(int iTextureUnit = 0);
    void Release();

private:
	uint m_uiVAO;
	CVertexBufferObject m_vboRenderData;
	GLuint m_uiTexture;
	GLuint m_uiSampler; // Sampler name

};
