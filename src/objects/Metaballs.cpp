
#include "Metaballs.h"

// http://www.humus.name/index.php?page=3D&ID=57
// http://www.angelcode.com/dev/metaballs/metaballs.html
// http://www.angelcode.com/dev/metaballs/

//=============================================================================
CMetaballs::CMetaballs()
{
    m_vao = 0;
    m_textures = {};
}

CMetaballs::~CMetaballs(){
    
    Release();
}

void CMetaballs::Create(const float &level, const int &numberOfBalls, const int &gridSize, const int &maxOpenVoxels, const std::string &directory,
                        const std::map<std::string, TextureType> &textureFiles){
    m_fLevel    = level;//100.0f;
    m_nNumBalls = numberOfBalls; //20;

    m_nGridSize = gridSize;//0;

	m_nMaxOpenVoxels = maxOpenVoxels;
	m_pOpenVoxels    = new int[m_nMaxOpenVoxels*3];
   
	m_nNumOpenVoxels    = 0;
	m_pfGridEnergy      = 0;
	m_pnGridPointStatus = 0;
	m_pnGridVoxelStatus = 0;

	m_nNumVertices = 0;
	m_nNumIndices  = 0;
	m_pVertices.reserve(MAX_VERTICES);
	m_pIndices.reserve(MAX_INDICES);

    m_textureFiles = textureFiles;
    m_textures.reserve(textureFiles.size());
    
    // Iterate through all elements in std::map
    for (auto it = textureFiles.begin(); it != textureFiles.end(); ++it) {
        // if the current index is needed:
        auto i = std::distance(textureFiles.begin(), it);
        
        // access element as *it
        m_textures.push_back(new CTexture);
        m_textures[i]->LoadTexture(directory+it->first, it->second, true);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // any code including continue, break, return
    }

	for( int i = 0; i < m_nNumBalls; i++ )
	{
		m_Balls[i].p[0] = 0;
		m_Balls[i].p[1] = 0;
		m_Balls[i].p[2] = 0;
		m_Balls[i].v[0] = 0;
		m_Balls[i].v[1] = 0;
		m_Balls[i].v[2] = 0;
		m_Balls[i].a[0] = (float(Extensions::randFloat())/RAND_MAX*2-1) / 2;
		m_Balls[i].a[1] = (float(Extensions::randFloat())/RAND_MAX*2-1) / 2;
		m_Balls[i].a[2] = (float(Extensions::randFloat())/RAND_MAX*2-1) / 2;
        m_Balls[i].t = float(Extensions::randFloat())/RAND_MAX;
		m_Balls[i].m = 1;
	}
    
}

//=============================================================================
void CMetaballs::Update(const GLfloat &fDeltaTime)
{
	for( int i = 0; i < m_nNumBalls; i++ )
	{
		m_Balls[i].p[0] += fDeltaTime*m_Balls[i].v[0];
		m_Balls[i].p[1] += fDeltaTime*m_Balls[i].v[1];
		m_Balls[i].p[2] += fDeltaTime*m_Balls[i].v[2];

		m_Balls[i].t -= fDeltaTime;
		if( m_Balls[i].t < 0 )
		{
			// When is the next time to act?
			m_Balls[i].t = float(Extensions::randFloat())/RAND_MAX;

			// Use a new attraction point
			m_Balls[i].a[0] = (float(Extensions::randFloat())/RAND_MAX*2-1)/2;
			m_Balls[i].a[1] = (float(Extensions::randFloat())/RAND_MAX*2-1)/2;
			m_Balls[i].a[2] = (float(Extensions::randFloat())/RAND_MAX*2-1)/2;
		}

		// Accelerate towards the attraction point
		float x = m_Balls[i].a[0] - m_Balls[i].p[0];
		float y = m_Balls[i].a[1] - m_Balls[i].p[1];
		float z = m_Balls[i].a[2] - m_Balls[i].p[2];
		float fDist = 1/sqrtf(x*x + y*y + z*z);

		x *= fDist;
		y *= fDist;
		z *= fDist;
		
		m_Balls[i].v[0] += 0.1f*x*fDeltaTime;
		m_Balls[i].v[1] += 0.1f*y*fDeltaTime;
		m_Balls[i].v[2] += 0.1f*z*fDeltaTime;

		fDist = m_Balls[i].v[0]*m_Balls[i].v[0] + 
		        m_Balls[i].v[1]*m_Balls[i].v[1] + 
		        m_Balls[i].v[2]*m_Balls[i].v[2];
		if( fDist > 0.050f ) // how further away can they go
		{
			fDist = 1/sqrtf(fDist);
			m_Balls[i].v[0] = 0.20f*m_Balls[i].v[0]*fDist;
			m_Balls[i].v[1] = 0.20f*m_Balls[i].v[1]*fDist;
			m_Balls[i].v[2] = 0.20f*m_Balls[i].v[2]*fDist;
		}

		if( m_Balls[i].p[0] < -1+m_fVoxelSize ) 
		{
			m_Balls[i].p[0] = -1+m_fVoxelSize;
			m_Balls[i].v[0] = 0;
		}
		if( m_Balls[i].p[0] >  1-m_fVoxelSize ) 
		{
			m_Balls[i].p[0] =  1-m_fVoxelSize;
			m_Balls[i].v[0] = 0;
		}
		if( m_Balls[i].p[1] < -1+m_fVoxelSize ) 
		{
			m_Balls[i].p[1] = -1+m_fVoxelSize;
			m_Balls[i].v[1] = 0;
		}
		if( m_Balls[i].p[1] >  1-m_fVoxelSize ) 
		{
			m_Balls[i].p[1] =  1-m_fVoxelSize;
			m_Balls[i].v[1] = 0;
		}
		if( m_Balls[i].p[2] < -1+m_fVoxelSize ) 
		{
			m_Balls[i].p[2] = -1+m_fVoxelSize;
			m_Balls[i].v[2] = 0;
		}
		if( m_Balls[i].p[2] >  1-m_fVoxelSize ) 
		{
			m_Balls[i].p[2] =  1-m_fVoxelSize;
			m_Balls[i].v[2] = 0;
		}
	}
}

//=============================================================================
void CMetaballs::Render(const GLboolean &useTexture)
{
    
	int nCase,x,y,z = 0;
	bool bComputed;

    /*  
	// Lock the vertex and index buffer
	//CGraphics *pGfx = CGraphics::GetInstance();   /// graphics.h

	//pGfx->LockVertexBuffer(MAX_VERTICES, (BYTE**)&m_pVertices); /// graphics.h
	//pGfx->LockIndexBuffer(MAX_INDICES, (BYTE**)&m_pIndices);    /// graphics.h
    
    */
    
    
    // we go in
    // we count all the data until we stop
    // when we stop we render
    // then clear every vector
    // then count again until we stop again
    // then render
    // then clear every vector again
    // the render all the remaining triangles when the loop ends
    
    
    ClearVectors();
    
    
	// Clear status grids
	memset(m_pnGridPointStatus, 0, (m_nGridSize+1)*(m_nGridSize+1)*(m_nGridSize+1));
	memset(m_pnGridVoxelStatus, 0, m_nGridSize*m_nGridSize*m_nGridSize);

    // we render each ball
	for( int i = 0; i < m_nNumBalls; i++ )
	{
		x = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[0]);   /// first method
		y = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[1]);
		z = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[2]);

		// Work our way out from the center of the ball until the surface is
		// reached. If the voxel at the surface is already computed then this
		// ball share surface with a previous ball.
		bComputed = false;
		while(1) //// while loop runs many times until 1
		{
			if( IsGridVoxelComputed(x,y,z) )  
			{
				bComputed = true;
				break;
			}

			nCase = ComputeGridVoxel(x,y,z, useTexture);
			if( nCase < 255 )
				break;

			z--;
		}

		if( bComputed )
			continue;

		// Compute all voxels on the surface by computing neighbouring voxels
		// if the surface goes into them.
		AddNeighborsToList(nCase,x,y,z);  //// forth is fine

		while( m_nNumOpenVoxels )
		{
			m_nNumOpenVoxels--;
			x = m_pOpenVoxels[m_nNumOpenVoxels*3];
			y = m_pOpenVoxels[m_nNumOpenVoxels*3 + 1];
			z = m_pOpenVoxels[m_nNumOpenVoxels*3 + 2];

			nCase = ComputeGridVoxel(x,y,z, useTexture);  ////  grid voxel

			AddNeighborsToList(nCase,x,y,z);   ////  neighbors
		}
        
	}

    /* 
	// Unlock buffers
	//pGfx->UnlockVertexBuffer();     /// graphics.h
	//pGfx->UnlockIndexBuffer();      /// graphics.h

	// Render the last triangles
	//IDirect3DDevice8 *pDev = pGfx->GetD3DDevice();    
    pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumVertices, 0, m_nNumIndices/3); ////<d3dx8.h>
     */
    
    // Render the last triangles
    DrawElements(useTexture);
}

//=============================================================================
void CMetaballs::AddNeighborsToList(int nCase, int x, int y, int z)
{
	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<0) )
		AddNeighbor(x+1, y, z);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<1) )
		AddNeighbor(x-1, y, z);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<2) )
		AddNeighbor(x, y+1, z);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<3) )
		AddNeighbor(x, y-1, z);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<4) )
		AddNeighbor(x, y, z+1);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<5) )
		AddNeighbor(x, y, z-1);
}

//=============================================================================
void CMetaballs::AddNeighbor(int x, int y, int z)
{
	if( IsGridVoxelComputed(x,y,z) || IsGridVoxelInList(x,y,z) )
		return;

	// Make sure the array is large enough
	if( m_nMaxOpenVoxels == m_nNumOpenVoxels )
	{
		m_nMaxOpenVoxels *= 2;
		int *pTmp = new int[m_nMaxOpenVoxels*3];
		memcpy(pTmp, m_pOpenVoxels, m_nNumOpenVoxels*3*sizeof(int));
		delete[] m_pOpenVoxels;
		m_pOpenVoxels = pTmp;
	}

	m_pOpenVoxels[m_nNumOpenVoxels*3] = x;
	m_pOpenVoxels[m_nNumOpenVoxels*3+1] = y;
	m_pOpenVoxels[m_nNumOpenVoxels*3+2] = z;

	SetGridVoxelInList(x,y,z);

	m_nNumOpenVoxels++;
}

//=============================================================================
float CMetaballs::ComputeEnergy(float x, float y, float z)
{
	float fEnergy = 0;
	float fSqDist;

	for( int i = 0; i < m_nNumBalls; i++ )
	{
		// The formula for the energy is 
		// 
		//   e += mass/distance^2 

		fSqDist = (m_Balls[i].p[0] - x)*(m_Balls[i].p[0] - x) +
		          (m_Balls[i].p[1] - y)*(m_Balls[i].p[1] - y) +
		          (m_Balls[i].p[2] - z)*(m_Balls[i].p[2] - z);

		if( fSqDist < 0.0001f ) fSqDist = 0.0001f;

		fEnergy += m_Balls[i].m / fSqDist;
	}

	return fEnergy;
}

//=============================================================================
void CMetaballs::ComputeNormal(SVertex *pVertex)
{
	float fSqDist;

	for( int i = 0; i < m_nNumBalls; i ++ )
	{
		// To compute the normal we derive the energy formula and get
		//
		//   n += 2 * mass * vector / distance^4

		float x = pVertex->v[0] - m_Balls[i].p[0];
		float y = pVertex->v[1] - m_Balls[i].p[1];
		float z = pVertex->v[2] - m_Balls[i].p[2];

		fSqDist = x*x + y*y + z*z;

		pVertex->n[0] += 2 * m_Balls[i].m * x / (fSqDist * fSqDist);
		pVertex->n[1] += 2 * m_Balls[i].m * y / (fSqDist * fSqDist);
		pVertex->n[2] += 2 * m_Balls[i].m * z / (fSqDist * fSqDist);
	}

    /*
	////D3DXVec3Normalize((D3DXVECTOR3*)pVertex->n, (D3DXVECTOR3*)pVertex->n); 
     */
    
    
	// Compute the sphere-map texture coordinate
	// Note: The normal used here should be transformed to camera space first
	// for correct result. In this application no transformation is needed 
	// since the camera is fixed. 
	pVertex->t[0] = pVertex->n[0]/2 + 0.5f;
	pVertex->t[1] = -pVertex->n[1]/2 + 0.5f;
    
    
    // Add the normals and texture data in their vector 
    glm::vec2 t = glm::vec2(pVertex->t[0],pVertex->t[1] );
    m_pTextureAttribute.push_back(t);   // texture data
    
    glm::vec3 n = glm::vec3(pVertex->t[0], pVertex->t[1], pVertex->t[2]);
    m_pNormalAttribute.push_back(n);    // normal data
}

//=============================================================================
float CMetaballs::ComputeGridPointEnergy(int x, int y, int z)
{
	if( IsGridPointComputed(x,y,z) )
		return m_pfGridEnergy[x +
		                      y*(m_nGridSize+1) +
		                      z*(m_nGridSize+1)*(m_nGridSize+1)];

	// The energy on the edges are always zero to make sure the isosurface is
	// always closed.
	if( x == 0 || y == 0 || z == 0 ||
	    x == m_nGridSize || y == m_nGridSize || z == m_nGridSize )
	{
		m_pfGridEnergy[x +
		               y*(m_nGridSize+1) +
		               z*(m_nGridSize+1)*(m_nGridSize+1)] = 0;
		SetGridPointComputed(x,y,z);
		return 0;
	}

	float fx = ConvertGridPointToWorldCoordinate(x);
	float fy = ConvertGridPointToWorldCoordinate(y);
	float fz = ConvertGridPointToWorldCoordinate(z);

	m_pfGridEnergy[x +
	               y*(m_nGridSize+1) +
	               z*(m_nGridSize+1)*(m_nGridSize+1)] = ComputeEnergy(fx,fy,fz);

	SetGridPointComputed(x,y,z);

	return m_pfGridEnergy[x +
  	                      y*(m_nGridSize+1) +
	                      z*(m_nGridSize+1)*(m_nGridSize+1)];
}

//=============================================================================
int CMetaballs::ComputeGridVoxel(int x, int y, int z, const bool &useTexture)
{
	float b[8];

	b[0] = ComputeGridPointEnergy(x  , y  , z  );
	b[1] = ComputeGridPointEnergy(x+1, y  , z  );
	b[2] = ComputeGridPointEnergy(x+1, y  , z+1);
	b[3] = ComputeGridPointEnergy(x  , y  , z+1);
	b[4] = ComputeGridPointEnergy(x  , y+1, z  );
	b[5] = ComputeGridPointEnergy(x+1, y+1, z  );
	b[6] = ComputeGridPointEnergy(x+1, y+1, z+1);
	b[7] = ComputeGridPointEnergy(x  , y+1, z+1);

	float fx = ConvertGridPointToWorldCoordinate(x) + m_fVoxelSize/2;
	float fy = ConvertGridPointToWorldCoordinate(y) + m_fVoxelSize/2;
	float fz = ConvertGridPointToWorldCoordinate(z) + m_fVoxelSize/2;

	int c = 0;
	c |= b[0] > m_fLevel ? (1<<0) : 0;
	c |= b[1] > m_fLevel ? (1<<1) : 0;
	c |= b[2] > m_fLevel ? (1<<2) : 0;
	c |= b[3] > m_fLevel ? (1<<3) : 0;
	c |= b[4] > m_fLevel ? (1<<4) : 0;
	c |= b[5] > m_fLevel ? (1<<5) : 0;
	c |= b[6] > m_fLevel ? (1<<6) : 0;
	c |= b[7] > m_fLevel ? (1<<7) : 0;

	// Compute vertices from marching pyramid case
	fx = ConvertGridPointToWorldCoordinate(x);
	fy = ConvertGridPointToWorldCoordinate(y);
	fz = ConvertGridPointToWorldCoordinate(z);

	int i = 0;
	unsigned short EdgeIndices[12];
	memset(EdgeIndices, 0xFF, 12*sizeof(unsigned short));
	while(1)  // while loop runs many times until 1
	{
		int nEdge =	CMarchingCubes::m_CubeTriangles[c][i];
		if( nEdge == -1 )
			break;

		if( EdgeIndices[nEdge] == 0xFFFF )
		{
			EdgeIndices[nEdge] = m_nNumVertices;

			// Optimization: It's possible that the non-interior edges
			// have been computed already in neighbouring voxels

			// Compute the vertex by interpolating between the two points
			int nIndex0 = CMarchingCubes::m_CubeEdges[nEdge][0];
			int nIndex1 = CMarchingCubes::m_CubeEdges[nEdge][1];

			float t = (m_fLevel - b[nIndex0])/(b[nIndex1] - b[nIndex0]);

			m_pVertices[m_nNumVertices].v[0] =
				CMarchingCubes::m_CubeVertices[nIndex0][0]*(1-t) +
			    CMarchingCubes::m_CubeVertices[nIndex1][0]*t;
			m_pVertices[m_nNumVertices].v[1] =
				CMarchingCubes::m_CubeVertices[nIndex0][1]*(1-t) +
				CMarchingCubes::m_CubeVertices[nIndex1][1]*t;
			m_pVertices[m_nNumVertices].v[2] =
				CMarchingCubes::m_CubeVertices[nIndex0][2]*(1-t) +
				CMarchingCubes::m_CubeVertices[nIndex1][2]*t;

            
			m_pVertices[m_nNumVertices].v[0] = fx +
				m_pVertices[m_nNumVertices].v[0]*m_fVoxelSize;
			m_pVertices[m_nNumVertices].v[1] = fy +
				m_pVertices[m_nNumVertices].v[1]*m_fVoxelSize;
			m_pVertices[m_nNumVertices].v[2] = fz +
				m_pVertices[m_nNumVertices].v[2]*m_fVoxelSize;
            
            // Add the vertex in the vertex vector 
            glm::vec3 v = glm::vec3(m_pVertices[m_nNumVertices].v[0],
                                    m_pVertices[m_nNumVertices].v[1],
                                    m_pVertices[m_nNumVertices].v[2]);
            m_pVertexAttribute.push_back(v);
            
			// Compute the normal at the vertex and Add normal data
			ComputeNormal(&m_pVertices[m_nNumVertices]);  

			m_nNumVertices++;
		}

		// Add the edge's vertex index to the index list
		m_pIndices[m_nNumIndices] = EdgeIndices[nEdge];
		m_nNumIndices++;

		i++;
	}

	SetGridVoxelComputed(x,y,z);
    
	if( m_nNumIndices >= MAX_INDICES-30 )  // either come inside here or not
	{
        m_pVertices.resize(m_nNumVertices);
        m_pIndices.resize(m_nNumIndices);
        
        /* 
         
		// Render the computed triangles
		//CGraphics *pGfx = CGraphics::GetInstance();  /// graphics.h
		//IDirect3DDevice8 *pDev = pGfx->GetD3DDevice();   //<d3dx8.h>

		// Unlock buffers
		//pGfx->UnlockVertexBuffer();  /// graphics.h
		//pGfx->UnlockIndexBuffer();   /// graphics.h

         // first drawelements
		//pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumVertices, 0, m_nNumIndices/3);
         
		// Lock the vertex and index buffer
		//pGfx->LockVertexBuffer(MAX_VERTICES, (BYTE**)&m_pVertices);   /// graphics.h
		//pGfx->LockIndexBuffer(MAX_INDICES, (BYTE**)&m_pIndices);      /// graphics.h
         
         
         */
        // first draw elements, Render the computed triangles
        DrawElements(useTexture);
        
        ClearVectors();
        
	}

	return c;
}

//=============================================================================
void CMetaballs::ClearVectors(){
    
    m_pVertices.clear();
    for( int i = 0; i < MAX_VERTICES; i++ ){ m_pVertices.push_back(SVertex()); }
    
    m_pIndices.clear();
    for( int i = 0; i < MAX_INDICES; i++ ) { m_pIndices.push_back(0); }
    
    m_pVertexAttribute.clear(); 
    m_pTextureAttribute.clear();
    m_pNormalAttribute.clear(); 
    
    m_nNumVertices = 0;
    m_nNumIndices = 0;
    
}

//=============================================================================
float CMetaballs::ConvertGridPointToWorldCoordinate(int x)
{
	return float(x)*m_fVoxelSize - 1.0f;
}

//=============================================================================
int CMetaballs::ConvertWorldCoordinateToGridPoint(float x)
{
	return int((x + 1.0f)/m_fVoxelSize + 0.5f);
}

//=============================================================================
void CMetaballs::SetGridSize(const int &nSize)
{
	m_fVoxelSize = 2/float(nSize);
	m_nGridSize  = nSize;

	m_pfGridEnergy      = new float[(nSize+1)*(nSize+1)*(nSize+1)];
	m_pnGridPointStatus = new char[(nSize+1)*(nSize+1)*(nSize+1)];
	m_pnGridVoxelStatus = new char[nSize*nSize*nSize];
}

//=============================================================================
inline bool CMetaballs::IsGridPointComputed(int x, int y, int z)
{
	if( m_pnGridPointStatus[x +
	                        y*(m_nGridSize+1) +
	                        z*(m_nGridSize+1)*(m_nGridSize+1)] == 1 )
		return true;
	else
		return false;
}

//=============================================================================
inline bool CMetaballs::IsGridVoxelComputed(int x, int y, int z)
{
	if( m_pnGridVoxelStatus[x +
	                        y*m_nGridSize +
	                        z*m_nGridSize*m_nGridSize] == 1 )
		return true;
	else
		return false;
}

//=============================================================================
inline bool CMetaballs::IsGridVoxelInList(int x, int y, int z)
{
	if( m_pnGridVoxelStatus[x +
	                        y*m_nGridSize +
	                        z*m_nGridSize*m_nGridSize] == 2 )
		return true;
	else
		return false;
}

//=============================================================================
inline void CMetaballs::SetGridPointComputed(int x, int y, int z)
{
	m_pnGridPointStatus[x +
	                    y*(m_nGridSize+1) +
	                    z*(m_nGridSize+1)*(m_nGridSize+1)] = 1;
}

//=============================================================================
inline void CMetaballs::SetGridVoxelComputed(int x, int y, int z)
{
	m_pnGridVoxelStatus[x +
	                    y*m_nGridSize +
	                    z*m_nGridSize*m_nGridSize] = 1;
}

//=============================================================================
inline void CMetaballs::SetGridVoxelInList(int x, int y, int z)
{
	m_pnGridVoxelStatus[x +
	                    y*m_nGridSize +
	                    z*m_nGridSize*m_nGridSize] = 2;
}


// Render the metalballs as a set of triangles
void CMetaballs::DrawElements(const GLboolean &useTexture){
    
    // Release memory on the GPU 
    glDeleteVertexArrays(1, &m_vao);
    m_vbo.Release();
    //m_ballsTexture.Release();
    
    // make and bind the VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    m_vbo.Create();
    m_vbo.Bind();
    
    
    // Compute vertex attributes and store in VBO
    for( int vert = 0; vert < m_nNumVertices; vert++ )
    {
        glm::vec3 v = m_pVertexAttribute[vert];
        glm::vec2 t = m_pTextureAttribute[vert];
        glm::vec3 n = m_pNormalAttribute[vert];
    
        m_vbo.AddVertexData(&v, sizeof(glm::vec3));
        m_vbo.AddVertexData(&t, sizeof(glm::vec2));
        m_vbo.AddVertexData(&n, sizeof(glm::vec3));
        
    }
    
    // Compute indices and store in VBO 
    for (int ind = 0; ind < m_nNumIndices; ind++) {
        unsigned int index = m_pIndices[ind];
        m_vbo.AddIndexData(&index, sizeof(unsigned int));
    }
    
    m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
    
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
    
    /*
    std::cout << std::endl;
    std::cout << "triangles: " << m_nNumVertices << ", indicies: " << m_nNumIndices << std::endl;
    std::cout << "v triangles: " << m_pVertices.size() << ", v indicies: " << m_pIndices.size() << std::endl;
    
    std::cout << "v: " << m_pVertexAttribute.size() << std::endl;
    std::cout << "t: " << m_pTextureAttribute.size() << std::endl;
    std::cout << "n: " << m_pNormalAttribute.size() << std::endl;
    */
    
    glBindVertexArray(m_vao);
    if (useTexture){
        for (GLuint i = 0; i < m_textures.size(); ++i){
            m_textures[i]->BindTexture2DToTextureType();
        }
    }
    
    // Render the metalBalls as a set of triangles
    glDrawElements(GL_TRIANGLES, m_nNumIndices, GL_UNSIGNED_INT, 0);
    //pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumVertices, 0, m_nNumIndices/3);
    
}

void CMetaballs::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

void CMetaballs::Release() {
    delete m_pOpenVoxels;
    delete m_pfGridEnergy;
    delete m_pnGridPointStatus;
    delete m_pnGridVoxelStatus;
    
    // Release memory on the GPU
    for (GLuint i = 0; i < m_textures.size(); ++i){
        m_textures[i]->Release();
        delete m_textures[i];
    }
    m_textures.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    
    m_vbo.Release();
}
