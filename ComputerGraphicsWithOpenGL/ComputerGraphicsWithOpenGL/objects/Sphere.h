#pragma once

#include "Texture.h"
#include "VertexBufferObjectIndexed.h"
#include "GameObject.h"

// Class for generating a unit sphere
class CSphere: public GameObject
{
public:
	CSphere();
	~CSphere();
    void Create(std::string directory, std::string front, int slicesIn, int stacksIn);
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const bool &useTexture = true);
    void Release();
    
private:
	GLuint m_vao;
	CVertexBufferObjectIndexed m_vbo;
	CTexture m_texture;
	std::string m_directory;
	std::string m_filename;
    
    // inputs
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
	int m_numTriangles;
};
