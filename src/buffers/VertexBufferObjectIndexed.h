#pragma once

#include "../BuffersBase.h"

class CVertexBufferObjectIndexed
{
public:
	CVertexBufferObjectIndexed();
	~CVertexBufferObjectIndexed();

	void Create();									// Creates a VBO
	void Bind();									// Binds the VBO
    void UnBind();                                  // UnBinds the VBO
	void Release();									// Releases the VBO
    void CopyTo(const GLenum &readtarget, const GLenum &writetarget, const GLsizeiptr &size);
	void AddVertexData(void* pVertexData, uint vertexDataSize);	// Adds vertex data
	void AddIndexData(void* pIndexData, uint indexDataSize);	// Adds index data
	void UploadDataToGPU(int iUsageHint);			// Upload the VBO to the GPU
    

private:
	GLuint m_vboVertices;		// VBO id for vertices
	GLuint m_vboIndices;		// VBO id for indices

	std::vector<BYTE> m_vertexData;	// Vertex data to be uploaded
	std::vector<BYTE> m_indexData;	// Index data to be uploaded

	bool m_dataUploaded;		// Flag indicating if data is uploaded to the GPU
};
