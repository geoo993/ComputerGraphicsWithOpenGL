#include "Cubemap.h"

CCubemap::CCubemap()
{}

CCubemap::~CCubemap()
{
    Release();
}


bool CCubemap::LoadTexture(std::string filename, BYTE **bmpBytes, int &iWidth, int &iHeight)
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

	iWidth = FreeImage_GetWidth(dib);
	iHeight = FreeImage_GetWidth(dib);
	int bpp = FreeImage_GetBPP(dib);


	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data


	// If somehow one of these failed (they shouldn't), return failure
	if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0) 
		return false;
	int test = FreeImage_GetDIBSize(dib);
	*bmpBytes = new BYTE [iWidth*iHeight*bpp/8];
	memcpy(*bmpBytes, bDataPointer, iWidth*iHeight*bpp/8);
	
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
void CCubemap::Bind(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);
	glBindSampler(iTextureUnit, m_uiSampler);
}


// Create the plane, including its geometry, texture mapping, normal, and colour
void CCubemap::Create(std::string sPositiveX, std::string sNegativeX, std::string sPositiveY, std::string sNegativeY, std::string sPositiveZ, std::string sNegativeZ)
{
	int iWidth, iHeight;

	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);

	// Load the six sides
	BYTE *pbImagePosX = nullptr;
    BYTE *pbImageNegX = nullptr;
    BYTE *pbImagePosY = nullptr;
    BYTE *pbImageNegY = nullptr;
    BYTE *pbImagePosZ = nullptr;
    BYTE *pbImageNegZ = nullptr;

	LoadTexture(sPositiveX, &pbImagePosX, iWidth, iHeight);
	LoadTexture(sNegativeX, &pbImageNegX, iWidth, iHeight);
	LoadTexture(sPositiveY, &pbImagePosY, iWidth, iHeight);
	LoadTexture(sNegativeY, &pbImageNegY, iWidth, iHeight);
	LoadTexture(sPositiveZ, &pbImagePosZ, iWidth, iHeight);
	LoadTexture(sNegativeZ, &pbImageNegZ, iWidth, iHeight);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pbImagePosX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pbImageNegX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pbImagePosY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pbImageNegY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pbImagePosZ);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pbImageNegZ);

    
	if (pbImagePosX != NULL) delete[] pbImagePosX;
	if (pbImageNegX != NULL) delete[] pbImageNegX;
	if (pbImagePosY != NULL) delete[] pbImagePosY;
	if (pbImageNegY != NULL) delete[] pbImageNegY;
	if (pbImagePosZ != NULL) delete[] pbImagePosZ;
	if (pbImageNegZ != NULL) delete[] pbImageNegZ;

	glGenSamplers(1, &m_uiSampler);
	glSamplerParameteri(m_uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(m_uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

}

// Release resources
void CCubemap::Release()
{
	glDeleteSamplers(1, &m_uiSampler);
	glDeleteTextures(1, &m_uiTexture);
}
