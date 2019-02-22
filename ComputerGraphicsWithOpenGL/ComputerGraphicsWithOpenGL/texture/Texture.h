#pragma once

#include "Common.h"
#include "TextureType.h"

// Class that provides a texture for texture mapping in OpenGL
class CTexture
{
public:
	void CreateFromData(BYTE* data, GLint width, GLint height, GLint bpp, GLenum format, const TextureType &type,
                        GLboolean generateMipMaps = true, GLboolean gammaCorrection = false);
    GLboolean Load(const std::string &path, const TextureType &type = TextureType::UNKNOWN, const GLboolean &generateMipMaps = true);
    GLuint CreateTexture(std::string path, GLboolean generateMipMaps = true, GLint textureUnitAt = 0, GLboolean gammaCorrection = false);
    GLuint CreateTexture(GLint width, GLint height, GLboolean generateMipMaps = true,
                         const TextureType &type = TextureType::AMBIENT, const GLvoid * data = nullptr);
    
	void BindTexture2D(GLint textureUnit = 0) const;
    void BindTexture2DToTextureType() const;
    void BindTexture3D(GLint textureUnit = 0);
    void BindTextureCubeMap(GLint textureUnit = 0);

	void SetSamplerObjectParameter(GLenum parameter, GLenum value);
	void SetSamplerObjectParameterf(GLenum parameter, GLfloat value);

	GLint GetWidth();
    GLint GetHeight();
    GLint GetBPP();
    std::string GetPath();
    TextureType GetType();

    void Release();

    CTexture();
    ~CTexture();
private:
    
	GLint m_width, m_height, m_bpp; // Texture width, height, and bytes per pixel
	GLuint m_textureID; // Texture id
	GLuint m_samplerObjectID; // Sampler id
	GLboolean m_mipMapsGenerated;

    std::string m_path;
    TextureType m_type;
};

