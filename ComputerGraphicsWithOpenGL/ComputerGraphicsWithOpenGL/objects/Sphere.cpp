
#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// http://www.songho.ca/opengl/gl_sphere.html
#include "Sphere.h"

CSphere::CSphere()
{
    m_vao = 0;
    m_textures = {};
}

CSphere::~CSphere()
{
    Release();
}

// Create a unit sphere
void CSphere::Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames, int slicesIn, int stacksIn)
{
    m_textureNames = textureNames;
    m_textures.reserve(textureNames.size());
    
    // Iterate through all elements in std::map
    for (auto it = textureNames.begin(); it != textureNames.end(); ++it) {
        // if the current index is needed:
        auto i = std::distance(textureNames.begin(), it);
        
        // access element as *it
        m_textures.push_back(new CTexture);
        m_textures[i]->LoadTexture(directory+it->first, it->second, true);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_textures[i]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        // any code including continue, break, return
    }

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
			
            glm::vec3 v = glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
			glm::vec2 t = glm::vec2((float)slices / (float) slicesIn, (float)stacks / (float) stacksIn);
			glm::vec3 n = v;
            
            vertices.push_back(v);
            uvs.push_back(t);
            normals.push_back(n);
            
			vertexCount++;
            //cout << "stacks: " << stacks << ", slices: " << slices << endl;
		}
	}
    
    for (long unsigned i = 0; i < vertexCount; i+=2) {
        // triangle
        glm::vec3 v0 = vertices[i+0];
        glm::vec3 v1 = vertices[i+1 % vertexCount];
        
        // triangle UVs
        glm::vec2 & uv0 = uvs[i+0];
        glm::vec2 & uv1 = uvs[i+1 % vertexCount];
        
        // triangle Normals
        glm::vec3 & norm0 = normals[i+0];
        glm::vec3 & norm1 = normals[i+1 % vertexCount];
        
        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent, bitangent;
        
        float coef = 1.0f / (uv0.x * uv1.y - uv1.x * uv0.y);
        tangent.x = coef * ((v0.x * uv1.y) + (v1.x * -uv0.y));
        tangent.y = coef * ((v0.y * uv1.y) + (v1.y * -uv0.y));
        tangent.z = coef * ((v0.z * uv1.y) + (v1.z * -uv0.y));
        tangent = glm::normalize(tangent);
        bitangent = glm::normalize(glm::cross(norm0, tangent));
        
        m_vbo.AddVertexData(&v0, sizeof(glm::vec3));
        m_vbo.AddVertexData(&uv0, sizeof(glm::vec2));
        m_vbo.AddVertexData(&norm0, sizeof(glm::vec3));
        m_vbo.AddVertexData(&tangent, sizeof(glm::vec3));
        m_vbo.AddVertexData(&bitangent, sizeof(glm::vec3));
        
        bitangent = glm::normalize(glm::cross(norm1, tangent));
        
        m_vbo.AddVertexData(&v1, sizeof(glm::vec3));
        m_vbo.AddVertexData(&uv1, sizeof(glm::vec2));
        m_vbo.AddVertexData(&norm1, sizeof(glm::vec3));
        m_vbo.AddVertexData(&tangent, sizeof(glm::vec3));
        m_vbo.AddVertexData(&bitangent, sizeof(glm::vec3));
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
    // 4 vector3 plus 1 vector2
	GLsizei stride = (4 * sizeof(glm::vec3))+sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
    
    // Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)));
    // Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(glm::vec3)));
    
}

void CSphere::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

// Render the sphere as a set of triangles
void CSphere::Render(const GLboolean &useTexture)
{
    // bind the VAO (the triangle)
    glBindVertexArray(m_vao);
    
    if (useTexture){
        for (GLuint i = 0; i < m_textures.size(); ++i){
            m_textures[i]->BindTexture2DToTextureType();
        }
    }
    glDrawElements(GL_TRIANGLES, m_numTriangles*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Release memory on the GPU
void CSphere::Release()
{
    for (GLuint i = 0; i < m_textures.size(); ++i){
        m_textures[i]->Release();
        delete m_textures[i];
    }
    m_textures.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    
    m_vbo.Release();
    
}
