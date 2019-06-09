#include "Skybox.h"

//https://stackoverflow.com/questions/11685608/convention-of-faces-in-opengl-cubemapping
// https://www.flickr.com/groups/353787@N23/pool/page3
// http://www.custommapmakers.org/skyboxes.php
// https://jaxry.github.io/panorama-to-cubemap/
// http://texturify.com/category/environment-panoramas.html
// http://www.zbrushcentral.com/showthread.php?192249-100-Free-Spherical-Environment-Maps-amp-200-Sky-Backgrounds-amp-1000-Textures

CSkybox::CSkybox()
{
    m_vao = 0;
    m_envFramebuffer = 0;
    m_envCubemap = 0;
    m_envRenderbuffer = 0;
    m_useEnvCubemap = false;
    m_skyboxes = {};
}

CSkybox::~CSkybox()
{
    Release();
}

// Create a skybox
void CSkybox::Create(const GLfloat &size, const std::string &path, const TextureType &type, const GLboolean &useEnvCubemap, CShaderProgram *equirectangularProgram, const TextureType &equirectangularTexturetype, const GLuint &skyboxNumber)
{
    m_useEnvCubemap = useEnvCubemap;
    if (useEnvCubemap == false) {
    /*
         A skybox is a (large) cube that encompasses the entire scene and contains 6 images of a surrounding environment, giving the player the illusion that the environment he's in is actually much larger than it actually is. Some examples of skyboxes used in videogames are images of mountains, of clouds or of a starry night sky. 
     
            ________
            |        |
            | pos y  |
            |   up   |
     _______|________|_________________
    |       |        |        |        |
    | neg x | pos z  |  pos x |  neg z |
    | rt    |   bk   |  lf    |   ft   |
    |_______|________|________|________|
            |        |
            | neg y  |
            |   dn   |
            |________|
     
     /////--------------/
     ////  .     T     // |
     ////--------------   |
     //    .      B   |   |
     //    .          |   |
     // L  .          | R |
     //    .   F      |   |
     //    .......... |   /
     //  /     D      | //
     ///--------------/
    
     */
    
        /// http://www.custommapmakers.org/skyboxes.php
        m_skyboxes = {
            //"colorbasement",
            //"commonroom",
            //"deserthighway",
            //"diningroom",
            //"fog",
            //"petrolstation",
            //"skywater",
            //"tokyobigsight",
            //"valley",
            "winterseashore"
            //"yokohamaday",
            //"yokohamanight"
        };
        
        unsigned int ind = skyboxNumber % m_skyboxes.size();
        
        m_cubemapTexture.LoadCubemap({
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_rt.jpg", //right
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_lf.jpg", //left
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_up.jpg", //up
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_dn.jpg", //down
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_bk.jpg", //back
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_ft.jpg",  //front
                                    }, type);
    
    
    
    } else {
    
        /*
         To convert an equirectangular image into a cubemap we need to render a (unit) cube and project the equirectangular map on all of the cube's faces from the inside and take 6 images of each of the cube's sides as a cubemap face. The vertex shader of this cube simply renders the cube as is and passes its local position to the fragment shader as a 3D sample vector:
         
         */
        
        
        int width = (int)size;
        int height = (int)size;
        
        /// Create a framebuffer object and bind it with
        glGenFramebuffers(1, &m_envFramebuffer);
        
        // To bind the framebuffer we use glBindFramebuffer:
        glBindFramebuffer(GL_FRAMEBUFFER, m_envFramebuffer);
        
        // pbr: setup cubemap to render to and attach to framebuffer
        // ---------------------------------------------------------
        //unsigned int envCubemap;
        glGenTextures(1, &m_envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
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
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envCubemap, 0);
            
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
            m_pEquirectangularCube->Create(path+"/skyboxes/deserthighway/", {
                { "Road_to_MonumentValley_Ref.hdr", equirectangularTexturetype }
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
    
    
    
    CreateAttributes(size);
}


void CSkybox::CreateAttributes(const GLfloat &size) {
    
     
     glGenVertexArrays(1, &m_vao);
     glBindVertexArray(m_vao);
     
     m_vbo.Create();
     m_vbo.Bind();
     
     glm::vec3 vSkyBoxVertices[24] =
     {
         // Front face
         glm::vec3(size, size, size), glm::vec3(size, -size, size), glm::vec3(-size, size, size), glm::vec3(-size, -size, size),
         // Back face
         glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size), glm::vec3(size, size, -size), glm::vec3(size, -size, -size),
         // Left face
         glm::vec3(-size, size, size), glm::vec3(-size, -size, size), glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size),
         // Right face
         glm::vec3(size, size, -size), glm::vec3(size, -size, -size), glm::vec3(size, size, size), glm::vec3(size, -size, size),
         // Top face
         glm::vec3(-size, size, -size), glm::vec3(size, size, -size), glm::vec3(-size, size, size), glm::vec3(size, size, size),
         // Bottom face
         glm::vec3(size, -size, -size), glm::vec3(-size, -size, -size), glm::vec3(size, -size, size), glm::vec3(-size, -size, size),
     };
     glm::vec2 vSkyBoxTexCoords[4] =
     {
        glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
     };
     
     glm::vec3 vSkyBoxNormals[6] =
     {
         glm::vec3(0.0f, 0.0f, -1.0f),
         glm::vec3(0.0f, 0.0f, 1.0f),
         glm::vec3(1.0f, 0.0f, 0.0f),
         glm::vec3(-1.0f, 0.0f, 0.0f),
         glm::vec3(0.0f, -1.0f, 0.0f),
         glm::vec3(0.0f, 1.0f, 0.0f)
     };
     
     glm::vec4 vColour = glm::vec4(1, 1, 1, 1);
     for (int i = 0; i < 24; i++) {
         m_vbo.AddData(&vSkyBoxVertices[i], sizeof(glm::vec3));
         m_vbo.AddData(&vSkyBoxTexCoords[i%4], sizeof(glm::vec2));
         m_vbo.AddData(&vSkyBoxNormals[i/4], sizeof(glm::vec3));
     }
     
     m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
     
     // Set the vertex attribute locations
     GLsizei istride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);
     
     // Vertex positions
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
     // Texture coordinates
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
     // Normal vectors
     glEnableVertexAttribArray(2);
     glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
}


void CSkybox::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    // Render the planar terrain
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

// Render the skybox
void CSkybox::Render(const GLboolean &useTexture) {
    
    glDepthMask(GL_FALSE);
    glBindVertexArray(m_vao);
    
    if (m_useEnvCubemap) {
        glActiveTexture(GL_TEXTURE0+static_cast<GLint>(TextureType::CUBEMAP));
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);
        //glBindSampler(textureUnit, m_uiSampler);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        m_cubemapTexture.Bind(static_cast<GLint>(m_cubemapTexture.GetType()));
    }
    
    for (int i = 0; i < 6; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
    }
    glBindVertexArray(0);
    
    glDepthMask(GL_TRUE);
}

void CSkybox::BindSkyboxTo(const GLint &textureUnit){
    m_cubemapTexture.Bind(textureUnit);	
}

GLuint CSkybox::GetNumberOfSkyboxes() const{
    return m_skyboxes.size();
}
GLboolean CSkybox::IsEnvCubemap() const {
    return m_useEnvCubemap;
}

std::vector<std::string> CSkybox::GetSkyboxes() const {
    return m_skyboxes;
}

// Release the storage assocaited with the skybox
void CSkybox::Release()
{
	m_cubemapTexture.Release();
   
    glDeleteTextures(1, &m_envFramebuffer);
    glDeleteTextures(1, &m_envCubemap);
    glDeleteTextures(1, &m_envRenderbuffer);
    
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}
