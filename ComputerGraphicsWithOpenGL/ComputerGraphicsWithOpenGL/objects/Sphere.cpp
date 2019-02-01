
#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// http://www.songho.ca/opengl/gl_sphere.html
#include "Sphere.h"

CSphere::CSphere()
{}

CSphere::~CSphere()
{
    Release();
}

// Create a unit sphere 
void CSphere::Create(std::string a_sDirectory, std::string a_sFilename, int slicesIn, int stacksIn)
{
	// check if filename passed in -- if so, load texture

	m_texture.Load(a_sDirectory+a_sFilename);

	m_directory = a_sDirectory;
	m_filename = a_sFilename;

	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_vbo.Create();
	m_vbo.Bind();
    
    // http://www.songho.ca/opengl/gl_sphere.html
	// Compute vertex attributes and store in VBO
	int vertexCount = 0;
	for (int stacks = 0; stacks < stacksIn; stacks++) {
		float phi = (stacks / (float) (stacksIn - 1)) * (float) M_PI;
		for (int slices = 0; slices <= slicesIn; slices++) {
			float theta = (slices / (float) slicesIn) * ((float)M_PI * 2);
			
            long double cosPhi = Extensions::sciToDouble(std::to_string(cos(phi)));
            long double cosTheta = Extensions::sciToDouble(std::to_string(cos(theta)));
            long double sinPhi = Extensions::sciToDouble(std::to_string(sin(phi)));
            long double sinTheta = Extensions::sciToDouble(std::to_string(sin(theta)));
            
            //glm::vec3 v = glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
			glm::vec3 v = glm::vec3(cosTheta * sinPhi, sinTheta * sinPhi, cosPhi);
			glm::vec2 t = glm::vec2((float)slices / (float) slicesIn, (float)stacks / (float) stacksIn);
			glm::vec3 n = v;
            
            m_vbo.AddVertexData(&v, sizeof(glm::vec3));
            m_vbo.AddVertexData(&t, sizeof(glm::vec2));
            m_vbo.AddVertexData(&n, sizeof(glm::vec3));
            
            vertices.push_back(v);
            uvs.push_back(t);
            normals.push_back(n);
            
			vertexCount++;
            //cout << "stacks: " << stacks << ", slices: " << slices << endl;
		}
	}
    
	// Compute indices and store in VBO
	m_numTriangles = 0;
	for (int stacks = 0; stacks < stacksIn; stacks++) {
		for (int slices = 0; slices < slicesIn; slices++) {
			unsigned int nextSlice = slices + 1;
			unsigned int nextStack = (stacks + 1) % stacksIn;

			unsigned int index0 = stacks * (slicesIn+1) + slices;
			unsigned int index1 = nextStack * (slicesIn+1) + slices;
			unsigned int index2 = stacks * (slicesIn+1) + nextSlice;
			unsigned int index3 = nextStack * (slicesIn+1) + nextSlice;

			m_vbo.AddIndexData(&index0, sizeof(unsigned int));
			m_vbo.AddIndexData(&index1, sizeof(unsigned int));
			m_vbo.AddIndexData(&index2, sizeof(unsigned int));
			m_numTriangles++;

			m_vbo.AddIndexData(&index2, sizeof(unsigned int));
			m_vbo.AddIndexData(&index1, sizeof(unsigned int));
			m_vbo.AddIndexData(&index3, sizeof(unsigned int));
			m_numTriangles++;
		}
	}

	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

    // https://stackoverflow.com/questions/22296510/what-does-stride-mean-in-opengles
    // https://www.opengl.org/discussion_boards/showthread.php/156620-glVertexPointer-Stride
    // 2 vector3 plus 1 vector2
	GLsizei stride = (2 * sizeof(glm::vec3))+sizeof(glm::vec2);

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
    // Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)));
    // Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(glm::vec3)));
    */
    
}

void CSphere::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(rotation.x);
    transform.RotateY(rotation.y);
    transform.RotateZ(rotation.z);
    transform.Scale(scale);
}

// Render the sphere as a set of triangles
void CSphere::Render(const bool &useTexture)
{
	glBindVertexArray(m_vao);
    if (useTexture) {
        this->m_texture.BindTexture2D();
    }
	glDrawElements(GL_TRIANGLES, m_numTriangles*3, GL_UNSIGNED_INT, 0);

}

// Release memory on the GPU 
void CSphere::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}
