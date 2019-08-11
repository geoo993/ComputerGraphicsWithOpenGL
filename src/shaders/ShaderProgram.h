

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
 
    // Setting Uniform Buffer Objects
    void SetUniformBlock(std::string uniformName, const GLint &bindingPoint);
    
    // Setting vectors
    void SetUniform(std::string sName, glm::vec2* vVectors, const GLint &iCount = 1);
    void SetUniform(std::string sName, const glm::vec2 vVector);
    void SetUniform(std::string sName, glm::vec3* vVectors, const GLint &iCount = 1);
    void SetUniform(std::string sName, const glm::vec3 vVector);
    void SetUniform(std::string sName, glm::vec4* vVectors, const GLint &iCount = 1);
    void SetUniform(std::string sName, const glm::vec4 vVector);
    
    // Setting floats
    void SetUniform(std::string sName, GLfloat* fValues, const GLint & iCount = 1);
    void SetUniform(std::string sName, const GLfloat &fValue);
    
    // Setting 3x3 matrices
    void SetUniform(std::string sName, glm::mat3* mMatrices, const GLint & iCount = 1);
    void SetUniform(std::string sName, const glm::mat3 mMatrix);
    
    // Setting 4x4 matrices
    void SetUniform(std::string sName, glm::mat4* mMatrices, const GLint & iCount = 1);
    void SetUniform(std::string sName, const glm::mat4 mMatrix);
    
    // Setting integers
    void SetUniform(std::string sName, GLint* iValues, const GLint & iCount = 1);
    void SetUniform(std::string sName, const GLint & iValue);
    
    void Release();
private:
    uint m_uiProgram; // ID of program
    bool m_bLinked; // Whether program was linked and is ready to use
};


#endif /* ShaderProgram_h */
