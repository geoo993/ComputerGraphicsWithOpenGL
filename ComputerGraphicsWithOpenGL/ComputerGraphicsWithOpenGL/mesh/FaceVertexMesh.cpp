#include "FaceVertexMesh.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

CFaceVertexMesh::CFaceVertexMesh()
{}

CFaceVertexMesh::~CFaceVertexMesh()
{
    Release();
}

// Compute the normal of a triangle using the cross product
glm::vec3 CFaceVertexMesh::ComputeTriangleNormal(const unsigned int &tId)
{
	CVertex v0, v1, v2;
	glm::vec3 normal, p, q;
	
	v0 = m_vertices[m_triangles[3*tId]];
	v1 = m_vertices[m_triangles[3*tId+1]];
	v2 = m_vertices[m_triangles[3*tId+2]];

	p = v1.position - v0.position;
	q = v2.position - v0.position;
	normal = glm::normalize(glm::cross(p, q)); 

	return normal;
}


void CFaceVertexMesh::ComputeTextureCoordsXZ(const float &xScale, const float &zScale)
{
	// Set texture coords based on the x and z coordinates
	for (unsigned int i = 0; i < m_vertices.size(); i++) {
		m_vertices[i].textureCoord.s = m_vertices[i].position.x / xScale;
		m_vertices[i].textureCoord.t = m_vertices[i].position.z / zScale;
	}
}
void CFaceVertexMesh::ComputeVertexNormals()
{
	for (unsigned int i = 0; i < m_vertices.size(); i++) {
		// Get normal for each triangle around v in one ring neighbourhood
		glm::vec3 normal = glm::vec3(0, 0, 0);
		int faceCount = 0;
		for (unsigned int j = 0; j < m_onTriangle[i].id.size(); j++) {
			int tId = m_onTriangle[i].id[j];			
			normal += ComputeTriangleNormal(tId);
			faceCount++;
		}
		normal = glm::normalize(normal / (float) faceCount);
		m_vertices[i].normal = normal;
	}
}

bool CFaceVertexMesh::CreateFromTriangleList(const std::vector<CVertex> &vertices, const std::vector<unsigned int> &triangles)
{
	// Set the vertices and indices
	m_vertices = vertices;
	m_triangles = triangles;
	
	// Now we must fill the onTriangle list
	m_onTriangle.resize(m_vertices.size());
	unsigned int numTriangles = (unsigned int) (triangles.size() / 3);
	for (unsigned int t = 0; t < numTriangles; t++) {
		m_onTriangle[m_triangles[t*3]].id.push_back(t);
		m_onTriangle[m_triangles[t*3+1]].id.push_back(t);
		m_onTriangle[m_triangles[t*3+2]].id.push_back(t);
	}

	// Compute vertex normals and texture coords
	ComputeVertexNormals();
	ComputeTextureCoordsXZ(20.0f, 20.0f);
	
	// Create a VAO 
	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);

	// Create a VBO for the vertex data
	glGenBuffers(1, &m_uiVBOVertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVBOVertices);

	// Fill the vertices VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(CVertex), &m_vertices[0], GL_STATIC_DRAW);

	// Generate a VGO for the indices and bind it
	glGenBuffers(1, &m_uiVBOIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiVBOIndices);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangles.size() * sizeof(GLuint), &m_triangles[0], GL_STATIC_DRAW);

	GLsizei stride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));


	return true;
}

void CFaceVertexMesh::Render()
{
	glBindVertexArray(m_uiVAO);

	// Draw
	glDrawElements(GL_TRIANGLES, m_triangles.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

}

void CFaceVertexMesh::Release() {
    glDeleteVertexArrays(1, &m_uiVAO);
    glDeleteBuffers(1, &m_uiVBOVertices);
    glDeleteBuffers(1, &m_uiVBOIndices);
}
