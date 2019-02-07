#include "Texture.h"

CTexture::CTexture()
{
	m_mipMapsGenerated = false;
}
CTexture::~CTexture()
{
    Release();
}

// Create a texture from the data stored in bData.  
void CTexture::CreateFromData(BYTE* data, GLint width, GLint height, GLint bpp, GLenum format, const TextureType &type,
                              GLboolean generateMipMaps, GLboolean gammaCorrection)
{
	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
 
    GLenum internalFormat;
    // We must handle this because of internal format parameter
    if(format == GL_RGBA || format == GL_BGRA){
        internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
        if(gammaCorrection)format=GL_RGBA;
    }
    else if(format == GL_RGB || format == GL_BGR){
        internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
        if(gammaCorrection)format=GL_RGB;
    }
    else if(format == GL_RED ){
        internalFormat = GL_RED;
    }
    else{
        internalFormat = format;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
    glGenSamplers(1, &m_samplerObjectID);

    m_path = "";
    m_type = type;
	m_mipMapsGenerated = generateMipMaps;
	m_width = width;
	m_height = height;
	m_bpp = bpp;
    
}

// Loads a 2D texture given the filename (sPath).  bGenerateMipMaps will generate a mipmapped texture if true
GLboolean CTexture::Load(const std::string &path, const TextureType &type, const GLboolean &generateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(path.c_str(), 0); // Check the file signature and deduce its format

	if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(path.c_str());
	
	if(fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, path.c_str());

	if(!dib) {
		char message[1024];
		sprintf(message, "Cannot load image\n%s\n", path.c_str());
		return false;
	}

	BYTE* pData = FreeImage_GetBits(dib); // Retrieve the image data

	// If somehow one of these failed (they shouldn't), return failure
	if (pData == nullptr || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;
	

	GLenum format;
	int bada = FreeImage_GetBPP(dib);
	if(FreeImage_GetBPP(dib) == 32)format = GL_BGRA;
	if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
	if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;
	CreateFromData(pData, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, type, generateMipMaps);
	
	FreeImage_Unload(dib);

	m_path = path;
    m_type = type;

	return true; // Success
}

// loads the file "hazard.png" into gTexture
GLuint CTexture::CreateTexture(std::string path, GLboolean generateMipMaps, GLint textureUnitAt, GLboolean gammaCorrection) {
    
    //    Bitmap img(filePath.c_str());
    //    ////*-----------------------------------------------------------------------------
    //    ////*  Make some rgba data (can also load a file here)
    //    ////*-----------------------------------------------------------------------------
    
    //    int tw = img.width; 
    //    int th = img.height;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* dib(0);
    
    fif = FreeImage_GetFileType(path.c_str(), 0); // Check the file signature and deduce its format
    
    if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
        fif = FreeImage_GetFIFFromFilename(path.c_str());
    
    if(fif == FIF_UNKNOWN) // If still unknown, return failure
        return -1;
    
    if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
        dib = FreeImage_Load(fif, path.c_str());
    
    if(!dib) {
        char message[1024];
        sprintf(message, "Cannot load image\n%s\n",path.c_str());
        return -1;
    }
    
    BYTE* pData = FreeImage_GetBits(dib); // Retrieve the image data
    
    // If somehow one of these failed (they shouldn't), return failure
    if (pData == nullptr || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
        return -1;
    
    GLint width = FreeImage_GetWidth(dib); 
    GLint height = FreeImage_GetHeight(dib);
    GLint bpp = FreeImage_GetBPP(dib);// bytes per pixel
    
    GLenum internalFormat;
    GLenum dataFormat;
    if(FreeImage_GetBPP(dib) == 32)dataFormat = GL_BGRA;
    if(FreeImage_GetBPP(dib) == 24)dataFormat = GL_BGR;
    if(FreeImage_GetBPP(dib) == 8)dataFormat = GL_LUMINANCE;
    
    // Generate an OpenGL texture ID for this texture
    //GLuint texture;
    m_textureID = textureUnitAt;
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // set wrap mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
    
    // We must handle this because of internal format parameter
    if(dataFormat == GL_RGBA || dataFormat == GL_BGRA){
        internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
        if(gammaCorrection)dataFormat=GL_RGBA;
    }
    else if(dataFormat == GL_RGB || dataFormat == GL_BGR){
        internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
        if(gammaCorrection)dataFormat=GL_RGB;
    }
    else if(dataFormat == GL_RED ){
        internalFormat = GL_RED;
    }
    else{
        internalFormat = dataFormat;
    }
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, pData);
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, dataFormat,GL_UNSIGNED_BYTE, pData);
    
    if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
    
    //GLuint m_samplerObject;
    m_samplerObjectID = textureUnitAt;
    glGenSamplers(1, &m_samplerObjectID);
    
    return m_textureID;
}

GLuint CTexture::CreateSimpleTexture(GLint width, GLint height, GLboolean generateMipMaps, GLint textureUnitAt, const GLvoid * data){
    
    // Generate an OpenGL texture ID for this texture
    //GLuint texture;
    m_textureID = textureUnitAt;
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
    
    //GLuint m_samplerObject;
    m_samplerObjectID = textureUnitAt;
    glGenSamplers(1, &m_samplerObjectID);
    
    return m_textureID;
}

void CTexture::SetSamplerObjectParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(m_samplerObjectID, parameter, value);
}

void CTexture::SetSamplerObjectParameterf(GLenum parameter, float value)
{
	glSamplerParameterf(m_samplerObjectID, parameter, value);
}


// Binds a texture for rendering
void CTexture::BindTexture2D(GLint iTextureUnit) const
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glBindSampler(iTextureUnit, m_samplerObjectID);
    
}

// Binds a texture for rendering
void CTexture::BindTexture3D(GLint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_3D, m_textureID);
    glBindSampler(iTextureUnit, m_samplerObjectID);
    
}

// Binds a texture for rendering
void CTexture::BindTextureCubeMap(GLint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
    glBindSampler(iTextureUnit, m_samplerObjectID);
    
}

// Frees memory on the GPU of the texture
void CTexture::Release()
{
	glDeleteSamplers(1, &m_samplerObjectID);
	glDeleteTextures(1, &m_textureID);
}

GLint CTexture::GetWidth()
{
	return m_width;
}

GLint CTexture::GetHeight()
{
	return m_height;
}

GLint CTexture::GetBPP()
{
	return m_bpp;
}

std::string CTexture::GetPath(){
    return m_path;
}

TextureType CTexture::GetType(){
    return m_type;
}
