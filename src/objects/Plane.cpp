
#include "Plane.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


CPlane::CPlane()
{
    m_width = 0;
    m_height = 0;
    m_totalVertices = 0;
}

CPlane::~CPlane()
{
    Release();
}

void CPlane::Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames,
                    GLfloat fWidth, GLfloat fHeight, GLfloat fTextureRepeat, GLuint fDivisions) {
    m_width = fWidth;
    m_height = fHeight;
    
    m_textureNames = textureNames;
    m_textures.reserve(textureNames.size());
    
    // Iterate through all elements in std::map
    for (auto it = textureNames.begin(); it != textureNames.end(); ++it) {
        // if the current index is needed:
        auto i = std::distance(textureNames.begin(), it);
        
        // access element as *it
        m_textures.push_back(new CTexture);
        m_textures[i]->LoadTexture(directory+it->first, it->second, true);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, m_textures[i]->GetFormat() ==  GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, m_textures[i]->GetFormat() ==  GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        
        // Anisotropic filtering https://paroj.github.io/gltut/Texturing/Tut15%20Anisotropy.html
        GLfloat aniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
        m_textures[i]->SetSamplerObjectParameterf(GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    }
    
    
    
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
    for (unsigned int i = 0; i < m_totalVertices; i+=2) {
        
        // triangle
        glm::vec3 v0 = planeVertices[i+0];
        glm::vec3 v1 = planeVertices[i+1 % m_totalVertices];
        
        // triangle UVs
        glm::vec2 uv0 = glm::vec2(((planeVertices[i+0].x+halfWidth)/m_width)*fTextureRepeat,
                                    ((planeVertices[i+0].z+halfHeight)/m_height)*fTextureRepeat);
        glm::vec2 uv1 = glm::vec2(((planeVertices[i+1 % m_totalVertices].x+halfWidth)/m_width)*fTextureRepeat,
                                    ((planeVertices[i+1 % m_totalVertices].z+halfHeight)/m_height)*fTextureRepeat);
        
        glm::vec3 tangent, bitangent;
        
        float coef = 1.0f / (uv0.x * uv1.y - uv1.x * uv0.y);
        tangent.x = coef * ((v0.x * uv1.y) + (v1.x * -uv0.y));
        tangent.y = coef * ((v0.y * uv1.y) + (v1.y * -uv0.y));
        tangent.z = coef * ((v0.z * uv1.y) + (v1.z * -uv0.y));
        tangent = glm::normalize(tangent);
        bitangent = glm::normalize(glm::cross(planeNormal, tangent));
        
        m_vbo.AddData(&v0, sizeof(glm::vec3));
        m_vbo.AddData(&uv0, sizeof(glm::vec2));
        m_vbo.AddData(&planeNormal, sizeof(glm::vec3));
        m_vbo.AddData(&tangent, sizeof(glm::vec3));
        m_vbo.AddData(&bitangent, sizeof(glm::vec3));
        
        bitangent = glm::normalize(glm::cross(planeNormal, tangent));
        
        m_vbo.AddData(&v1, sizeof(glm::vec3));
        m_vbo.AddData(&uv1, sizeof(glm::vec2));
        m_vbo.AddData(&planeNormal, sizeof(glm::vec3));
        m_vbo.AddData(&tangent, sizeof(glm::vec3));
        m_vbo.AddData(&bitangent, sizeof(glm::vec3));
    }
    
    // Upload the VBO to the GPU
    m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
    
    // Set the vertex attribute locations
    GLsizei istride = 4*sizeof(glm::vec3)+sizeof(glm::vec2);
    
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
    
    // Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)));
    
    // Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(glm::vec3)));
    
}

void CPlane::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    // Render the planar terrain
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

// Render the plane as a triangle strip
void CPlane::Render(const GLboolean &useTexture)
{
	glBindVertexArray(m_vao);
    if (useTexture){
        for (GLuint i = 0; i < m_textures.size(); ++i){
            m_textures[i]->BindTexture2DToTextureType();
        }
    }
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_totalVertices);
}

// Release resources
void CPlane::Release()
{
    for (GLuint i = 0; i < m_textures.size(); ++i){
        m_textures[i]->Release();
        delete m_textures[i];
    }
    m_textures.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    
    m_vbo.Release();
}
