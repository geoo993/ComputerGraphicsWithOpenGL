#include "Cubemap.h"

CCubemap::CCubemap()
{
    m_uiSampler = 0;
    m_uiTexture = 0;
    m_envFramebuffer = 0;
    m_envRenderbuffer = 0;
    m_faces = {};
}

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
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CCubemap::LoadHRDCubemap(const int &width, const int &height, const TextureType &type, CShaderProgram *equirectangularProgram, const std::string &equirectangularCubmapPath, const std::string &equirectangularCubmap, const TextureType &equirectangularTexturetype) {
  
    m_faces = {};
    m_type = type;
    
    
    /*
     To convert an equirectangular image into a cubemap we need to render a (unit) cube and project the equirectangular map on all of the cube's faces from the inside and take 6 images of each of the cube's sides as a cubemap face. The vertex shader of this cube simply renders the cube as is and passes its local position to the fragment shader as a 3D sample vector:
     
     */
    
    /// Create a framebuffer object and bind it with
    glGenFramebuffers(1, &m_envFramebuffer);
    
    // To bind the framebuffer we use glBindFramebuffer:
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    
    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    glGenTextures(1, &m_uiTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glGenSamplers(1, &m_uiSampler);
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_uiSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    /*
     setting up 6 different view matrices facing each side of the cube, given a projection matrix with a fov of 90 degrees to capture the entire face, and render a cube 6 times storing the results in a floating point framebuffer:
     */
    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    
    
    
    // Creating a renderbuffer object looks similar to the framebuffer's code:
    glGenRenderbuffers(1, &m_envRenderbuffer);
    
    // And similarly we want to bind the renderbuffer object so all subsequent renderbuffer operations affect the current rbo:
    glBindRenderbuffer(GL_RENDERBUFFER, m_envRenderbuffer);
    
    // Creating a depth and stencil renderbuffer object is done by calling the glRenderbufferStorage function:
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    
    // Once we've allocated enough memory for the renderbuffer object we can unbind the renderbuffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    // Last thing left to do is actually attach the renderbuffer object:
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_envRenderbuffer);
    
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    for (unsigned int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_uiTexture, 0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0f);
        
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 view = captureViews[i];
        
        equirectangularProgram->UseProgram();
        int iTextureUnit = static_cast<int>(equirectangularTexturetype); // cubemap
        equirectangularProgram->SetUniform("material.emissionMap", iTextureUnit);
        equirectangularProgram->SetUniform("matrices.projMatrix", projection);
        equirectangularProgram->SetUniform("matrices.viewMatrix", view);
        
        CEquirectangularCube * m_pEquirectangularCube = new CEquirectangularCube(1.0f);
        m_pEquirectangularCube->Create(equirectangularCubmapPath, {
            { equirectangularCubmap, equirectangularTexturetype }
        } );
        m_pEquirectangularCube->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
        m_pEquirectangularCube->Render();
        
        delete m_pEquirectangularCube;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    glDisable(GL_DEPTH_TEST);
    
    /*
     
     We take the color attachment of the framebuffer and switch its texture target around for every face of the cubemap, directly rendering the scene into one of the cubemap's faces. Once this routine has finished (which we only have to do once) the cubemap envCubemap should be the cubemapped environment version of our original HDR image.
     
     We sample the environment map using its interpolated vertex cube positions that directly correspond to the correct direction vector to sample. Seeing as the camera's translation components are ignored, rendering this shader over a cube should give you the environment map as a non-moving background. Also, note that as we directly output the environment map's HDR values to the default LDR framebuffer we want to properly tone map the color values. Furthermore, almost all HDR maps are in linear color space by default so we need to apply gamma correction before writing to the default framebuffer.
     
     
     Well... it took us quite a bit of setup to get here, but we successfully managed to read an HDR environment map, convert it from its equirectangular mapping to a cubemap and render the HDR cubemap into the scene as a skybox. Furthermore, we set up a small system to render onto all 6 faces of a cubemap which we'll need again when convoluting the environment map.
     */
}

// Binds a texture for rendering
void CCubemap::BindCubemapTexture(GLint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);
    glBindSampler(iTextureUnit, m_uiSampler);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

TextureType CCubemap::GetType() const {
    return m_type;
}

// Release resources
void CCubemap::Release()
{
	glDeleteSamplers(1, &m_uiSampler);
	glDeleteTextures(1, &m_uiTexture);
    
    glDeleteTextures(1, &m_envFramebuffer);
    glDeleteTextures(1, &m_envRenderbuffer);
    m_faces.clear();
}
