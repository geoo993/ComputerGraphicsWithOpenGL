#pragma once

#include "../TextureBase.h"

// Class that provides a texture for texture mapping in OpenGL
class CTexture
{
public:
    void CreateFromData(BYTE* data, GLint width, GLint height, GLint bpp, GLenum format, const TextureType &type,
                        GLboolean generateMipMaps = true, GLboolean gammaCorrection = false);
    GLboolean LoadTexture(const std::string &path, const TextureType &type, const GLboolean &generateMipMaps);
    GLuint LoadTexture(char const * path, const TextureType &type = TextureType::AMBIENT,
                       const GLboolean &generateMipMaps = true, GLboolean gammaCorrection = false);
    GLuint LoadCustomTexture(GLint width, GLint height, GLboolean generateMipMaps = true,
                            const TextureType &type = TextureType::AMBIENT, const GLvoid * data = nullptr);
    GLuint LoadHDRTexture(char const * path, const TextureType &type = TextureType::AMBIENT,
                          const GLboolean &generateMipMaps = true);

	void BindTexture2D(GLint textureUnit = 0) const;
    void BindTexture2DToTextureType() const;
    void BindCustomTexture2DToTextureType() const;
    void BindHDRTexture2D(GLint textureUnit = 0) const;
    void BindHDRTexture2DToTextureType() const;
    void BindTexture3D(GLint textureUnit = 0);
    void BindTextureCubeMap(GLint textureUnit = 0);

	void SetSamplerObjectParameter(GLenum parameter, GLenum value);
	void SetSamplerObjectParameterf(GLenum parameter, GLfloat value);
    void SetSamplerObjectParameterfv(GLenum parameter, const GLfloat * value);
    
	GLint GetWidth() const;
    GLint GetHeight() const;
    GLint GetBPP() const;
    GLenum GetFormat() const;
    std::string GetPath();
    TextureType GetType();

    void Release();

    CTexture();
    ~CTexture();
private:
    
    GLenum m_format;
	GLint m_width, m_height, m_bpp; // Texture width, height, and bytes per pixel
    GLuint m_textureID, m_hdrTextureID; // Texture id
	GLuint m_samplerObjectID; // Sampler id
	GLboolean m_mipMapsGenerated;

    std::string m_path;
    TextureType m_type;
};

