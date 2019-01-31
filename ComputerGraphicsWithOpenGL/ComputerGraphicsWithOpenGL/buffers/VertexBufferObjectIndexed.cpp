#include "VertexBufferObjectIndexed.h"

/*   https://learnopengl.com/#!Advanced-OpenGL/Advanced-Data
 A buffer in OpenGL is only an object that manages a certain piece of memory and nothing more. We give a meaning to a buffer when binding it to a specific buffer target. A buffer is only a vertex array buffer when we bind it to GL_ARRAY_BUFFER, but we could just as easily bind it to GL_ELEMENT_ARRAY_BUFFER. OpenGL internally stores a buffer per target and based on the target, processes the buffers differently.
 
 */

// Constructor -- initialise member variable m_bDataUploaded to false
CVertexBufferObjectIndexed::CVertexBufferObjectIndexed()
{
	m_dataUploaded = false;
}

CVertexBufferObjectIndexed::~CVertexBufferObjectIndexed()
{
    Release();
}


// Create buffer objects for the vertices and indices
void CVertexBufferObjectIndexed::Create()
{
    /*-----------------------------------------------------------------------------
     *  CREATE THE VERTEX BUFFER OBJECT
     *-----------------------------------------------------------------------------*/
	glGenBuffers(1, &m_vboVertices);
    
    /*-----------------------------------------------------------------------------
     *  CREATE THE ELEMENT/INDEXES ARRAY BUFFER OBJECT
     *-----------------------------------------------------------------------------*/
	glGenBuffers(1, &m_vboIndices);
}

// Binds the buffers
void CVertexBufferObjectIndexed::Bind()
{
    /*-----------------------------------------------------------------------------
     *  BIND THE VERTEX BUFFER OBJECT
     *-----------------------------------------------------------------------------*/
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
    
    /*-----------------------------------------------------------------------------
     *  BIND THE ELEMENT/INDEXES ARRAY BUFFER OBJECT
     *-----------------------------------------------------------------------------*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndices);
}

// Binds the buffers
void CVertexBufferObjectIndexed::UnBind()
{
    /*-----------------------------------------------------------------------------
     *  UNBIND THE VERTEX BUFFER OBJECT
     *-----------------------------------------------------------------------------*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    /*-----------------------------------------------------------------------------
     *  UNBIND THE ELEMENT/INDEXES ARRAY BUFFER OBJECT
     *-----------------------------------------------------------------------------*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Release the buffers and any associated data
void CVertexBufferObjectIndexed::Release()
{
    
    glDeleteBuffers(1, &m_vboVertices);
    glDeleteBuffers(1, &m_vboIndices);
    m_dataUploaded = false;
    m_vertexData.clear();
    m_indexData.clear();
}

void CVertexBufferObjectIndexed::CopyTo(const GLenum &readtarget, const GLenum &writetarget, const GLsizeiptr &size)
{
    /* Copying buffers  https://learnopengl.com/#!Advanced-OpenGL/Advanced-Data
     
     Once your buffers are filled with your data you could want to share that data with other buffers or perhaps copy the buffer's content into another buffer. The function glCopyBufferSubData allows us to copy the data from one buffer to another buffer with relative ease. The function's prototype is as follows:
     
     
     void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset,
     GLintptr writeoffset, GLsizeiptr size);
     The readtarget and writetarget parameters expect to give the buffer targets that we want to copy from and to. We could for example copy from a VERTEX_ARRAY_BUFFER buffer to a VERTEX_ELEMENT_ARRAY_BUFFER buffer by specifying those buffer targets as the read and write targets respectively. The buffers currently bound to those buffer targets will then be affected.
     
     But what if we wanted to read and write data into two different buffers that are both vertex array buffers? We can't bind two buffers at the same time to the same buffer target. For this reason, and this reason alone, OpenGL gives us two more buffer targets called GL_COPY_READ_BUFFER and GL_COPY_WRITE_BUFFER. We then bind the buffers of our choice to these new buffer targets and set those targets as the readtarget and writetarget argument.
     
     glCopyBufferSubData then reads data of a given size from a given readoffset and writes it into the writetarget buffer at writeoffset. An example of copying the content of two vertex array buffers is shown below:
     
     */
    glBindBuffer(GL_COPY_READ_BUFFER, readtarget);
    glBindBuffer(GL_COPY_WRITE_BUFFER, writetarget);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
    
}


// Uploads the data to the GPU.  Afterwards, the data can be cleared.  
// iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...
void CVertexBufferObjectIndexed::UploadDataToGPU(int iUsageHint)
{

    /* https://learnopengl.com/#!Advanced-OpenGL/Advanced-Data
     A buffer in OpenGL is only an object that manages a certain piece of memory and nothing more. We give a meaning to a buffer when binding it to a specific buffer target. A buffer is only a vertex array buffer when we bind it to GL_ARRAY_BUFFER, but we could just as easily bind it to GL_ELEMENT_ARRAY_BUFFER. OpenGL internally stores a buffer per target and based on the target, processes the buffers differently.
     
     So far we've been filling the memory managed by the buffer objects by calling glBufferData which allocates a piece of memory and adds data into this memory. If we were to pass NULL as its data argument, the function would only allocate memory and not fill it. This is useful if we first want to reserve a specific amount of memory and later come back to this buffer to fill it piece by piece.
     
     Instead of filling the entire buffer with one function call we can also fill specific regions of the buffer by calling glBufferSubData. This function expects a buffer target, an offset, the size of the data and the actual data as its arguments. What's new with this function is that we can now give an offset that specifies from where we want to fill the buffer. This allows us to insert/update only certain parts of the buffer's memory. Do note that the buffer should have enough allocated memory so a call to glBufferData is necessary before calling glBufferSubData on the buffer.
     
     */
    
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size(), &m_vertexData[0], iUsageHint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size(), &m_indexData[0], iUsageHint);
	m_dataUploaded = true;
	m_vertexData.clear();
	m_indexData.clear();
}

// Adds data to the VBO.  
void CVertexBufferObjectIndexed::AddVertexData(void* ptrVertexData, uint uiVertexDataSize)
{
	m_vertexData.insert(m_vertexData.end(), (BYTE*)ptrVertexData, (BYTE*)ptrVertexData+uiVertexDataSize);
}


// Adds data to the VBO.  
void CVertexBufferObjectIndexed::AddIndexData(void* ptrIndexData, uint uiIndexDataSize)
{
	m_indexData.insert(m_indexData.end(), (BYTE*)ptrIndexData, (BYTE*)ptrIndexData+uiIndexDataSize);
}

