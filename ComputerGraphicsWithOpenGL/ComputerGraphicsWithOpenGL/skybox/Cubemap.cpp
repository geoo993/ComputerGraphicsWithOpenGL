#include "Cubemap.h"

CCubemap::CCubemap()
{
    m_skyTexture = 0;
    m_skySampler = 0;
    m_envTexture = 0;
    m_envSampler = 0;
    m_irrTexture = 0;
    m_irrSampler = 0;
    m_prefilterTexture = 0;
    m_prefilterSampler = 0;
    m_brdfLUTSampler = 0;
    m_brdfLUTTexture = 0;

    m_envFramebuffer = 0;
    m_envRenderbuffer = 0;
    m_faces = {};
    
    m_shaderProgram = nullptr;
    m_pEquirectangularCube = nullptr;
    m_irradianceCube = nullptr;
    m_prefilterCube = nullptr;
    m_brdfLUTCube = nullptr;
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
    glGenTextures(1, &m_skyTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyTexture);
    
    GLint iWidth, iHeight, iChannels;
    BYTE *data = nullptr;
    for (GLuint i = 0; i < cubemapFaces.size(); i++)
    {
        std::string face = cubemapFaces[i];
        LoadTexture(face, &data, iWidth, iHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

        if (data != NULL) delete[] data;
    }

    glGenSamplers(1, &m_skySampler);
    glSamplerParameteri(m_skySampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_skySampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glSamplerParameteri(m_skySampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_skySampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_skySampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CCubemap::LoadHRDCubemap(const int &width, const int &height, const TextureType &type, std::vector <CShaderProgram *> *shaderPrograms, IMaterials *mat, const std::string &equirectangularCubmapPath, const std::string &equirectangularCubmap, const TextureType &equirectangularTexturetype) {
  
    m_faces = {};
    m_type = type;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    /*
     To convert an equirectangular image into a cubemap we need to render a (unit) cube and project the equirectangular map on all of the cube's faces from the inside and take 6 images of each of the cube's sides as a cubemap face. The vertex shader of this cube simply renders the cube as is and passes its local position to the fragment shader as a 3D sample vector:
     
     */
    
    /// Create a framebuffer object and bind it with
    glGenFramebuffers(1, &m_envFramebuffer);
    
    // To bind the framebuffer we use glBindFramebuffer:
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    
    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    glGenTextures(1, &m_envTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envTexture);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glGenSamplers(1, &m_envSampler);
    glSamplerParameteri(m_envSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_envSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glSamplerParameteri(m_envSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_envSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_envSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
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
    
    // Last thing left to do is actually attach the renderbuffer object:
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_envRenderbuffer);
    
    // Once we've allocated enough memory for the renderbuffer object we can unbind the renderbuffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    
    m_shaderProgram = (*shaderPrograms)[77]; // equirectangularProgram
    mat->SetMaterialUniform(m_shaderProgram, "material", glm::vec4(1.0f), 32.0f, 1.0f, false, glm::vec4(1.0f));
    
    int iTextureUnit = static_cast<int>(equirectangularTexturetype); // cubemap
    for (unsigned int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envTexture, 0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0f);
        
        glm::mat4 view = captureViews[i];
        m_shaderProgram->UseProgram();
        m_shaderProgram->SetUniform("material.emissionMap", iTextureUnit);
        m_shaderProgram->SetUniform("matrices.projMatrix", captureProjection);
        m_shaderProgram->SetUniform("matrices.viewMatrix", view);
        
        m_pEquirectangularCube = new CEquirectangularCube(1.0f);
        m_pEquirectangularCube->Create(equirectangularCubmapPath, {
            { equirectangularCubmap, equirectangularTexturetype }
        } );
        m_pEquirectangularCube->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
        m_pEquirectangularCube->Render();
        
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

void CCubemap::LoadIrradianceCubemap(const int &width, const int &height, const TextureType &type, std::vector <CShaderProgram *> *shaderPrograms, IMaterials *mat, const std::string &equirectangularCubmapPath, const std::string &equirectangularCubmap, const TextureType &equirectangularTexturetype) {
    
    m_faces = {};
    m_type = type;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    /// Create a framebuffer object and bind it with
    glGenFramebuffers(1, &m_envFramebuffer);
    
    // To bind the framebuffer we use glBindFramebuffer:
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    
    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    glGenTextures(1, &m_envTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envTexture);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glGenSamplers(1, &m_envSampler);
    glSamplerParameteri(m_envSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_envSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glSamplerParameteri(m_envSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_envSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_envSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
    
    // Last thing left to do is actually attach the renderbuffer object:
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_envRenderbuffer);
    
    // Once we've allocated enough memory for the renderbuffer object we can unbind the renderbuffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    
    m_shaderProgram = (*shaderPrograms)[77]; // equirectangularProgram
    mat->SetMaterialUniform(m_shaderProgram, "material", glm::vec4(1.0f), 32.0f, 1.0f, false, glm::vec4(1.0f));
    int iTextureUnit = static_cast<int>(equirectangularTexturetype); // cubemap
    for (unsigned int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envTexture, 0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0f);
        
        glm::mat4 view = captureViews[i];
        m_shaderProgram->UseProgram();
        m_shaderProgram->SetUniform("material.emissionMap", iTextureUnit);
        m_shaderProgram->SetUniform("matrices.projMatrix", captureProjection);
        m_shaderProgram->SetUniform("matrices.viewMatrix", view);
        
        m_pEquirectangularCube = new CEquirectangularCube(1.0f);
        m_pEquirectangularCube->Create(equirectangularCubmapPath, {
            { equirectangularCubmap, equirectangularTexturetype }
        } );
        m_pEquirectangularCube->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
        m_pEquirectangularCube->Render();
        
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envTexture);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_envRenderbuffer);
    
    glGenTextures(1, &m_irrTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irrTexture);
    
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glGenSamplers(1, &m_irrSampler);
    glSamplerParameteri(m_irrSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_irrSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(m_irrSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_irrSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_irrSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
    
    
    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    GLint irrTextureUnit = static_cast<GLint>(type); // cubemap
    BindEnvCubemapTexture(irrTextureUnit);
    
    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    
    m_irradianceCube = new CEquirectangularCube(1.0f);
    m_irradianceCube->Create("", {});
    m_irradianceCube->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    
    m_shaderProgram = (*shaderPrograms)[78]; // irradianceMapProgram
    mat->SetMaterialUniform(m_shaderProgram, "material", glm::vec4(1.0f), 32.0f, 1.0f, false, glm::vec4(1.0f));
    for (unsigned int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irrTexture, 0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0f);
        
        glm::mat4 view = captureViews[i];
        m_shaderProgram->UseProgram();
        m_shaderProgram->SetUniform("material.cubeMap", irrTextureUnit);
        m_shaderProgram->SetUniform("matrices.projMatrix", captureProjection);
        m_shaderProgram->SetUniform("matrices.viewMatrix", view);
        
        m_irradianceCube->Render(false);
        
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
    // --------------------------------------------------------------------------------
    glGenTextures(1, &m_prefilterTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterTexture);
    
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glGenSamplers(1, &m_prefilterSampler);
    glSamplerParameteri(m_prefilterSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_prefilterSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(m_prefilterSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_prefilterSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_prefilterSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    // ----------------------------------------------------------------------------------------------------
    GLint prefilterTextureUnit = static_cast<GLint>(type); // cubemap
    BindEnvCubemapTexture(prefilterTextureUnit);
    
    glViewport(0, 0, 128, 128);
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    
    m_prefilterCube = new CEquirectangularCube(1.0f);
    m_prefilterCube->Create("", {});
    m_prefilterCube->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    
    m_shaderProgram = (*shaderPrograms)[79]; // prefilterProgram
    mat->SetMaterialUniform(m_shaderProgram, "material", glm::vec4(1.0f), 32.0f, 1.0f, false, glm::vec4(1.0f));
    unsigned int maxMipLevels = 40;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth  = 128 * std::pow(0.5f, mip);
        unsigned int mipHeight = 128 * std::pow(0.5f, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, m_envRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        
        glViewport(0, 0, mipWidth, mipHeight);
        
        float roughness = (float)mip / (float)(maxMipLevels - 1);
        m_shaderProgram->UseProgram();
        m_shaderProgram->SetUniform("roughness", roughness);
        m_shaderProgram->SetUniform("resolution", width);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilterTexture, mip);
        
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearDepth(1.0f);
            
            glm::mat4 view = captureViews[i];
            m_shaderProgram->SetUniform("material.cubeMap", prefilterTextureUnit);
            m_shaderProgram->SetUniform("matrices.projMatrix", captureProjection);
            m_shaderProgram->SetUniform("matrices.viewMatrix", view);
            
            m_prefilterCube->Render(false);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    // pbr: generate a 2D LUT from the BRDF equations used.
    // ----------------------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_envRenderbuffer);
    
    glGenTextures(1, &m_brdfLUTTexture);
    glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);
    
    // pre-allocate enough memory for the LUT texture.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, 0);
    
    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
    glGenSamplers(1, &m_brdfLUTSampler);
    glSamplerParameteri(m_brdfLUTSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_brdfLUTSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_brdfLUTSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_brdfLUTSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfLUTTexture, 0);
    
    glViewport(0, 0, width, height);
    
    m_shaderProgram = (*shaderPrograms)[80]; // m_brdfLUTProgram
    mat->SetMaterialUniform(m_shaderProgram, "material", glm::vec4(1.0f), 32.0f, 1.0f, false, glm::vec4(1.0f));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);
    
    m_brdfLUTCube = new CEquirectangularCube(1.0f);
    m_brdfLUTCube->Create("", {});
    m_brdfLUTCube->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    m_brdfLUTCube->Render(false);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
     
    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    glDisable(GL_DEPTH_TEST);
}

// Binds texture for rendering
void CCubemap::BindCubemapTexture(GLint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyTexture);
    glBindSampler(iTextureUnit, m_skySampler);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

// Binds a environment mapping texture for rendering
void CCubemap::BindEnvCubemapTexture(GLint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envTexture);
    glBindSampler(iTextureUnit, m_envSampler);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

// Binds irradiance map texture for rendering
void CCubemap::BindIrrCubemapTexture(GLint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irrTexture);
    glBindSampler(iTextureUnit, m_irrSampler);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

// Binds prefilter map texture for rendering
void CCubemap::BindPrefilterCubemapTexture(GLint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterTexture);
    glBindSampler(iTextureUnit, m_prefilterSampler);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

// Binds a BRDF texture for rendering
void CCubemap::BindBRDFLUTTexture(GLint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_3D, m_brdfLUTTexture);
    glBindSampler(iTextureUnit, m_brdfLUTSampler);
}


TextureType CCubemap::GetType() const {
    return m_type;
}

// Clear resources
void CCubemap::Clear()
{
    glDeleteSamplers(1, &m_skySampler);
    glDeleteTextures(1, &m_skyTexture);
    
    glDeleteSamplers(1, &m_envSampler);
    glDeleteTextures(1, &m_envTexture);
    
    glDeleteSamplers(1, &m_irrSampler);
    glDeleteTextures(1, &m_irrTexture);
    
    glDeleteSamplers(1, &m_prefilterSampler);
    glDeleteTextures(1, &m_prefilterTexture);
    
    glDeleteSamplers(1, &m_brdfLUTSampler);
    glDeleteTextures(1, &m_brdfLUTTexture);
    
    glDeleteTextures(1, &m_envFramebuffer);
    glDeleteTextures(1, &m_envRenderbuffer);
    m_faces.clear();
    
    if (m_pEquirectangularCube != nullptr) m_pEquirectangularCube = nullptr;
    if (m_irradianceCube != nullptr) m_irradianceCube = nullptr;
    if (m_prefilterCube != nullptr) m_prefilterCube = nullptr;
    if (m_brdfLUTCube != nullptr) m_brdfLUTCube = nullptr;
}

// Release resources
void CCubemap::Release()
{
    glDeleteSamplers(1, &m_skySampler);
    glDeleteTextures(1, &m_skyTexture);
    
	glDeleteSamplers(1, &m_envSampler);
	glDeleteTextures(1, &m_envTexture);
    
    glDeleteSamplers(1, &m_irrSampler);
    glDeleteTextures(1, &m_irrTexture);
    
    glDeleteSamplers(1, &m_prefilterSampler);
    glDeleteTextures(1, &m_prefilterTexture);
    
    glDeleteSamplers(1, &m_brdfLUTSampler);
    glDeleteTextures(1, &m_brdfLUTTexture);
    
    glDeleteTextures(1, &m_envFramebuffer);
    glDeleteTextures(1, &m_envRenderbuffer);
    m_faces.clear();
    
    delete m_pEquirectangularCube;
    delete m_irradianceCube;
    delete m_prefilterCube;
    delete m_brdfLUTCube;
    delete m_shaderProgram;
}
