//
//  Quad.cpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 28/09/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Quad.h"

CQuad::CQuad()
{
    
}

CQuad::~CQuad()
{
    Release();
}

void CQuad::Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames,
                   const GLfloat &width, const GLfloat &height){
    
    m_textureNames = textureNames;
    m_textures.reserve(textureNames.size());
    
    // Iterate through all elements in std::map
    for (auto it = textureNames.begin(); it != textureNames.end(); ++it) {
        // if the current index is needed:
        auto i = std::distance(textureNames.begin(), it);
        
        // access element as *it
        m_textures.push_back(new CTexture);
        m_textures[i]->LoadTexture(directory+it->first, it->second, true);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        // any code including continue, break, return
    }

    
    // Use VAO to store state associated with vertices
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    // Create a VBO
    m_vbo.Create();
    m_vbo.Bind();
    
    //A//////////B
    //
    ////Center///
    //
    //C//////////D
    GLfloat w = width;
    GLfloat h = height;
    
    std::vector<glm::vec3> quadVertices = {
        glm::vec3(-w, h, 0.0f),//A   top left
        glm::vec3(-w, -h, 0.0f), //C  bottom left
        glm::vec3(w, -h, 0.0f),//D    bottom right
        
        glm::vec3(-w, h, 0.0f),//A    top left
        glm::vec3(w, -h, 0.0f),//D   bottom left
        glm::vec3(w, h, 0.0f)//B      top right
    };
    m_totalVertices = quadVertices.size();
    
    std::vector<glm::vec2>  quadTexCoord = {
        glm::vec2(0.0f, 1.0f),//A
        glm::vec2(0.0f, 0.0f),//C
        glm::vec2(1.0f, 0.0f),//D
        glm::vec2(0.0f, 1.0f),//A
        glm::vec2(1.0f, 0.0f),//D
        glm::vec2(1.0f, 1.0f)//B
    };
   
    std::vector<glm::vec3>  quadNormals = {
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
    };

    for (int i = 0; i < m_totalVertices; i += 2) {
        
      
        // triangle
        glm::vec3 v0 = quadVertices[i+0];
        glm::vec3 v1 = quadVertices[i+1 % m_totalVertices];
        
        // triangle UVs
        glm::vec2 uv0 = quadTexCoord[i+0];
        glm::vec2 uv1 = quadTexCoord[i+1 % m_totalVertices];
        
        glm::vec3 norm0 = quadNormals[i+0];
        glm::vec3 norm1 = quadNormals[i+1 % m_totalVertices];
        
        
        glm::vec3 tangent, bitangent;
        
        float coef = 1.0f / (uv0.x * uv1.y - uv1.x * uv0.y);
        tangent.x = coef * ((v0.x * uv1.y) + (v1.x * -uv0.y));
        tangent.y = coef * ((v0.y * uv1.y) + (v1.y * -uv0.y));
        tangent.z = coef * ((v0.z * uv1.y) + (v1.z * -uv0.y));
        tangent = glm::normalize(tangent);
        bitangent = glm::normalize(glm::cross(norm0, tangent));
        
        m_vbo.AddData(&v0, sizeof(glm::vec3));
        m_vbo.AddData(&uv0, sizeof(glm::vec2));
        m_vbo.AddData(&norm0, sizeof(glm::vec3));
        m_vbo.AddData(&tangent, sizeof(glm::vec3));
        m_vbo.AddData(&bitangent, sizeof(glm::vec3));
        
        bitangent = glm::normalize(glm::cross(norm1, tangent));
        
        m_vbo.AddData(&v1, sizeof(glm::vec3));
        m_vbo.AddData(&uv1, sizeof(glm::vec2));
        m_vbo.AddData(&norm1, sizeof(glm::vec3));
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

void CQuad::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

// Render the quad as a triangle strip
void CQuad::Render(const GLboolean &useTexture)
{
    glBindVertexArray(m_vao);
    if (useTexture){
        for (GLuint i = 0; i < m_textures.size(); ++i){
            m_textures[i]->BindTexture2DToTextureType();
        }
    }
    
    glDrawArrays(GL_TRIANGLES, 0, m_totalVertices);
}

// Release resources
void CQuad::Release()
{
    for (GLuint i = 0; i < m_textures.size(); ++i){
        m_textures[i]->Release();
        delete m_textures[i];
    }
    m_textures.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    
    m_vbo.Release();
}
