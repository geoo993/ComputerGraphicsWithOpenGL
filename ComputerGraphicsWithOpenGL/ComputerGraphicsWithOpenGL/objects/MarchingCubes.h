#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

class CMarchingCubes
{
public:
	static void  BuildTables();

	static char  m_CubeEdges[12][2];
	static char  m_CubeTriangles[256][16];
	static char  m_CubeNeighbors[256];
	static float m_CubeVertices[8][3];
};

#endif