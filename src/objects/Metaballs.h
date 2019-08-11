
#include "MarchingCubes.h"

#ifndef METABALLS_H
#define METABALLS_H

#define MAX_BALLS    32
#define MAX_VERTICES 3000
#define MAX_INDICES  3000

struct SBall
{
	float p[3]; // position
	float v[3]; // vertex
	float a[3];
	float t;
	float m;
};

#define FVF_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
struct SVertex
{
	float v[3];   // vertex
	float n[3];   // normal
	float t[2];   // texture
};

class CMetaballs: public IGameObject
{
public:
	CMetaballs();
    ~CMetaballs();
    
    void Create(const float &level, const int &numberOfBalls, const int &gridSize, const int &maxOpenVoxels, const std::string &directory,
                const std::map<std::string, TextureType> &textureFiles);
	void Compute();
	void Update(const GLfloat &fDeltaTime);

    void SetGridSize(const int &nSize);
    void ClearVectors();
    
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    void DrawElements(const GLboolean &useTexture);
    void Release();
    
protected:
	float ComputeEnergy(float x, float y, float z);
	void  ComputeNormal(SVertex *pVertex);
	float ComputeGridPointEnergy(int x, int y, int z);
	int   ComputeGridVoxel(int x, int y, int z, const bool &useTexture);

	bool  IsGridPointComputed(int x, int y, int z);
	bool  IsGridVoxelComputed(int x, int y, int z);
	bool  IsGridVoxelInList(int x, int y, int z);
	void  SetGridPointComputed(int x, int y, int z);
	void  SetGridVoxelComputed(int x, int y, int z);
	void  SetGridVoxelInList(int x, int y, int z);

	float ConvertGridPointToWorldCoordinate(int x);
	int   ConvertWorldCoordinateToGridPoint(float x);
	void  AddNeighborsToList(int nCase, int x, int y, int z);
	void  AddNeighbor(int x, int y, int z);

    float  m_fLevel;

	int    m_nNumOpenVoxels;
	int    m_nMaxOpenVoxels;
	int   *m_pOpenVoxels;

	int    m_nGridSize;
	float  m_fVoxelSize;

	float *m_pfGridEnergy;
	char  *m_pnGridPointStatus;
	char  *m_pnGridVoxelStatus;

	int    m_nNumVertices;
	int    m_nNumIndices;
    int    m_nNumBalls;
    SBall  m_Balls[MAX_BALLS];
    
    std::vector<SVertex>        m_pVertices;  // vertices data
    std::vector<unsigned short> m_pIndices;   // indices data
    
    std::vector<glm::vec3>      m_pVertexAttribute;  // vertex data
    std::vector<glm::vec2> m_pTextureAttribute;   // texture data
    std::vector<glm::vec3> m_pNormalAttribute;    // normal data
    
    GLuint m_vao;
    CVertexBufferObjectIndexed m_vbo;
    
    std::map<std::string, TextureType>m_textureFiles;
    std::vector<CTexture*> m_textures;
    
};

#endif
