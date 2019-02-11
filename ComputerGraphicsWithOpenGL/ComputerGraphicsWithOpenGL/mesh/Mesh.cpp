
#include "Mesh.h"

Mesh::Mesh()
{
    m_vao = INVALID_OGL_VALUE;
    m_vbo = INVALID_OGL_VALUE;
    m_ibo = INVALID_OGL_VALUE;
    m_numIndices, m_numFaces = 0;
    m_vertices.clear();
    m_textures.clear();
    m_materialIndex = INVALID_MATERIAL;
    
};

Mesh::Mesh(const Mesh &other) {
    this -> m_vao = other.m_vao;
    this -> m_vbo = other.m_vbo;
    this -> m_ibo = other.m_ibo;
    this -> m_numIndices = other.m_numIndices;
    this -> m_numFaces = other.m_numFaces;
    this -> m_materialIndex = other.m_materialIndex;
    this -> m_vertices = other.m_vertices;
    this -> m_textures = other.m_textures;
}

Mesh &Mesh::operator=(const Mesh &other){
    this -> m_vao = other.m_vao;
    this -> m_vbo = other.m_vbo;
    this -> m_ibo = other.m_ibo;
    this -> m_numIndices = other.m_numIndices;
    this -> m_numFaces = other.m_numFaces;
    this -> m_materialIndex = other.m_materialIndex;
    this -> m_vertices = other.m_vertices;
    this -> m_textures = other.m_textures;
    return *this;
}


Mesh::~Mesh()
{
    Release();
}

Mesh::Mesh(const std::vector<Vertex>& Vertices,
                     const std::vector<GLuint>& Indices,
                     const std::vector<CTexture*> &Textures,
                     const GLuint & materialIndex,
                     const GLuint & numFaces)
{
    m_vertices = Vertices;
    m_textures = Textures;
    m_numIndices = Indices.size();
    m_numFaces = numFaces;
    m_materialIndex = materialIndex;
    

    /* https://learnopengl.com/#!Advanced-OpenGL/Advanced-Data
     A buffer in OpenGL is only an object that manages a certain piece of memory and nothing more. We give a meaning to a buffer when binding it to a specific buffer target. A buffer is only a vertex array buffer when we bind it to GL_ARRAY_BUFFER, but we could just as easily bind it to GL_ELEMENT_ARRAY_BUFFER. OpenGL internally stores a buffer per target and based on the target, processes the buffers differently.

     So far we've been filling the memory managed by the buffer objects by calling glBufferData which allocates a piece of memory and adds data into this memory. If we were to pass NULL as its data argument, the function would only allocate memory and not fill it. This is useful if we first want to reserve a specific amount of memory and later come back to this buffer to fill it piece by piece.

     Instead of filling the entire buffer with one function call we can also fill specific regions of the buffer by calling glBufferSubData. This function expects a buffer target, an offset, the size of the data and the actual data as its arguments. What's new with this function is that we can now give an offset that specifies from where we want to fill the buffer. This allows us to insert/update only certain parts of the buffer's memory. Do note that the buffer should have enough allocated memory so a call to glBufferData is necessary before calling glBufferSubData on the buffer.

     */
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_numIndices, &Indices[0], GL_STATIC_DRAW);


    //vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    
    //texcoord
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)12);
    
    //normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)20);
    
    //tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)32);
    
    //bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44);
    
    glBindVertexArray(0);
}

void Mesh::Render(CShaderProgram *pShaderProgram, const GLboolean &useTexture) {
    
    if (useTexture) {
        
        for(unsigned int i = 0; i < m_textures.size(); i++)
        {
            //const TextureType textureType = m_textures[i]->GetType();
            GLint textureUnit = static_cast<GLint>(m_textures[i]->GetType());
            m_textures[i]->BindTexture2D(textureUnit);
        }
    }

    // draw mesh
    glBindVertexArray(m_vao);
    
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
    
}

// Release memory on the GPU
void Mesh::Release()
{
    
    for (GLuint i = 0 ; i < m_textures.size() ; i++) {
        m_textures[i]->Release();
        m_textures[i] = nullptr;
        SAFE_DELETE(m_textures[i]);
    }
    m_textures.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    
    if (m_vbo != INVALID_OGL_VALUE)
        glDeleteBuffers(1, &m_vbo);
    //m_vbo.Release();
    
    
    if (m_ibo != INVALID_OGL_VALUE)
        glDeleteBuffers(1, &m_ibo);
    //m_ebo.Release();
}
