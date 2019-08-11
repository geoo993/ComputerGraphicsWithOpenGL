
#include "Torus.h"

//https://stackoverflow.com/questions/7966362/how-to-draw-a-textured-torus-in-opengl-without-using-glut
//https://www.opengl.org/archives/resources/code/samples/redbook/torus.c
//https://gist.github.com/gyng/8939105
//https://www.3dgep.com/normal-mapping-with-cg-and-opengl/

 //https://github.com/cinder/Cinder/blob/master/src/cinder/GeomIo.cpp
 // Lengyel, Eric. "Computing Tangent Space Basis Vectors for an Arbitrary Mesh". 
 // Terathon Software 3D Graphics Library, 2001.
 // http://www.terathon.com/code/tangent.html

CTorus::CTorus()
{
    
};

CTorus::~CTorus()
{
    Release();
}

void CTorus::Create(const std::string &directory, const std::map<std::string, TextureType> &textureNames,
                    const int &radialSeg,
                    const int &circularSeg,
                    const float &outerRadius,           // radius
                    const float &innerRadius             //tick
                    ) {
    
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
    
    // make and bind the VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    m_vbo.Create();
    m_vbo.Bind();
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    // outputs
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<glm::vec3> binormals;
    
    int i, j, k;
    
    static const float sTexCoord[3] = { 2.0, 0, 0 };
    static const float tTexCoord[3] = { 0, 1.0, 0 };
   
    float x, y, z;      // POSITION
    float nx, ny, nz;   // NORMAL
    float tx, ty, tz;   // TANGENT
    float bx, by, bz;   // BINORMAL
    float s, t;         // TEXCOORD
    float u, v;          //  U V
    float cu, su;       // COS And SIN  U 
    float cv, sv;       // COS And SIN  V
    float twopi = 2.0f * glm::pi <float> (); 
    
    
    for (i = 0; i < radialSeg; i++) {
        for (j = 0; j <= circularSeg; j++) {
            for (k = 1; k >= 0; k--) {
                
                /*
                s = (i + k) % radialSeg + 0.5f; 
                t = j % (circularSeg + 1); 
                
                x = (radius + tick * cos(s * twopi/radialSeg)) * cos( t * twopi / circularSeg);
                y = (radius + tick * cos(s * twopi/radialSeg)) * sin( t * twopi / circularSeg);
                z = tick * sin(s * twopi / radialSeg);
                */
                
                u = (j % (circularSeg + 1)) / (float) circularSeg;
                v = (i + k) / (float) radialSeg ;
              
                cu = cos( u * twopi ); //cos u
                su = sin( u * twopi ); //sin u
                cv = cos( v * twopi ); //con v
                sv = sin( v * twopi ); //sin v
            
                // Position
                x = ( outerRadius + innerRadius * cv ) * cu;
                y = ( outerRadius + innerRadius * cv ) * su;
                z = innerRadius * sv;
                
                // Normal
                nx = cu * cv;
                ny = su * cv;
                nz = sv;
                
                // Tangent
                tx = ( outerRadius + innerRadius * cv ) * -su;
                ty = ( outerRadius + innerRadius * cv ) * cu;
                tz = 0.0f;
                
                // Binormal
                bx = -cu * sv;
                by = -su * cv;
                bz = cv;
                
                // U, V texture mapping
                s = ( u * sTexCoord[0] ) + ( v * sTexCoord[1] );
                t = ( v * tTexCoord[0] ) + ( v * tTexCoord[1] );
                
                //s = (i + k) % radialSeg + 0.5f; 
                //t = j % (circularSeg + 1); 
                
                glm::vec3 vert = glm::vec3(x,y,z);
                glm::vec2 uv = glm::vec2(s,t);
                glm::vec3 norm = glm::vec3(nx, ny, nz );
                glm::vec3 tan = glm::vec3(tx, ty, tz );
                glm::vec3 biNorm = glm::vec3(bx, by, bz  );//glm::cross(norm, tan);
                glm::vec3 biTan = glm::cross(norm, tan);
                
                vertices.push_back(vert);
                texCoords.push_back(uv);
                normals.push_back(norm);
                tangents.push_back(tan);
                binormals.push_back(biNorm);
                bitangents.push_back(biTan);
                
            }
        }
    }
    
    m_numTriangles = vertices.size();
    

    for (int i = 0; i < m_numTriangles; i++) {
        m_vbo.AddData(&vertices[i], sizeof(glm::vec3));
        m_vbo.AddData(&texCoords[i], sizeof(glm::vec2));
        m_vbo.AddData(&normals[i], sizeof(glm::vec3));
        m_vbo.AddData(&tangents[i], sizeof(glm::vec3));
        m_vbo.AddData(&bitangents[i], sizeof(glm::vec3));
    }
   
    
    m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
    
    // 4 vector3 plus 1 vector2
    GLsizei stride = 4 * sizeof(glm::vec3) + sizeof(glm::vec2);
    
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    
    // Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)));
    
    // Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(glm::vec3)));
    
}

void CTorus::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

void CTorus::Render(const GLboolean &useTexture)
{
    // bind the VAO (the triangle)
    glBindVertexArray(m_vao);
    if (useTexture){
        for (GLuint i = 0; i < m_textures.size(); ++i){
            m_textures[i]->BindTexture2DToTextureType();
        }
    }
    glDrawArrays( GL_TRIANGLE_STRIP, 0, m_numTriangles ); // draw the vertixes
    
}

// Release memory on the GPU 
void CTorus::Release()
{
    
    for (GLuint i = 0; i < m_textures.size(); ++i){
        m_textures[i]->Release();
        delete m_textures[i];
    }
    m_textures.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    m_vbo.Release();
    
}
