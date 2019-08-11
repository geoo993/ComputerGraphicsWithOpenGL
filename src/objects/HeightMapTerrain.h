#pragma once

#include "../ObjectsBase.h"

class CHeightMapTerrain: public IGameObject
{
public:
	CHeightMapTerrain();
	~CHeightMapTerrain();
	GLboolean Create(const char *terrainFilename, const std::map<std::string, TextureType> &textureFilenames, glm::vec3 origin,
                     GLfloat terrainSizeX, GLfloat terrainSizeZ, GLfloat terrainHeightScale);
	GLfloat ReturnGroundHeight(glm::vec3 p);

    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    void Release();
    GLboolean IsHeightMapRendered();
    
private:
	GLint m_width, m_height;
    GLboolean m_isRendered;
	GLfloat *m_heightMap;
	CFaceVertexMesh m_mesh;
	GLuint m_hTexture;
	GLfloat m_terrainSizeX, m_terrainSizeZ;
	glm::vec3 m_origin;
    std::map<std::string, TextureType> m_textureFileNames;
    std::vector<CTexture*> m_textures;
    
	FIBITMAP* m_dib;

	glm::vec3 WorldToImageCoordinates(glm::vec3 p);
	glm::vec3 ImageToWorldCoordinates(glm::vec3 p);
	GLboolean GetImageBytes(char *terrainFilename, BYTE **bDataPointer, GLuint &width, GLuint &height);
};
