

#ifndef ShaderProgram_h
#define ShaderProgram_h

#include "Shaders.h"

// A class the provides a wrapper around an OpenGL shader program
class CShaderProgram
{
public:
    CShaderProgram();
    ~CShaderProgram();
    
    void CreateProgram();
    void DeleteProgram();
    
    bool AddShaderToProgram(CShader* shShader);
    bool LinkProgram();
    
    void UseProgram();
    
    uint GetProgramID();
    
    /*
     void SetMaterialUniform(
     const std::string &uniformName,
     const float &shininess = 32.0f,
     const float &intensity = 9.0f,
     const glm::vec3 &color = glm::vec3(1.0f)
     );
     void SetDirectionalLightUniform(
     const std::string &uniformName,
     const DirectionalLight & directionalLight,
     const glm::vec3& direction);
     void SetPointLightUniform(
     const std::string &uniformName,
     const PointLight& pointLight);
     
     void SetSpotLightUniform(
     const std::string &uniformName,
     const SpotLight& spotLight,
     const glm::vec3& direction);
     */
    
    // Setting Uniform Buffer Objects
    void SetUniformBlock(std::string uniformName, const int &bindingPoint);
    
    // Setting vectors
    void SetUniform(std::string sName, glm::vec2* vVectors, int iCount = 1);
    void SetUniform(std::string sName, const glm::vec2 vVector);
    void SetUniform(std::string sName, glm::vec3* vVectors, int iCount = 1);
    void SetUniform(std::string sName, const glm::vec3 vVector);
    void SetUniform(std::string sName, glm::vec4* vVectors, int iCount = 1);
    void SetUniform(std::string sName, const glm::vec4 vVector);
    
    // Setting floats
    void SetUniform(std::string sName, float* fValues, int iCount = 1);
    void SetUniform(std::string sName, const float fValue);
    
    // Setting 3x3 matrices
    void SetUniform(std::string sName, glm::mat3* mMatrices, int iCount = 1);
    void SetUniform(std::string sName, const glm::mat3 mMatrix);
    
    // Setting 4x4 matrices
    void SetUniform(std::string sName, glm::mat4* mMatrices, int iCount = 1);
    void SetUniform(std::string sName, const glm::mat4 mMatrix);
    
    // Setting integers
    void SetUniform(std::string sName, int* iValues, int iCount = 1);
    void SetUniform(std::string sName, const int iValue);
    
    
private:
    uint m_uiProgram; // ID of program
    bool m_bLinked; // Whether program was linked and is ready to use
};


#endif /* ShaderProgram_h */
