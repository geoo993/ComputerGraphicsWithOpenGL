//
//  TorusKnot.h
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 22/09/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef TorusKnot_h
#define TorusKnot_h

#include "../ObjectsBase.h"


class CTorusKnot: public IGameObject {
    
public:
    
    CTorusKnot();
    ~CTorusKnot();
    void Create(const std::string &directory,
                const std::map<std::string, TextureType> &textureNames,
                int aSteps,           // in: Number of steps in the torus knot
                int aFacets,          // in: Number of facets
                float aScale,         // in: Scale of the knot
                float aThickness,     // in: Thickness of the knot
                float aClumps,        // in: Number of clumps in the knot
                float aClumpOffset,   // in: Offset of the clump (in 0..2pi)
                float aClumpScale,    // in: Scale of a clump
                float aUScale,        // in: U coordinate scale
                float aVScale,        // in: V coordinate scale
                float aP,             // in: P parameter of the knot
                float aQ);            // in: Q parameter of the knot
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    
    void Render(const GLboolean &useTexture = true);
    void Release();

private:
    
    GLuint m_vao, m_numVertices, m_numIndices;
    CVertexBufferObjectIndexed m_vbo;
    
    std::map<std::string, TextureType> m_textureNames;
    std::vector<CTexture*> m_textures;
};

#endif /* TorusKnot_h */
