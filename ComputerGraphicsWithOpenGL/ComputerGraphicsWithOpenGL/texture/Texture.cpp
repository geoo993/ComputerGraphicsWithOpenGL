
/*
 Do this:
    #define STB_IMAGE_IMPLEMENTATION
 before you include this file in *one* C or C++ file to create the implementation.
 
 // i.e. it should look like this:
 #include ...
 #include ...
 #include ...
 #define STB_IMAGE_IMPLEMENTATION
 #include "stb_image.h"
 
 You can #define STBI_ASSERT(x) before the #include to avoid using assert.h.
 And #define STBI_MALLOC, STBI_REALLOC, and STBI_FREE to avoid using malloc,realloc,free
 
 
 QUICK NOTES:
     Primarily of interest to game developers and other people who can
     avoid problematic images and only need the trivial interface
 
     JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
     PNG 1/2/4/8/16-bit-per-channel
 
     TGA (not sure what subset, if a subset)
     BMP non-1bpp, non-RLE
     PSD (composited view only, no extra channels, 8/16 bit-per-channel)
 
     GIF (*comp always reports as 4-channel)
     HDR (radiance rgbE format)
     PIC (Softimage PIC)
     PNM (PPM and PGM binary only)
 
     Animated GIF still needs a proper API, but here's one way to do it:
     http://gist.github.com/urraka/685d9a6340b26b830d49
 
     - decode from memory or through FILE (define STBI_NO_STDIO to remove code)
     - decode from arbitrary I/O callbacks
     - SIMD acceleration on x86/x64 (SSE2) and ARM (NEON)
 Full documentation under "DOCUMENTATION" below.
 
 */

// STB image
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <stb/stb_image.h>
#include "Texture.h"

CTexture::CTexture()
{
    m_format = GL_RGB;
    m_path = "";
    m_type = TextureType::AMBIENT;
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
GLboolean CTexture::LoadTexture(const std::string &path, const TextureType &type, const GLboolean &generateMipMaps)
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
    
    int bada = FreeImage_GetBPP(dib);
    if(FreeImage_GetBPP(dib) == 32)m_format = GL_BGRA;
    if(FreeImage_GetBPP(dib) == 24)m_format = GL_BGR;
    if(FreeImage_GetBPP(dib) == 8)m_format = GL_LUMINANCE;
    CreateFromData(pData, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), m_format, type, generateMipMaps);
    
    FreeImage_Unload(dib);

    m_path = path;
    m_type = type;
    return true; // Success
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
GLuint CTexture::LoadTexture(char const * path, const TextureType &type,
                             const GLboolean &generateMipMaps, GLboolean gammaCorrection) {
    glGenTextures(1, &m_textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat, format;
        if(nrComponents == 4){
            format = GL_RGBA;
        }
        else if(nrComponents == 3){
            format = GL_RGB;
        }
        else {
            format = GL_LUMINANCE;
        }
        
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
        
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
        glGenSamplers(1, &m_samplerObjectID);
        
        m_format = format;
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    m_path = path;
    m_type = type;
    m_mipMapsGenerated = generateMipMaps;
    m_width = width;
    m_height = height;
    m_bpp = 0;
                                 
    return m_textureID;
}

GLuint CTexture::LoadCustomTexture(GLint width, GLint height, GLboolean generateMipMaps, const TextureType &type, const GLvoid * data) {
    
    m_format = GL_RGB;
    m_path = "";
    m_type = type;
    m_mipMapsGenerated = generateMipMaps;
    m_width = width;
    m_height = height;
    m_bpp = 0;
    
    // Generate an OpenGL texture ID for this texture
    //GLuint texture;
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, m_format, GL_FLOAT, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
    
    return m_textureID;
}

GLuint CTexture::LoadHDRTexture(char const * path, const TextureType &type, const GLboolean &generateMipMaps) {
    
    /*
     This might not be exactly what you were expecting as the image appears distorted and doesn't show any of the 6 individual cubemap faces of environment maps we've seen before. This environment map is projected from a sphere onto a flat plane such that we can more easily store the environment into a single image known as an equirectangular map. This does come with a small caveat as most of the visual resolution is stored in the horizontal view direction, while less is preserved in the bottom and top directions. In most cases this is a decent compromise as with almost any renderer you'll find most of the interesting lighting and surroundings in the horizontal viewing directions.
     
     Loading radiance HDR images directly requires some knowledge of the file format which isn't too difficult, but cumbersome nonetheless. Lucky for us, the popular one header library stb_image.h supports loading radiance HDR images directly as an array of floating point values which perfectly fits our needs. With stb_image added to your project, loading an HDR image is now as simple as follows:
     
     stb_image.h automatically maps the HDR values to a list of floating point values: 32 bits per channel and 3 channels per color by default. This is all we need to store the equirectangular HDR environment map into a 2D floating point texture.
     
     */
    
    std::string pathString = (std::string)path;
    if(pathString.substr(pathString.find_last_of(".") + 1) != "hdr") {
        char message[1024];
        sprintf(message, "Cannot load file that is not .hdr extension. \n%s\n", path);
        return -1;
    }
    
    // pbr: load the HDR environment map
    // ---------------------------------
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf(path, &width, &height, &nrComponents, 0);
    m_format = GL_RGB;
    if (data)
    {
        glGenTextures(1, &m_hdrTextureID);
        glBindTexture(GL_TEXTURE_2D, m_hdrTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, m_format, GL_FLOAT, data); // note how we specify the texture's data value to be float
        if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
        glGenSamplers(1, &m_samplerObjectID);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
        stbi_image_free(data);
    }
    
    m_path = path;
    m_type = type;
    m_mipMapsGenerated = generateMipMaps;
    m_width = width;
    m_height = height;
    m_bpp = 0;

    return m_hdrTextureID;
}

void CTexture::SetSamplerObjectParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(m_samplerObjectID, parameter, value);
}

void CTexture::SetSamplerObjectParameterf(GLenum parameter, float value)
{
	glSamplerParameterf(m_samplerObjectID, parameter, value);
}

void CTexture::SetSamplerObjectParameterfv(GLenum parameter, const GLfloat * value)
{
    glSamplerParameterfv(m_samplerObjectID, parameter, value);
}

// Binds a texture for rendering
void CTexture::BindTexture2D(GLint iTextureUnit) const
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glBindSampler(iTextureUnit, m_samplerObjectID);
}

void CTexture::BindTexture2DToTextureType() const
{
    GLint iTextureUnit = static_cast<GLint>(m_type);
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glBindSampler(iTextureUnit, m_samplerObjectID);
}

void CTexture::BindCustomTexture2DToTextureType() const
{
    GLint iTextureUnit = static_cast<GLint>(m_type);
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

// Binds a hdr texture for rendering
void CTexture::BindHDRTexture2D(GLint iTextureUnit) const
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_hdrTextureID);
    glBindSampler(iTextureUnit, m_samplerObjectID);
}

void CTexture::BindHDRTexture2DToTextureType() const
{
    GLint iTextureUnit = static_cast<GLint>(m_type);
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_hdrTextureID);
    glBindSampler(iTextureUnit, m_hdrTextureID);
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
    glDeleteTextures(1, &m_hdrTextureID);
}

GLint CTexture::GetWidth() const
{
	return m_width;
}

GLint CTexture::GetHeight() const
{
	return m_height;
}

GLint CTexture::GetBPP() const
{
	return m_bpp;
}

GLenum CTexture::GetFormat() const
{
    return m_format;
}

std::string CTexture::GetPath() {
    return m_path;
}

TextureType CTexture::GetType() {
    return m_type;
}
