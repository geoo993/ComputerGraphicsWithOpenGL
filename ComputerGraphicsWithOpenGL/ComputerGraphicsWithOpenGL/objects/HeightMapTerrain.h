#pragma once

#include "FaceVertexMesh.h"
#include "GameObject.h"
#include "Texture.h"

class CHeightMapTerrain: public GameObject
{
public:
	CHeightMapTerrain();
	~CHeightMapTerrain();
	bool Create(const char *terrainFilename, const std::vector<std::string> &textureFilenames, glm::vec3 origin, float terrainSizeX, float terrainSizeZ, float terrainHeightScale);
	float ReturnGroundHeight(glm::vec3 p);

    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const bool &useTexture = true);
    void Release();
    bool IsHeightMapRendered();
    
private:
	int m_width, m_height;
    bool m_isRendered;
	GLfloat *m_heightMap;
	CFaceVertexMesh m_mesh;
	GLuint m_hTexture;
	GLfloat m_terrainSizeX, m_terrainSizeZ;
	glm::vec3 m_origin;
    std::vector<std::string> m_textureFileNames;
    std::vector<CTexture> m_textures;
    
	FIBITMAP* m_dib;

	glm::vec3 WorldToImageCoordinates(glm::vec3 p);
	glm::vec3 ImageToWorldCoordinates(glm::vec3 p);
	bool GetImageBytes(char *terrainFilename, BYTE **bDataPointer, unsigned int &width, unsigned int &height);
};
