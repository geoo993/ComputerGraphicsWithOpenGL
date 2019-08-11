
#include "Grid.h"

CGrid::CGrid()
{
	m_iVertices = 0;
}

CGrid::~CGrid()
{
    Release();
}


// Create a planar grid
void CGrid::Create(float fWidth, float fHeight, int iLines)
{
	
	if (iLines < 2)
		return;

	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create a VBO
	m_vbo.Create();
	m_vbo.Bind();

	glm::vec2 textureCoord = glm::vec2(0, 0);
	glm::vec3 normal = glm::vec3(0, 1, 0);

	
	// Add lines along x
	for (int i = 0; i < iLines; i++) {
		float t = ((float) i / (float) (iLines-1));
		glm::vec3 p1 = glm::vec3(-fWidth / 2, 0, -fHeight / 2 + t * fHeight);
		m_vbo.AddData(&p1, sizeof(glm::vec3));
		m_vbo.AddData(&textureCoord, sizeof(glm::vec2));
		m_vbo.AddData(&normal, sizeof(glm::vec3));

		glm::vec3 p2 = glm::vec3(fWidth / 2, 0, -fHeight / 2 + t * fHeight);
		m_vbo.AddData(&p2, sizeof(glm::vec3));
		m_vbo.AddData(&textureCoord, sizeof(glm::vec2));
		m_vbo.AddData(&normal, sizeof(glm::vec3));
		
		m_iVertices += 2;
	}
	
	
	
	// Add lines along z
	for (int i = 0; i < iLines; i++) {
		float t = ((float) i / (float) (iLines-1));
		glm::vec3 p1 = glm::vec3(-fWidth / 2 + t * fWidth, 0, -fHeight / 2);
		m_vbo.AddData(&p1, sizeof(glm::vec3));
		m_vbo.AddData(&textureCoord, sizeof(glm::vec2));
		m_vbo.AddData(&normal, sizeof(glm::vec3));

		glm::vec3 p2 = glm::vec3(-fWidth / 2 + t * fWidth, 0, fHeight / 2);
		m_vbo.AddData(&p2, sizeof(glm::vec3));
		m_vbo.AddData(&textureCoord, sizeof(glm::vec2));
		m_vbo.AddData(&normal, sizeof(glm::vec3));

		m_iVertices += 2;

	}
	


	// Upload the VBO to the GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei istride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
	
}


void CGrid::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    // Render the planar terrain
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

// Render the grid with lines
void CGrid::Render(const GLboolean &useTexture)
{
	if (m_iVertices < 2)
		return;

	glLineWidth(1.0);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINES, 0, m_iVertices);
}

// Release resources
void CGrid::Release()
{
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}
