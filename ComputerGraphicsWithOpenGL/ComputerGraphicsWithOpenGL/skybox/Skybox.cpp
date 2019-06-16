#include "Skybox.h"

//https://stackoverflow.com/questions/11685608/convention-of-faces-in-opengl-cubemapping
// https://www.flickr.com/groups/353787@N23/pool/page3
// http://www.custommapmakers.org/skyboxes.php
// https://jaxry.github.io/panorama-to-cubemap/
// http://texturify.com/category/environment-panoramas.html
// http://www.zbrushcentral.com/showthread.php?192249-100-Free-Spherical-Environment-Maps-amp-200-Sky-Backgrounds-amp-1000-Textures

CSkybox::CSkybox()
{
    m_useHDRTexture = false;
    m_cubemapTexture = new CCubemap();
    m_vao = 0;
    m_skyboxes = {};
}

CSkybox::~CSkybox()
{
    Release();
}

// Create a skybox
void CSkybox::Create(const GLfloat &size, const std::string &path, const TextureType &type, const GLboolean &useEnvCubemap, CShaderProgram *equirectangularProgram, const TextureType &equirectangularTexturetype, const GLuint &skyboxNumber)
{
    m_useHDRTexture = useEnvCubemap;
    if (m_useHDRTexture == false) {
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
            "winterseashore",
            "yokohamaday",
            "yokohamanight"
        };
        
        unsigned int ind = skyboxNumber % m_skyboxes.size();
        
        m_cubemapTexture->LoadCubemap({
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_rt.jpg", //right
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_lf.jpg", //left
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_up.jpg", //up
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_dn.jpg", //down
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_bk.jpg", //back
                                    path+"/skyboxes/"+m_skyboxes[ind]+"/flipped/_ft.jpg",  //front
                                    }, type);
    } else {
    
        m_cubemapTexture->LoadHRDCubemap((int)size, (int)size, type, equirectangularProgram, path+"/skyboxes/deserthighway/", "Road_to_MonumentValley_Ref.hdr", equirectangularTexturetype);
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
    
    GLint iTextureUnit = static_cast<GLint>(m_cubemapTexture->GetType());
    m_cubemapTexture->BindCubemapTexture(iTextureUnit);
    
    for (int i = 0; i < 6; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
    }
    glBindVertexArray(0);
    
    glDepthMask(GL_TRUE);
}

void CSkybox::BindSkyboxTo(const GLint &textureUnit){
    m_cubemapTexture->BindCubemapTexture(textureUnit);
}

GLuint CSkybox::GetNumberOfSkyboxes() const{
    return m_skyboxes.size();
}

std::vector<std::string> CSkybox::GetSkyboxes() const {
    return m_skyboxes;
}

GLboolean CSkybox::IsEnvCubemap() const {
    return m_useHDRTexture;
}

// Release the storage assocaited with the skybox
void CSkybox::Release()
{
    glDeleteVertexArrays(1, &m_vao);
    m_vbo.Release();
    
    m_cubemapTexture->Release();
    delete m_cubemapTexture;
}
