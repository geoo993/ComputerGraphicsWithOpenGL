
#include "Plane.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


CPlane::CPlane()
{
   
}

CPlane::~CPlane()
{
    Release();
}


void CPlane::Create(float fWidth, float fHeight, float fTextureRepeat, unsigned int fDivisions){
    
    m_width = fWidth;
    m_height = fHeight;
    
    // Use VAO to store state associated with vertices
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    // Create a VBO
    m_vbo.Create();
    m_vbo.Bind();
    
    std::vector<glm::vec3> planeVertices;
    
    float halfWidth = m_width / 2.0f;
    float halfHeight = m_height / 2.0f;
    float rowWidth = m_width/(fDivisions+1);
    float colWidth = m_height/(fDivisions+1);
    
    for (unsigned int c = 0; c < (m_width/rowWidth); c++) {
        if (c>0) {
            if (c%2 == 1) {
                planeVertices.push_back(glm::vec3(c*rowWidth-halfWidth, 0.0f, halfHeight));
            }else{
                planeVertices.push_back(glm::vec3(c*rowWidth-halfWidth, 0.0f, -halfHeight));
            }
        }
        for (unsigned int r = 0; r < (m_height/colWidth)+1; r++) {
            unsigned int row = r;
            if (c%2 == 1) {
                row = (m_height/colWidth) - r;
                planeVertices.push_back(glm::vec3(c*rowWidth-halfWidth, 0.0f, row*colWidth-halfHeight));
                planeVertices.push_back(glm::vec3((c+1)*rowWidth-halfWidth, 0.0f, row*colWidth-halfHeight));
            }else{
                planeVertices.push_back(glm::vec3((c+1)*rowWidth-halfWidth, 0.0f, row*colWidth-halfHeight));
                planeVertices.push_back(glm::vec3(c*rowWidth-halfWidth, 0.0f, row*colWidth-halfHeight));
            }
        }
        if (fDivisions > 0) {
            planeVertices.push_back(planeVertices.back());
        }
    }
    m_totalVertices = planeVertices.size();
    
    // Plane normal
    glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Put the vertex attributes in the VBO
    for (unsigned int i = 0; i < planeVertices.size(); i++) {
        glm::vec2 textCoords(0.0f);
        textCoords.x = ((planeVertices[i].x+halfWidth)/m_width)*fTextureRepeat;
        textCoords.y = ((planeVertices[i].z+halfHeight)/m_height)*fTextureRepeat;
        
        m_vbo.AddData(&planeVertices[i], sizeof(glm::vec3));
        m_vbo.AddData(&textCoords, sizeof(glm::vec2));
        m_vbo.AddData(&planeNormal, sizeof(glm::vec3));
    }
    
    // Upload the VBO to the GPU
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

// Create the plane, including its geometry, texture mapping, normal, and colour
void CPlane::Create(const std::string &fLocation, float fWidth, float fHeight, float fTextureRepeat, unsigned int fDivisions){
    
    Create(fWidth, fHeight, fTextureRepeat, fDivisions);
    
	// Load the texture
	m_texture.Load(fLocation, true);

	// Set parameters for texturing using sampler object
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
    GLfloat aniso;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
    m_texture.SetSamplerObjectParameterf(GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    
}

// Render the plane as a triangle strip
void CPlane::Render(const bool &useTexture)
{
	glBindVertexArray(m_vao);
    if (useTexture) {
        m_texture.BindTexture2D();
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_totalVertices);
}

// Release resources
void CPlane::Release()
{
    m_texture.Release();
    
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}
