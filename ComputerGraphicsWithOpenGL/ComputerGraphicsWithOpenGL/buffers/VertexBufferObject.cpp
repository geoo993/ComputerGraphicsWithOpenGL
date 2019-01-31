#include "VertexBufferObject.h"

/*   https://learnopengl.com/#!Advanced-OpenGL/Advanced-Data
 A buffer in OpenGL is only an object that manages a certain piece of memory and nothing more. We give a meaning to a buffer when binding it to a specific buffer target. A buffer is only a vertex array buffer when we bind it to GL_ARRAY_BUFFER, but we could just as easily bind it to GL_ELEMENT_ARRAY_BUFFER. OpenGL internally stores a buffer per target and based on the target, processes the buffers differently.
 
 */

// Constructor -- initialise member variable m_bDataUploaded to false
CVertexBufferObject::CVertexBufferObject()
{
	m_dataUploaded = false;
}

CVertexBufferObject::~CVertexBufferObject()
{
    Release();
}

// Create a VBO 
void CVertexBufferObject::Create()
{
	glGenBuffers(1, &m_vbo);
}

// Release the VBO and any associated data
void CVertexBufferObject::Release()
{
	glDeleteBuffers(1, &m_vbo);
	m_dataUploaded = false;
	m_data.clear();
}

void CVertexBufferObject::CopyTo(const GLenum &writetarget, const GLsizeiptr &size)
{
    /* Copying buffers  https://learnopengl.com/#!Advanced-OpenGL/Advanced-Data
     
     Once your buffers are filled with your data you could want to share that data with other buffers or perhaps copy the buffer's content into another buffer. The function glCopyBufferSubData allows us to copy the data from one buffer to another buffer with relative ease. The function's prototype is as follows:
     
     
     void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset,
     GLintptr writeoffset, GLsizeiptr size);
     The readtarget and writetarget parameters expect to give the buffer targets that we want to copy from and to. We could for example copy from a VERTEX_ARRAY_BUFFER buffer to a VERTEX_ELEMENT_ARRAY_BUFFER buffer by specifying those buffer targets as the read and write targets respectively. The buffers currently bound to those buffer targets will then be affected.
     
     But what if we wanted to read and write data into two different buffers that are both vertex array buffers? We can't bind two buffers at the same time to the same buffer target. For this reason, and this reason alone, OpenGL gives us two more buffer targets called GL_COPY_READ_BUFFER and GL_COPY_WRITE_BUFFER. We then bind the buffers of our choice to these new buffer targets and set those targets as the readtarget and writetarget argument.
     
     glCopyBufferSubData then reads data of a given size from a given readoffset and writes it into the writetarget buffer at writeoffset. An example of copying the content of two vertex array buffers is shown below:
     
     */
    glBindBuffer(GL_COPY_READ_BUFFER, m_vbo);
    glBindBuffer(GL_COPY_WRITE_BUFFER, writetarget);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
    
}

// Binds a VBO.  
void CVertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

// UnBinds a VBO.  
void CVertexBufferObject::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// Uploads the data to the GPU.  Afterwards, the data can be cleared.  
// iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...
void CVertexBufferObject::UploadDataToGPU(int usageHint)
{
    /* https://learnopengl.com/#!Advanced-OpenGL/Advanced-Data
     A buffer in OpenGL is only an object that manages a certain piece of memory and nothing more. We give a meaning to a buffer when binding it to a specific buffer target. A buffer is only a vertex array buffer when we bind it to GL_ARRAY_BUFFER, but we could just as easily bind it to GL_ELEMENT_ARRAY_BUFFER. OpenGL internally stores a buffer per target and based on the target, processes the buffers differently.
     
     So far we've been filling the memory managed by the buffer objects by calling glBufferData which allocates a piece of memory and adds data into this memory. If we were to pass NULL as its data argument, the function would only allocate memory and not fill it. This is useful if we first want to reserve a specific amount of memory and later come back to this buffer to fill it piece by piece.
     
     Instead of filling the entire buffer with one function call we can also fill specific regions of the buffer by calling glBufferSubData. This function expects a buffer target, an offset, the size of the data and the actual data as its arguments. What's new with this function is that we can now give an offset that specifies from where we want to fill the buffer. This allows us to insert/update only certain parts of the buffer's memory. Do note that the buffer should have enough allocated memory so a call to glBufferData is necessary before calling glBufferSubData on the buffer.
     
     */
    
	glBufferData(GL_ARRAY_BUFFER, m_data.size(), &m_data[0], usageHint);
	m_dataUploaded = true;
	m_data.clear();
}

// Adds data to the VBO.  
void CVertexBufferObject::AddData(void* ptrData, uint dataSize)
{
	m_data.insert(m_data.end(), (BYTE*)ptrData, (BYTE*)ptrData+dataSize);
}



