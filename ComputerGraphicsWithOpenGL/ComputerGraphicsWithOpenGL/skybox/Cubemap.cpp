#include "Cubemap.h"

CCubemap::CCubemap()
{}

CCubemap::~CCubemap()
{
    Release();
}

GLboolean CCubemap::LoadTexture(std::string filename, BYTE **bmpBytes, GLint &iWidth, GLint &iHeight)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(filename.c_str(), 0); // Check the file signature and deduce its format

	if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
	
	if(fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, filename.c_str());

	if(!dib) {
		char message[1024];
		printf(message, "Cannot load image\n%s\n", filename.c_str());
		return false;
	}

    BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

    // If somehow one of these failed (they shouldn't), return failure
    if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0) {
        return false;
    }
    
    iWidth = FreeImage_GetWidth(dib);
    iHeight = FreeImage_GetWidth(dib);
    int bpp = FreeImage_GetBPP(dib);
    
	int test = FreeImage_GetDIBSize(dib);
	*bmpBytes = new BYTE [iWidth*iHeight*bpp/8];
    
	memcpy(*bmpBytes, bDataPointer, iWidth * iHeight * bpp / 8);
	
	/*
	GLenum format;
	FreeImage_GetBPP(dib);
	if(FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
	if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
	if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;
	*/
	
	FreeImage_Unload(dib);
	return true; // Success
}

// Binds a texture for rendering
void CCubemap::Bind(GLint iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);
	glBindSampler(iTextureUnit, m_uiSampler);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
void CCubemap::LoadCubemap(const std::vector<std::string> &cubemapFaces, const TextureType &type) {

    m_faces = cubemapFaces;
    m_type = type;
    
    // Generate an OpenGL texture ID for this texture
    glGenTextures(1, &m_uiTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);
    
    GLint iWidth, iHeight, iChannels;
    BYTE *data = nullptr;
    for (GLuint i = 0; i < cubemapFaces.size(); i++)
    {
        std::string face = cubemapFaces[i];
        LoadTexture(face, &data, iWidth, iHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

        if (data != NULL) delete[] data;
    }

    glGenSamplers(1, &m_uiSampler);
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
}

void CCubemap::LoadHRDCubemap(const std::vector<std::string> &cubemapFaces, const TextureType &type) {
    
    m_faces = cubemapFaces;
    m_type = type;
    
    // Generate an OpenGL texture ID for this texture
    glGenTextures(1, &m_uiTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);
    
    GLint iWidth, iHeight, iChannels;
    BYTE *data = nullptr;
    for (GLuint i = 0; i < cubemapFaces.size(); i++)
    {
        // note that we store each face with 16 bit floating point values
        std::string face = cubemapFaces[i];
        LoadTexture(face, &data, iWidth, iHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, iWidth, iHeight, 0, GL_RGB, GL_FLOAT, data);
        if (data != NULL) delete[] data;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
}

TextureType CCubemap::GetType() const {
    return m_type;
}


// Release resources
void CCubemap::Release()
{
	glDeleteSamplers(1, &m_uiSampler);
	glDeleteTextures(1, &m_uiTexture);
}
