
#include "MeshEntry.h"

MeshEntry::MeshEntry()
{
    vbo = INVALID_OGL_VALUE;
    mbo = INVALID_OGL_VALUE;
    ibo = INVALID_OGL_VALUE;
    m_numIndices  = 0;
    m_vertices.clear();
    m_materialIndex = INVALID_MATERIAL;
};

MeshEntry::~MeshEntry()
{
    if (vbo != INVALID_OGL_VALUE)
        glDeleteBuffers(1, &vbo);

    if (mbo != INVALID_OGL_VALUE)
        glDeleteBuffers(1, &mbo);

    if (ibo != INVALID_OGL_VALUE)
        glDeleteBuffers(1, &ibo);

}

void MeshEntry::Init(const std::vector<Vertex>& Vertices,
                     const std::vector<GLuint>& Indices,
                     const std::vector<glm::mat4> &mMatrices)
{
    m_numIndices = Indices.size();
    m_vertices = Vertices;

    /* https://learnopengl.com/#!Advanced-OpenGL/Advanced-Data
     A buffer in OpenGL is only an object that manages a certain piece of memory and nothing more. We give a meaning to a buffer when binding it to a specific buffer target. A buffer is only a vertex array buffer when we bind it to GL_ARRAY_BUFFER, but we could just as easily bind it to GL_ELEMENT_ARRAY_BUFFER. OpenGL internally stores a buffer per target and based on the target, processes the buffers differently.

     So far we've been filling the memory managed by the buffer objects by calling glBufferData which allocates a piece of memory and adds data into this memory. If we were to pass NULL as its data argument, the function would only allocate memory and not fill it. This is useful if we first want to reserve a specific amount of memory and later come back to this buffer to fill it piece by piece.

     Instead of filling the entire buffer with one function call we can also fill specific regions of the buffer by calling glBufferSubData. This function expects a buffer target, an offset, the size of the data and the actual data as its arguments. What's new with this function is that we can now give an offset that specifies from where we want to fill the buffer. This allows us to insert/update only certain parts of the buffer's memory. Do note that the buffer should have enough allocated memory so a call to glBufferData is necessary before calling glBufferSubData on the buffer.

     */

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    // vertex Buffer Object
    glGenBuffers(1, &mbo);
    glBindBuffer(GL_ARRAY_BUFFER, mbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mMatrices.size(), &mMatrices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numIndices, &Indices[0], GL_STATIC_DRAW);

}
