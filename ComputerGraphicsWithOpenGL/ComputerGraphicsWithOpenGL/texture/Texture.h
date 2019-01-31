#pragma once

#include "Common.h"

// Class that provides a texture for texture mapping in OpenGL
class CTexture
{
public:
	void CreateFromData(BYTE* data, int width, int height, int bpp, GLenum format, bool generateMipMaps = true, bool gammaCorrection = false);
	bool Load(std::string path, bool generateMipMaps = true);
    GLuint CreateTexture(std::string path, bool generateMipMaps = true, GLint textureUnitAt = 0, bool gammaCorrection = false);
    GLuint CreateSimpleTexture(int width, int height, bool generateMipMaps = true, GLint textureUnitAt = 0, const GLvoid * data = nullptr);
	void BindTexture2D(int textureUnit = 0) const;
    void BindTexture3D(int textureUnit = 0);
    void BindTextureCubeMap(int textureUnit = 0);

	void SetSamplerObjectParameter(GLenum parameter, GLenum value);
	void SetSamplerObjectParameterf(GLenum parameter, float value);

	int GetWidth();
    int GetHeight();
    int GetBPP();

    void Release();

    CTexture();
    ~CTexture();
private:
    
	int m_width, m_height, m_bpp; // Texture width, height, and bytes per pixel
	uint m_textureID; // Texture id
	uint m_samplerObjectID; // Sampler id
	bool m_mipMapsGenerated;

	std::string m_path;
};

