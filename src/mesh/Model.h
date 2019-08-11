
#ifndef Model_h
#define Model_h

#pragma once

#include "Mesh.h"

class CModel: public IGameObject
{
public:
    CModel();
    ~CModel();
    GLboolean Create(const std::string &modelPath,  const std::string &texturesPath, const std::map<std::string, TextureType> &texturesName);
    void Transform(const glm::vec3 & position,
                   const glm::vec3 & rotation = glm::vec3(0, 0, 0),
                   const glm::vec3 & scale = glm::vec3(1, 1, 1));
    glm::mat4 Model() const { return transform.GetModel(); }
    void Render(CShaderProgram *pShaderProgram, const GLboolean &useTexture = true);
    void RenderWithMeshTexture(CShaderProgram *pShaderProgram, const GLboolean &useTexture = true);
    void Release();
private:
    /*  Model Data */
    std::vector<Mesh*> m_meshes; // All the polygon meshes within this object.
    std::vector<CTexture*> m_mesheTextures; // mesh textures that are currently loaded in the model
    std::map<std::string, TextureType> m_textureNames;
    std::vector<CTexture*> m_textures;
    
    /*  Functions   */
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    GLboolean ProcessNode(const aiScene *scene, aiNode *node, const std::string &directory);
    Mesh* ProcessMesh(const aiScene *scene, const aiMesh *mesh, const std::string &directory);
    

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    std::vector<CTexture*> ProcessMaterials(const aiScene* scene, const GLuint &materialIndex, const std::string& directory);
    CTexture* CreateColorTexture(const aiMaterial *pMaterial,
                                 const TextureType &typeName,
                                 const char* matKey,
                                 const GLuint &type,
                                 const GLuint &index);
    CTexture* LoadTexture(const aiMaterial *pMaterial,
                          const GLuint &index,
                          const aiTextureType &type,
                          const TextureType &typeName,
                          const std::string &directory);
    
    void Render(const GLboolean &useTexture = true);
    void LoadTextures(const std::string &directory, const std::map<std::string, TextureType> &textureNames);
        
};


#endif /* Model_h */
