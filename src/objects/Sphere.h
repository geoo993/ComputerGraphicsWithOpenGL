#pragma once

#include "../ObjectsBase.h"

// Class for generating a unit sphere
class CSphere: public IGameObject
{
public:
	CSphere();
	~CSphere();
    void Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames, int slicesIn, int stacksIn);
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    void Release();
    
private:
    
	GLuint m_vao;
	CVertexBufferObjectIndexed m_vbo;
    
    std::map<std::string, TextureType> m_textureNames;
    std::vector<CTexture*> m_textures;
    
    // inputs
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
	GLint m_numTriangles;
};
