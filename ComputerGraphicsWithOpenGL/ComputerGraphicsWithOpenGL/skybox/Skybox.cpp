#include "Skybox.h"

//https://stackoverflow.com/questions/11685608/convention-of-faces-in-opengl-cubemapping
// https://www.flickr.com/groups/353787@N23/pool/page3
// http://www.custommapmakers.org/skyboxes.php
// https://jaxry.github.io/panorama-to-cubemap/
// http://texturify.com/category/environment-panoramas.html
// http://www.zbrushcentral.com/showthread.php?192249-100-Free-Spherical-Environment-Maps-amp-200-Sky-Backgrounds-amp-1000-Textures

CSkybox::CSkybox()
{}

CSkybox::~CSkybox()
{
    Release();
}

// Create a skybox of a given size with six textures
void CSkybox::Create(const float &size, const std::string &path, const GLuint &skyboxNumber)
{
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
    
    std::vector<std::string> names = {
        "colorbasement",
        "commonroom",
        "diningroom",
        "porchfold",
        "darkland",
        "valley",
        "skywater",
        "darksky",
        "sky",
        "landscape",
        "ashcanyon",
        "peninsulatokyo",
        "petrolstation",
        "sahara",
        "yokohamanight",
        "yokohamaday",
    };
    
    unsigned int ind = skyboxNumber % names.size();
    
    m_cubemapTexture.Create(
                            path+"/skyboxes/"+names[ind]+"/flipped/_rt.jpg", //right
                            path+"/skyboxes/"+names[ind]+"/flipped/_lf.jpg", //left
                            path+"/skyboxes/"+names[ind]+"/flipped/_up.jpg", //up
                            path+"/skyboxes/"+names[ind]+"/flipped/_dn.jpg", //down
                            path+"/skyboxes/"+names[ind]+"/flipped/_bk.jpg", //back
                            path+"/skyboxes/"+names[ind]+"/flipped/_ft.jpg"); //front
	
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
    transform.RotateX(rotation.x);
    transform.RotateY(rotation.y);
    transform.RotateZ(rotation.z);
    transform.Scale(scale);
}

void CSkybox::Render(const GLboolean &useTexture) {
    
}

// Render the skybox
void CSkybox::Render(const int &textureUnit)
{
    glDepthMask(GL_FALSE);
	glBindVertexArray(m_vao);
	m_cubemapTexture.Bind(textureUnit);
	for (int i = 0; i < 6; i++) {
		//m_textures[i].Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
    
    glDepthMask(GL_TRUE); 
}

void CSkybox::BindSkybox(const int &textureUnit){
    m_cubemapTexture.Bind(textureUnit);	
}

// Release the storage assocaited with the skybox
void CSkybox::Release()
{
	//for (int i = 0; i < 6; i++)
		//m_textures[i].Release();
	m_cubemapTexture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}
