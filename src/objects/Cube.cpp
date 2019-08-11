//
//  Cube.cpp
//  OpenGL_Course_Work
//
//  Created by GEORGE QUENTIN on 15/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//

#include "Cube.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

CCube::CCube(const GLfloat & size){
    this->size = size;
}

CCube::~CCube()
{
    Release();
}

void CCube::Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames) {
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
        
        // any code including continue, break, return
    }
    
    // make and bind the VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    m_vbo.Create();
    m_vbo.Bind();
    
    
    /////6--------------/5
    ////  .           // |
    //2--------------1   |          // back      4  7  6  5   normal -z
    //    .          |   |          // front     3  0  1  2   normal +z
    //    .          |   |          // top       2  1  5  6   normal +y
    //    .          |   |          // bottom    0  3  7  4   normal -y
    //    .          |   |          // left      7  3  2  6   normal +x
    //    7......... |   /4         // right     0  4  5  1   normal -x
    //               | //
    //3--------------/0
    
    ///--------------1
    //               |
    //               |
    //               |   y
    //               |
    //               |
    //               |
    //0--------------
    //      x
    
     std::vector<glm::vec3> cubeVertices = {
     
     
     // positions          // texture Coords
     //back
     glm::vec3(size, -size, -size),//4
     glm::vec3(-size, -size, -size),//7
     glm::vec3(-size,  size, -size),//6
     glm::vec3(-size,  size, -size),//6
     glm::vec3(size,  size, -size),//5
     glm::vec3(size, -size, -size),//4
     
     
     //front t1 = 3,0,1  t2=1,2,3
     //front
     glm::vec3(-size, -size,  size),//3
     glm::vec3(size, -size,  size), //0
     glm::vec3(size,  size,  size), //1
     glm::vec3(size,  size,  size), //1
     glm::vec3(-size,  size,  size),//2
     glm::vec3(-size, -size,  size),//3
     
     //right
     glm::vec3(-size, -size, -size),//7
     glm::vec3(-size, -size,  size),//3
     glm::vec3(-size,  size,  size),//2
     glm::vec3(-size,  size,  size),//2
     glm::vec3(-size,  size, -size),//6
     glm::vec3(-size, -size, -size),//7
     
     //left
     glm::vec3(size, -size,  size),//0
     glm::vec3(size, -size, -size),//4
     glm::vec3(size,  size, -size),//5
     glm::vec3(size,  size, -size),//5
     glm::vec3(size,  size,  size),//1
     glm::vec3(size, -size,  size),//0
     
     
     //bottom
     glm::vec3(-size, -size, -size),//7
     glm::vec3(size, -size, -size), //4
     glm::vec3(size, -size,  size),//0
     glm::vec3(size, -size,  size),//0
     glm::vec3(-size, -size,  size),//3
     glm::vec3(-size, -size, -size),//7
     
     //top
     glm::vec3(size,  size,  size), // 1
     glm::vec3(size,  size, -size), // 5
     glm::vec3( -size,  size, -size),//6
     glm::vec3(-size,  size, -size), // 6
     glm::vec3(-size,  size,  size),//2
     glm::vec3(size,  size,  size), //1
     };
     
     
     
     // set up vertex data (and buffer(s)) and configure vertex attributes
     // ------------------------------------------------------------------
     std::vector<glm::vec3> cubeNormals = {
     // normals
     glm::vec3(0.0f,  0.0f, -1.0f), //back
     glm::vec3(0.0f,  0.0f, -1.0f),
     glm::vec3(0.0f,  0.0f, -1.0f),
     glm::vec3(0.0f,  0.0f, -1.0f),
     glm::vec3(0.0f,  0.0f, -1.0f),
     glm::vec3(0.0f,  0.0f, -1.0f),
     
     glm::vec3(0.0f,  0.0f, 1.0f), //front
     glm::vec3(0.0f,  0.0f, 1.0f),
     glm::vec3(0.0f,  0.0f, 1.0f),
     glm::vec3(0.0f,  0.0f, 1.0f),
     glm::vec3(0.0f,  0.0f, 1.0f),
     glm::vec3(0.0f,  0.0f, 1.0f),
     
     glm::vec3(-1.0f,  0.0f,  0.0f), // right
     glm::vec3(-1.0f,  0.0f,  0.0f),
     glm::vec3(-1.0f,  0.0f,  0.0f),
     glm::vec3(-1.0f,  0.0f,  0.0f),
     glm::vec3(-1.0f,  0.0f,  0.0f),
     glm::vec3(-1.0f,  0.0f,  0.0f),
     
     glm::vec3(1.0f,  0.0f,  0.0f), // left
     glm::vec3(1.0f,  0.0f,  0.0f),
     glm::vec3(1.0f,  0.0f,  0.0f),
     glm::vec3(1.0f,  0.0f,  0.0f),
     glm::vec3(1.0f,  0.0f,  0.0f),
     glm::vec3(1.0f,  0.0f,  0.0f),
     
     glm::vec3(0.0f, -1.0f,  0.0f), // down
     glm::vec3(0.0f, -1.0f,  0.0f),
     glm::vec3(0.0f, -1.0f,  0.0f),
     glm::vec3(0.0f, -1.0f,  0.0f),
     glm::vec3(0.0f, -1.0f,  0.0f),
     glm::vec3(0.0f, -1.0f,  0.0f),
     
     glm::vec3(0.0f,  1.0f,  0.0f), // up
     glm::vec3(0.0f,  1.0f,  0.0f),
     glm::vec3(0.0f,  1.0f,  0.0f),
     glm::vec3(0.0f,  1.0f,  0.0f),
     glm::vec3(0.0f,  1.0f,  0.0f),
     glm::vec3(0.0f,  1.0f,  0.0f)
     };
     
     
     
     std::vector<glm::vec2> cubeTextCoords = {
     
     // positions          // texture Coords
     
     //back
     glm::vec2(0.0f, 0.0f),
     glm::vec2(1.0f, 0.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(0.0f, 1.0f),
     glm::vec2(0.0f, 0.0f),
     
     //front
     glm::vec2(0.0f, 0.0f),
     glm::vec2(1.0f, 0.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(0.0f, 1.0f),
     glm::vec2(0.0f, 0.0f),
     
     //right
     glm::vec2(0.0f, 0.0f),
     glm::vec2(1.0f, 0.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(0.0f, 1.0f),
     glm::vec2(0.0f, 0.0f),
     
     //left
     glm::vec2(0.0f, 0.0f),
     glm::vec2(1.0f, 0.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(0.0f, 1.0f),
     glm::vec2(0.0f, 0.0f),
     
     //bottom
     glm::vec2(0.0f, 0.0f),
     glm::vec2(1.0f, 0.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(1.0f, 1.0f),
     glm::vec2(0.0f, 1.0f),
     glm::vec2(0.0f, 0.0f),
     
     //top
     glm::vec2(1.0f, 0.0f),//1
     glm::vec2(1.0f, 1.0f),//5
     glm::vec2(0.0f, 1.0f),//6
     glm::vec2(0.0f, 1.0f),//6
     glm::vec2(0.0f, 0.0f),//2
     glm::vec2(1.0f, 0.0f),//1
     };
    
    //compute tangent and bitangent
    // https://learnopengl.com/#!Advanced-Lighting/Normal-Mapping
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
    // https://www.keithlantz.net/2011/10/tangent-space-normal-mapping-with-glsl/
    // https://www.opengl.org/discussion_boards/showthread.php/154967-Finding-the-tangent-vector
    // either do this (i += 3) or do ( (i % 3) == 0 ): to iterate through 3 vertices at a time.
    for (int i = 0; i < cubeVertices.size(); i+=3) {
        
        // triangle
        glm::vec3 v0 = cubeVertices[i+0];
        glm::vec3 v1 = cubeVertices[i+1];
        glm::vec3 v2 = cubeVertices[i+2];

        // triangle UVs
        glm::vec2 & uv0 = cubeTextCoords[i+0];
        glm::vec2 & uv1 = cubeTextCoords[i+1];
        glm::vec2 & uv2 = cubeTextCoords[i+2];
        
        // triangle Normals
        glm::vec3 & norm0 = cubeNormals[i+0];
        glm::vec3 & norm1 = cubeNormals[i+1];
        glm::vec3 & norm2 = cubeNormals[i+2];
        
        glm::vec3 tangent, bitangent;
        
        // calculate the first triangle's edges and delta UV coordinates
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;
    
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);
        m_cubeTangent.push_back(tangent);
        m_cubeTangent.push_back(tangent);
        m_cubeTangent.push_back(tangent);
    
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        m_cubeBitangent.push_back(bitangent);
        m_cubeBitangent.push_back(bitangent);
        m_cubeBitangent.push_back(bitangent);
        
        m_vbo.AddData(&v0, sizeof(glm::vec3));
        m_vbo.AddData(&uv0, sizeof(glm::vec2));
        m_vbo.AddData(&norm0, sizeof(glm::vec3));
        m_vbo.AddData(&tangent, sizeof(glm::vec3));
        m_vbo.AddData(&bitangent, sizeof(glm::vec3));
        
        m_vbo.AddData(&v1, sizeof(glm::vec3));
        m_vbo.AddData(&uv1, sizeof(glm::vec2));
        m_vbo.AddData(&norm1, sizeof(glm::vec3));
        m_vbo.AddData(&tangent, sizeof(glm::vec3));
        m_vbo.AddData(&bitangent, sizeof(glm::vec3));
        
        m_vbo.AddData(&v2, sizeof(glm::vec3));
        m_vbo.AddData(&uv2, sizeof(glm::vec2));
        m_vbo.AddData(&norm2, sizeof(glm::vec3));
        m_vbo.AddData(&tangent, sizeof(glm::vec3));
        m_vbo.AddData(&bitangent, sizeof(glm::vec3));
    }
    
    m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

    m_numTriangles = cubeVertices.size();
   
    // 4 vector3 plus 1 vector2
    // https://stackoverflow.com/questions/22296510/what-does-stride-mean-in-opengles
    // https://www.opengl.org/discussion_boards/showthread.php/156620-glVertexPointer-Stride
	GLsizei stride = (4 * sizeof(glm::vec3)) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    
    // Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)));
    
    // Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(glm::vec3)));
}

void CCube::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

// Render the cube as a set of triangles
void CCube::Render(const GLboolean &useTexture)
{
    // bind the VAO (the triangle)
    glBindVertexArray(m_vao);
    
    if (useTexture){
        for (GLuint i = 0; i < m_textures.size(); ++i){
            m_textures[i]->BindTexture2DToTextureType();
        }
    }
    glDrawArrays( GL_TRIANGLES, 0, m_numTriangles ); // draw the vertixes
    
    glBindVertexArray(0);
    
}

// Release memory on the GPU 
void CCube::Release()
{
    for (GLuint i = 0; i < m_textures.size(); ++i){
        m_textures[i]->Release();
        delete m_textures[i];
    }
    m_textures.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    
    m_vbo.Release();
    
}
