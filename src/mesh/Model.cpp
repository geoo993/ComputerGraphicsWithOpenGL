
// https://learnopengl.com/Model-Loading/Model
//https://www.youtube.com/watch?v=dF5rOveGOJc&index=2&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl

#include "Model.h"

CModel::CModel()
{
    m_meshes.clear();
    m_mesheTextures.clear();
    m_textureNames = {};
}


CModel::~CModel()
{
    Release();
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
GLboolean CModel::Create(const std::string &modelPath,  const std::string &texturesPath, const std::map<std::string, TextureType> &texturesName)
{
    // Release the previously loaded mesh (if it exists)
    Release();

    /*
     The great thing about Assimp is that it neatly abstracts from all the technical details of loading all the different file formats and does all this with a single one-liner:


     Assimp::Importer importer;
     const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

     We first declare an actual Importer object from Assimp's namespace and then call its ReadFile function. The function expects a file path and as its second argument several post-processing options. Aside from simply loading the file, Assimp allows us to specify several options that forces Assimp to do some extra calculations/operations on the imported data. By setting aiProcess_Triangulate we tell Assimp that if the model does not (entirely) consist of triangles it should transform all the model's primitive shapes to triangles. The aiProcess_FlipUVs flips the texture coordinates on the y-axis where necessary during processing (you might remember from the Textures tutorial that most images in OpenGL were reversed around the y-axis so this little postprocessing option fixes that for us). A few other useful options are:

     aiProcess_GenNormals : actually creates normals for each vertex if the model didn't contain normal vectors.
     aiProcess_SplitLargeMeshes : splits large meshes into smaller sub-meshes which is useful if your rendering has a maximum number of vertices allowed and can only process smaller meshes.
     aiProcess_OptimizeMeshes : actually does the reverse by trying to join several meshes into one larger mesh, reducing drawing calls for optimization.

     Assimp provides a great set of postprocessing instructions and you can find all of them here (http://assimp.sourceforge.net/lib_html/postprocess_8h.html). Actually loading a model via Assimp is (as you can see) surprisingly easy. The hard work lies in using the returned scene object to translate the loaded data to an array of Mesh objects.

     */

    bool isProcessed = false;
    
    // read file via ASSIMP
    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(
                                              modelPath,
                                              aiProcess_Triangulate |
                                              aiProcess_GenSmoothNormals |
                                              aiProcess_FlipUVs |
                                              aiProcess_CalcTangentSpace
                                              );
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
        return isProcessed;
    }

    if (scene) {
        // process ASSIMP's root node recursively
        isProcessed = ProcessNode(scene, scene->mRootNode, texturesPath);
        
        LoadTextures(texturesPath, texturesName);
    }
    
    return isProcessed;
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
GLboolean CModel::ProcessNode(const aiScene *scene, aiNode *node, const std::string &directory)
{


    /*  Mesh
     When modelling objects in modelling toolkits, artists generally do not create an entire model out of a single shape. Usually each model has several sub-models/shapes that it consists of. Each of those single shapes that a model is composed of is called a mesh. Think of a human-like character: artists usually model the head, limbs, clothes, weapons all as separate components and the combined result of all these meshes represents the final model. A single mesh is the minimal representation of what we need to draw an object in OpenGL (vertex data, indices and material properties). A model (usually) consists of several meshes.
     */
    
    GLboolean isProcessed = false;
    
    // process all the node's meshes (if any)
    for (GLuint i = 0; i < node->mNumMeshes ; i++) {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        //aiMesh* mesh = scene->mMeshes[i];
        m_meshes.push_back(ProcessMesh(scene, mesh, directory));
        isProcessed = true;
    }
    
    // then do the same for each of its children.
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        isProcessed = ProcessNode(scene, node->mChildren[i], directory);
    }
    
    return isProcessed;
}


Mesh * CModel::ProcessMesh(const aiScene *scene, const aiMesh *mesh, const std::string &directory)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<CTexture*> textures;
    
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
    // Walk through each of the mesh's vertices
    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        
        // process vertex positions, normals and texture coordinates
        const aiVector3D* pPos      = &(mesh->mVertices[i]);
        const aiVector3D* pNormal   = &(mesh->mNormals[i]);
        const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
        const aiVector3D* pTangent   = &(mesh->mTangents[i]);
        const aiVector3D* pBitangents   = &(mesh->mBitangents[i]);

        if (mesh->HasTangentsAndBitangents() == true )
        {
            vertex = Vertex(
                            glm::vec3(pPos->x, pPos->y, pPos->z),
                            glm::vec2(pTexCoord->x, 1.0f-pTexCoord->y),
                            glm::vec3(pNormal->x, pNormal->y, pNormal->z),
                            glm::vec3(pTangent->x, pTangent->y, pTangent->z),
                            glm::vec3(pBitangents->x, pBitangents->y, pBitangents->z)
                       );
        } else {
            glm::vec3 normal = glm::vec3(pNormal->x, pNormal->y, pNormal->z);
            // old implementation
            glm::vec3 tangent = glm::vec3(0.0f, normal.z , 1.0f);
            glm::vec3 bitangent = glm::cross(tangent, normal);

            // new implemetation
            // http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
            // https://stackoverflow.com/questions/5255806/how-to-calculate-tangent-and-binormal
            glm::vec3 c1 = glm::cross(normal, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::vec3 c2 = glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f));

            if (glm::length(c1) > glm::length(c2))
            {
                tangent = c1;
            }else{
                tangent = c2;
            }
            tangent = glm::normalize(tangent);
            bitangent = glm::normalize(glm::cross(normal, tangent));

            vertex = Vertex(
                       glm::vec3(pPos->x, pPos->y, pPos->z),
                       glm::vec2(pTexCoord->x, 1.0f-pTexCoord->y),
                       normal,
                       tangent,
                       bitangent
                       );
        }

        vertices.push_back(vertex);
    }

    // process indices
    // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        
        /// retrieve all indices of the face and store them in the indices vector
        for(GLuint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
        textures = ProcessMaterials(scene, mesh->mMaterialIndex, directory);
    }
    /*
    std::cout
    << "num materials " << scene->mNumMaterials << ", material index: " << mesh->mMaterialIndex << std::endl
    << directory << std::endl
    << vertices.size() << std::endl
    << indices.size() << std::endl
    << textures.size() << std::endl
    << mesh->mMaterialIndex << std::endl
    << mesh->mNumFaces << std::endl
    << std::endl << std::endl;
    */
    return new Mesh(vertices, indices, textures, mesh->mMaterialIndex, mesh->mNumFaces);
}

std::vector<CTexture*> CModel::ProcessMaterials(const aiScene* scene, const GLuint &materialIndex, const std::string& directory)
{
    
    bool isProcessed = true;
    std::vector<CTexture*> textures;
    
    const aiMaterial* material = scene->mMaterials[materialIndex];

    // 0. ambient map texture
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_AMBIENT); i++)
    {
        CTexture* ambientMap = LoadTexture(material, i, aiTextureType_AMBIENT, TextureType::AMBIENT, directory);
        if(ambientMap != nullptr)textures.insert(textures.end(), ambientMap);
        //std::cout << "ambient type: " << aiTextureType_AMBIENT << ", texture index: " << i << std::endl;
    }
    
    // 1. diffuse map texture
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
    {
        CTexture* diffuseMap = LoadTexture(material, i, aiTextureType_DIFFUSE, TextureType::DIFFUSE, directory);
        if(diffuseMap != nullptr)textures.insert(textures.end(), diffuseMap);
        //std::cout << "diffuse type: " << aiTextureType_DIFFUSE << ", texture index: " << i << std::endl;
    }
    
    // 2. specular map texture
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++)
    {
        CTexture* specularMap = LoadTexture(material, i, aiTextureType_SPECULAR, TextureType::SPECULAR, directory);
        if(specularMap != nullptr)textures.insert(textures.end(), specularMap);
        //std::cout << "specular type: " << aiTextureType_SPECULAR << ", texture index: " << i << std::endl;
    }
    
    // 3. (tangent space) normal map texture
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_NORMALS); i++)
    {
        CTexture* normalMap = LoadTexture(material, i, aiTextureType_NORMALS, TextureType::NORMAL, directory);
        if(normalMap != nullptr)textures.insert(textures.end(), normalMap);
        //std::cout << "normal type: " << aiTextureType_NORMALS << ", texture index: " << i << std::endl;
    }
    
    // 4. height map texture
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_HEIGHT); i++)
    {
        CTexture* heightMap = LoadTexture(material, i, aiTextureType_HEIGHT,TextureType::HEIGHT, directory);
        if(heightMap != nullptr)textures.insert(textures.end(), heightMap);
        //std::cout << "height type: " << aiTextureType_HEIGHT << ", texture index: " << i << std::endl;
    }
    
    // 5. The texture is added to the result of the lighting calculation. It isn't influenced by incoming light.
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_EMISSIVE); i++)
    {
        CTexture* emissionMap = LoadTexture(material, i, aiTextureType_EMISSIVE, TextureType::EMISSION, directory);
        if(emissionMap != nullptr)textures.insert(textures.end(), emissionMap);
        //std::cout << "emission type: " << aiTextureType_EMISSIVE << ", texture index: " << i << std::endl;
    }
    
    // 6. Displacement texture
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_DISPLACEMENT); i++)
    {
        CTexture* displacementMap = LoadTexture(material, i, aiTextureType_DISPLACEMENT, TextureType::DISPLACEMENT, directory);
        if(displacementMap != nullptr)textures.insert(textures.end(), displacementMap);
        //std::cout << "displacement type: " << aiTextureType_DISPLACEMENT << ", texture index: " << i << std::endl;
    }
    
    // 7. Lightmap texture (aka Ambient Occlusion)
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_LIGHTMAP); i++)
    {
        CTexture* aoMap = LoadTexture(material, i, aiTextureType_LIGHTMAP, TextureType::AO, directory);
        if(aoMap != nullptr)textures.insert(textures.end(), aoMap);
        //std::cout << "Ambient Occlusion type: " << aiTextureType_LIGHTMAP << ", texture index: " << i << std::endl;
    }
    
    // 8. The texture defines the glossiness of the material.
    // The glossiness is in fact the exponent of the specular
    // (phong) lighting equation. Usually there is a conversion
    // function defined to map the linear color values in the
    // texture to a suitable exponent.
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_SHININESS); i++)
    {
        CTexture* shininessMap = LoadTexture(material, i, aiTextureType_SHININESS, TextureType::GLOSSINESS, directory);
        if(shininessMap != nullptr)textures.insert(textures.end(), shininessMap);
        //std::cout << "glossiness type: " << aiTextureType_SHININESS << ", texture index: " << i << std::endl;
    }
    
    // 9. The texture defines per-pixel opacity.
    // Usually 'white' means opaque and 'black' means
    // 'transparency'. Or quite the opposite. Have fun.
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_OPACITY); i++)
    {
        CTexture* opacityMap = LoadTexture(material, i, aiTextureType_OPACITY, TextureType::OPACITY, directory);
        if(opacityMap != nullptr)textures.insert(textures.end(), opacityMap);
        //std::cout << "opacity type: " << aiTextureType_OPACITY << ", texture index: " << i << std::endl;
    }
    
    /*
    // 10. Reflection texture, Contains the color of a perfect mirror reflection.
    for(GLuint i = 0; i < material->GetTextureCount(aiTextureType_REFLECTION); i++)
    {
        CTexture* reflectionMap = LoadTexture(material, i, aiTextureType_REFLECTION, TextureType::REFLECTION, directory);
        if(reflectionMap != nullptr)textures.insert(textures.end(), reflectionMap);
        //std::cout << "reflection type: " << aiTextureType_REFLECTION << ", texture index: " << i << std::endl;
    }
    */
    return textures;
}

CTexture* CModel::CreateColorTexture(const aiMaterial *pMaterial,
                                                   const TextureType &typeName,
                                                   const char* matKey,
                                                   const GLuint &type,
                                                   const GLuint &index) {
    aiColor3D color (0.0f, 0.0f, 0.0f);
    pMaterial->Get(matKey, type, index, color);
    
    CTexture* tex = new CTexture();
    BYTE data[3];
    data[0] = (BYTE) (color[2]*255);
    data[1] = (BYTE) (color[1]*255);
    data[2] = (BYTE) (color[0]*255);
    tex->CreateFromData(data, 1, 1, 24, GL_BGR, typeName, false);
    return tex;
}

CTexture*  CModel::LoadTexture(const aiMaterial *pMaterial,
                                             const GLuint &index,
                                             const aiTextureType &type,
                                             const TextureType &typeName,
                                             const std::string &directory)
{
    
    aiString path;
    
    if (pMaterial->GetTexture(type, index, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
        GLboolean skip = false;
        CTexture* texture = new CTexture();
        
        for(GLuint j = 0; j < m_mesheTextures.size(); j++)
        {
            std::string texturePath = (directory + path.C_Str());
            if(std::strcmp(m_mesheTextures[j]->GetPath().c_str(), texturePath.c_str()) == 0)
            {
                texture = m_mesheTextures[j];
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        
        if(skip == false)
        {
            std::string fullPath = directory + path.data;
            GLboolean load = texture->LoadTexture(fullPath.c_str(), typeName, true);
            
            if (load == false) {
                delete texture;
                texture = CreateColorTexture(pMaterial, typeName, AI_MATKEY_COLOR_DIFFUSE);
                //printf("Texture did not load '%s'\n", fullPath.c_str());
            }
            else {
                //printf("Loaded texture '%s'\n", fullPath.c_str());
            }
            
            texture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            texture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            texture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
            texture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            m_mesheTextures.push_back(texture);
        }
        return texture;
    }
    
    return nullptr;
}

void CModel::LoadTextures(const std::string &directory, const std::map<std::string, TextureType> &textureNames) {
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
}

void CModel::Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) {
    transform.SetIdentity();
    transform.Translate(position.x, position.y, position.z);
    transform.RotateX(glm::radians(rotation.x));
    transform.RotateY(glm::radians(rotation.y));
    transform.RotateZ(glm::radians(rotation.z));
    transform.Scale(scale);
}

void CModel::Render(CShaderProgram *pShaderProgram, const GLboolean &useTexture)
{
    if (useTexture){
        for (GLuint i = 0; i < m_textures.size(); ++i){
            m_textures[i]->BindTexture2DToTextureType();
        }
    }
    for (unsigned int i = 0 ; i < m_meshes.size() ; i++) {
        m_meshes[i]->Render(pShaderProgram, false);
    }
}

void CModel::RenderWithMeshTexture(CShaderProgram *pShaderProgram, const GLboolean &useTexture) {
    for (unsigned int i = 0 ; i < m_meshes.size(); i++) {
        m_meshes[i]->Render(pShaderProgram, useTexture);
    }
}

void CModel::Render(const GLboolean &useTexture) {}

void CModel::Release()
{
    for (GLuint i = 0 ; i < m_mesheTextures.size() ; i++) {
        m_mesheTextures[i]->Release();
    }
    m_mesheTextures.clear();
    
    for (GLuint i = 0; i < m_textures.size(); ++i){
        m_textures[i]->Release();
        delete m_textures[i];
    }
    m_textures.clear();
    
    for(GLuint i = 0; i < m_meshes.size(); i++){
        m_meshes[i]->Release();
    }
    m_meshes.clear();
}
