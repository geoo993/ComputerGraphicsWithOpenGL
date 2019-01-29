/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


//https://www.youtube.com/watch?v=dF5rOveGOJc&index=2&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl


#include "OpenAssetImportMesh.h"

COpenAssetImportMesh::COpenAssetImportMesh()
{
    m_UseInstances = false;
    m_InstanceCount = 0;
    m_Center = glm::vec3(0.0f);
    m_Scale = 0.0f;
}


COpenAssetImportMesh::~COpenAssetImportMesh()
{
    Clear();
}

void COpenAssetImportMesh::Clear()
{
    for (unsigned int i = 0 ; i < m_Textures.size() ; i++) {
        SAFE_DELETE(m_Textures[i]);
    }
    glDeleteVertexArrays(1, &m_vao);
}

bool COpenAssetImportMesh::LoadWithInstances(
                                             const std::string& Filename,
                                             const bool &withIntances,
                                             const GLuint &instanceCount,
                                             const glm::vec3 &center,
                                             const float &scale){

    m_UseInstances = withIntances;
    m_InstanceCount = instanceCount;
    m_Center = center;
    m_Scale = scale;

    m_ModelMatrixInstances = GetModelMatrixInstancesData();

    return Load(Filename);
}

bool COpenAssetImportMesh::Load(const std::string& Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();

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

    bool Ret = false;
    Assimp::Importer Importer;
    const aiScene* pScene = Importer.ReadFile(
                                              Filename.c_str(),
                                              aiProcess_Triangulate |
                                              aiProcess_GenSmoothNormals |
                                              aiProcess_FlipUVs |
                                              aiProcess_CalcTangentSpace
                                              );
    if(!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
        return false;
    }

    if (pScene) {
        Ret = InitFromScene(pScene, Filename);
    }

    return Ret;
}

bool COpenAssetImportMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
    m_Meshes.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    /*  Mesh
     When modelling objects in modelling toolkits, artists generally do not create an entire model out of a single shape. Usually each model has several sub-models/shapes that it consists of. Each of those single shapes that a model is composed of is called a mesh. Think of a human-like character: artists usually model the head, limbs, clothes, weapons all as separate components and the combined result of all these meshes represents the final model. A single mesh is the minimal representation of what we need to draw an object in OpenGL (vertex data, indices and material properties). A model (usually) consists of several meshes.
     */

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Meshes.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, Filename);
}

void COpenAssetImportMesh::InitMesh(GLuint Index, const aiMesh* paiMesh)
{
    m_Meshes[Index].m_materialIndex = paiMesh->mMaterialIndex;

    std::vector<Vertex> Vertices;
    std::vector<GLuint> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        const aiVector3D* pTangent   = &(paiMesh->mTangents[i]);
        const aiVector3D* pBitangents   = &(paiMesh->mBitangents[i]);

        Vertex v;

        glm::vec3 normal = glm::vec3(pNormal->x, pNormal->y, pNormal->z);


        if (paiMesh->HasTangentsAndBitangents() == true )
        {
            glm::vec3 tangent = glm::vec3(pTangent->x, pTangent->y, pTangent->z);
            glm::vec3 bitangent = glm::vec3(pBitangents->x, pBitangents->y, pBitangents->z);
            v = Vertex(
                       glm::vec3(pPos->x, pPos->y, pPos->z),
                       glm::vec2(pTexCoord->x, 1.0f-pTexCoord->y),
                       normal,
                       tangent,
                       bitangent
                       );
        }else{
            // old implementation
            glm::vec3 tangent = glm::vec3(0.0f,normal.z , 1.0f);
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
            tangent = normalize(tangent);
            bitangent = glm::normalize(glm::cross(normal, tangent));

            v = Vertex(
                       glm::vec3(pPos->x, pPos->y, pPos->z),
                       glm::vec2(pTexCoord->x, 1.0f-pTexCoord->y),
                       normal,
                       tangent,
                       bitangent
                       );
        }

        Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        //sassert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_Meshes[Index].m_numFaces = paiMesh->mNumFaces;
    m_Meshes[Index].Init(Vertices, Indices, m_ModelMatrixInstances);
}

bool COpenAssetImportMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = nullptr;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            //for(unsigned int i = 0; i < pMaterial->GetTextureCount(type); i++){
            m_Textures[i] = LoadMaterialTextures(pMaterial, aiTextureType_DIFFUSE, Dir);
        }else if (pMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            m_Textures[i] = LoadMaterialTextures(pMaterial, aiTextureType_SPECULAR, Dir);
        }

        // Load a single colour texture matching the diffuse colour if no texture added (if nullptr)
        if (!m_Textures[i]) {
            Ret = false;
            m_Textures[i] = CreateColorTexture(AI_MATKEY_COLOR_DIFFUSE, pMaterial);
        }
    }

    return Ret;
}

CTexture*  COpenAssetImportMesh::LoadMaterialTextures(const aiMaterial *pMaterial,
                                                      const aiTextureType &type,
                                                      const std::string &directory)
{

    aiString Path;

    if (pMaterial->GetTexture(type, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
        CTexture* tex = new CTexture();
        std::string FullPath = directory + "/" + Path.data;
        bool load = tex->Load(FullPath, true);
        if (!load) {
            delete tex;
            tex = nullptr;
        }
        else {
            //printf("Loaded diffuse texture '%s'\n", FullPath.c_str());
        }

        return tex;
    }

    return nullptr;
}

CTexture* COpenAssetImportMesh::CreateColorTexture(const char* pMatKey,
                                                   GLuint pType,
                                                   GLuint pIndex,
                                                   const aiMaterial* pMaterial){

    aiColor3D color (0.0f, 0.0f, 0.0f);
    pMaterial->Get(pMatKey,pType,pIndex,color);

    CTexture* tex = new CTexture();
    BYTE data[3];
    data[0] = (BYTE) (color[2]*255);
    data[1] = (BYTE) (color[1]*255);
    data[2] = (BYTE) (color[0]*255);
    tex->CreateFromData(data, 1, 1, 24, GL_BGR, false);


    return tex;
}

std::vector<glm::mat4> COpenAssetImportMesh::GetModelMatrixInstancesData(){

    const GLuint amount = m_InstanceCount;
    std::vector<glm::mat4> m;
    m.reserve(amount);

    float radius = 1000.0f;
    float offset = 200.0f;
    for(unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model;
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, m_Center + glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale * m_Scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        m.push_back(model);
    }

    return m;
}

void COpenAssetImportMesh::Render()
{


    //cout << m_Meshes.size() << " " << helicopterRotor << endl;

    for (unsigned int i = 0 ; i < m_Meshes.size() ; i++) {
        glBindVertexArray(m_vao);


        /*
         // sizes
         std::cout << std::endl;
         std::cout << "float: " << sizeof(float) << std::endl; /// 4
         std::cout << "double: " << sizeof(double) << std::endl;  /// 8
         std::cout << "int: " << sizeof(int) << std::endl; /// 4
         std::cout << "unsigned int: " << sizeof(unsigned int) << std::endl; /// 4
         std::cout << "bool: " << sizeof(bool) << std::endl; //// 1
         std::cout << "vec2: " << sizeof(glm::vec2) << std::endl; /// 8
         std::cout << "vec3: " << sizeof(glm::vec3) << std::endl; /// 12
         std::cout << "vec4: " << sizeof(glm::vec4) << std::endl; /// 16
         std::cout << "mat3: " << sizeof(glm::mat3) << std::endl; /// vec3 * 3 = 36
         std::cout << "mat4: " << sizeof(glm::mat4) << std::endl; /// vec4 * 4 = 64

         // base alignment     // aligned offset
         float value;          // 4
         float double;         // 8
         glm::vec3 vector2;    // 8   same as  (float * 2)
         glm::vec3 vector3;    // 12  same as  (float * 3)
         glm::vec4 vector4;    // 16  same as  (float * 4)
         mat2 matrix3x3;       // 36  same as  (vec3 * 3)
         mat4 matrix4x4;       // 64  same as  (vec4 * 4)
         float values[3];      // 12  same as  (float * 3)
         bool boolean;         // 1
         int integer;          // 4

         */

        glBindBuffer(GL_ARRAY_BUFFER, m_Meshes[i].vbo);

        //vertex
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

        //texcoord
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)12);//(void*)sizeof(glm::vec3));

        //normal
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)20);//(void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

        //tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)32); //(void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)));

        //bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44);//(void*)(sizeof(glm::vec3)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(glm::vec3)));

        glBindBuffer(GL_ARRAY_BUFFER, m_Meshes[i].mbo);

        // vertex Attributes stride
        GLsizei vec4Size = sizeof(glm::vec4);

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));

        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));

        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Meshes[i].ibo);

        const unsigned int MaterialIndex = m_Meshes[i].m_materialIndex;

        if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->BindTexture2D(0);
        }

        if ( m_UseInstances ) {
            glDrawElementsInstanced(GL_TRIANGLES, m_Meshes[i].m_numIndices, GL_UNSIGNED_INT, 0, m_InstanceCount);
        }else{
            glDrawElements(GL_TRIANGLES, m_Meshes[i].m_numIndices, GL_UNSIGNED_INT, 0);
        }

        // need to unbind because we are binding attributes every frame
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
        glDisableVertexAttribArray(5);
        glDisableVertexAttribArray(6);
        glDisableVertexAttribArray(7);
        glDisableVertexAttribArray(8);

    }


}

