#pragma once

#include "../MeshBase.h"

typedef struct {
	std::vector<unsigned int> id;	// list of triangle IDs 
} TriangleList;

class CFaceVertexMesh
{
public:
	CFaceVertexMesh();
	~CFaceVertexMesh();
	void Render();
	bool CreateFromTriangleList(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &triangles);
	void ComputeVertexNormals();
	glm::vec3 ComputeTriangleNormal(const unsigned int &tId);
	void ComputeTextureCoordsXZ(const float &xScale, const float &zScale);
    void Release();
private:
	std::vector<Vertex> m_vertices;			// A list of vertices
	std::vector<unsigned int> m_triangles;		// Stores vertex IDs -- every three makes a triangle
	std::vector<TriangleList> m_onTriangle;	// For each vertex, stores a list of triangle IDs saying which triangles the vertex is on
	GLuint m_uiVAO;
    GLuint m_uiVBOVertices;
    GLuint m_uiVBOIndices;
};
