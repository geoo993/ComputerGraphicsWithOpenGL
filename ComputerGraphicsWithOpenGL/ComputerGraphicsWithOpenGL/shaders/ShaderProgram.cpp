//
//  ShaderProgram.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "ShaderProgram.h"

CShaderProgram::CShaderProgram()
{
    m_bLinked = false;
}

CShaderProgram::~CShaderProgram()
{}

// Creates a new shader program
void CShaderProgram::CreateProgram()
{
    m_uiProgram = glCreateProgram();
}

// Adds a compiled shader to a program
bool CShaderProgram::AddShaderToProgram(CShader* shShader)
{
    if(!shShader->IsLoaded())
        return false;
    
    glAttachShader(m_uiProgram, shShader->GetShaderID());
    
    return true;
}

// Performs final linkage of the OpenGL shader program
bool CShaderProgram::LinkProgram()
{
    glLinkProgram(m_uiProgram);
    int iLinkStatus;
    glGetProgramiv(m_uiProgram, GL_LINK_STATUS, &iLinkStatus);
    
    if (iLinkStatus == false)
    {
        char sInfoLog[1024];
        char sFinalMessage[1536];
        int iLogLength;
        glGetProgramInfoLog(m_uiProgram, 1024, &iLogLength, sInfoLog);
        printf(sFinalMessage, "Error! Shader program wasn't linked! The linker returned:\n\n%s", sInfoLog);
        return false;
    }
    
    m_bLinked = iLinkStatus == GL_TRUE;
    return m_bLinked;
}

// Deletes the program and frees memory on the GPU
void CShaderProgram::DeleteProgram()
{
    if(!m_bLinked)
        return;
    m_bLinked = false;
    glDeleteProgram(m_uiProgram);
}

// Instructs OpenGL to use this program
void CShaderProgram::UseProgram()
{
    if(m_bLinked)
        glUseProgram(m_uiProgram);
}

// Returns the OpenGL program ID
uint CShaderProgram::GetProgramID()
{
    return m_uiProgram;
}
/*
 void CShaderProgram::SetMaterialUniform(const std::string &uniformName,
 const float &shininess,
 const float &intensity,
 const glm::vec3 &color
 ){
 SetUniform((uniformName + ".ambientMap").c_str(), 0);
 SetUniform((uniformName + ".normalMap").c_str(), 1);
 SetUniform((uniformName + ".diffuseMap").c_str(), 2);
 SetUniform((uniformName + ".specularMap").c_str(), 3);
 SetUniform((uniformName + ".heightMap").c_str(), 4);
 SetUniform((uniformName + ".shininess").c_str(), shininess);
 SetUniform((uniformName + ".intensity").c_str(), intensity);
 SetUniform((uniformName + ".color").c_str(), color);
 
 }
 
 void CShaderProgram::SetDirectionalLightUniform(
 const std::string &uniformName,
 const DirectionalLight& directionalLight,
 const glm::vec3& direction)
 {
 SetUniform((uniformName + ".base.color").c_str(), directionalLight.color);
 SetUniform((uniformName + ".base.intensity").c_str(), directionalLight.intensity);
 SetUniform((uniformName + ".base.power").c_str(), directionalLight.power);
 SetUniform((uniformName + ".base.ambient").c_str(), glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
 SetUniform((uniformName + ".base.diffuse").c_str(), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
 SetUniform((uniformName + ".base.specular").c_str(), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
 SetUniform((uniformName + ".direction").c_str(), direction);
 
 }
 
 void CShaderProgram::SetPointLightUniform(
 const std::string &uniformName,
 const PointLight& pointLight)
 {
 
 // POINT LIGHT
 SetUniform((uniformName + ".base.color").c_str(), pointLight.color);
 SetUniform((uniformName + ".base.intensity").c_str(), pointLight.intensity);
 SetUniform((uniformName + ".base.power").c_str(), pointLight.power);
 SetUniform((uniformName + ".base.ambient").c_str(), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
 SetUniform((uniformName + ".base.diffuse").c_str(), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
 SetUniform((uniformName + ".base.specular").c_str(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
 SetUniform((uniformName + ".atten.constant").c_str(), pointLight.atten.constant);
 SetUniform((uniformName + ".atten.linear").c_str(), pointLight.atten.linear);
 SetUniform((uniformName + ".atten.exponent").c_str(), pointLight.atten.exponent);
 SetUniform((uniformName + ".position").c_str(), pointLight.position);
 SetUniform((uniformName + ".range").c_str(), pointLight.range);
 
 
 }
 
 void CShaderProgram::SetSpotLightUniform(
 const std::string &uniformName,
 const SpotLight& spotLight,
 const glm::vec3& direction)
 {
 // SPOT LIGHT
 SetUniform((uniformName + ".pointLight.base.color").c_str(), spotLight.color);
 SetUniform((uniformName + ".pointLight.base.intensity").c_str(), spotLight.intensity);
 SetUniform((uniformName + ".pointLight.base.power").c_str(), spotLight.power);
 SetUniform((uniformName + ".pointLight.base.ambient").c_str(), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
 SetUniform((uniformName + ".pointLight.base.diffuse").c_str(), glm::vec4(1.0f, 1.0f, 1.0f, 1.f));
 SetUniform((uniformName + ".pointLight.base.specular").c_str(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
 SetUniform((uniformName + ".pointLight.atten.constant").c_str(), spotLight.atten.constant);
 SetUniform((uniformName + ".pointLight.atten.linear").c_str(), spotLight.atten.linear);
 SetUniform((uniformName + ".pointLight.atten.exponent").c_str(), spotLight.atten.exponent);
 SetUniform((uniformName + ".pointLight.position").c_str(), spotLight.position );
 SetUniform((uniformName + ".pointLight.range").c_str(), spotLight.range);
 SetUniform((uniformName + ".direction").c_str(), direction );
 SetUniform((uniformName + ".cutOff").c_str(), spotLight.cutOff);
 SetUniform((uniformName + ".outerCutOff").c_str(), spotLight.outerCutOff);
 
 }
 */

// A collection of functions to set uniform variables inside shaders

// Setting Uniform Buffer Objects

void CShaderProgram::SetUniformBlock(std::string uniformName, const int &bindingPoint)
{
    //    unsigned int iLoc = glGetUniformBlockIndex(GetProgramID(), uniformName.c_str());
    //    glUniformBlockBinding(GetProgramID(), iLoc, bindingPoint);
    int iLoc = glGetUniformBlockIndex(m_uiProgram, uniformName.c_str());
    glUniformBlockBinding(m_uiProgram, iLoc, bindingPoint);
}


// Setting floats

void CShaderProgram::SetUniform(std::string sName, float* fValues, int iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform1fv(iLoc, iCount, fValues);
}

void CShaderProgram::SetUniform(std::string sName, const float fValue)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

void CShaderProgram::SetUniform(std::string sName, glm::vec2* vVectors, int iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
    
}


void CShaderProgram::SetUniform(std::string sName, const glm::vec2 vVector)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

void CShaderProgram::SetUniform(std::string sName, glm::vec3* vVectors, int iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

void CShaderProgram::SetUniform(std::string sName, const glm::vec3 vVector)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

void CShaderProgram::SetUniform(std::string sName, glm::vec4* vVectors, int iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}

void CShaderProgram::SetUniform(std::string sName, const glm::vec4 vVector)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// Setting 3x3 matrices

void CShaderProgram::SetUniform(std::string sName, glm::mat3* mMatrices, int iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniformMatrix3fv(iLoc, iCount, false, (GLfloat*)mMatrices);
}

void CShaderProgram::SetUniform(std::string sName, const glm::mat3 mMatrix)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniformMatrix3fv(iLoc, 1, false, (GLfloat*)&mMatrix);
}

// Setting 4x4 matrices

void CShaderProgram::SetUniform(std::string sName, glm::mat4* mMatrices, int iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniformMatrix4fv(iLoc, iCount, false, (GLfloat*)mMatrices);
}

void CShaderProgram::SetUniform(std::string sName, const glm::mat4 mMatrix)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniformMatrix4fv(iLoc, 1, false, (GLfloat*)&mMatrix);
}

// Setting integers

void CShaderProgram::SetUniform(std::string sName, int* iValues, int iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform1iv(iLoc, iCount, iValues);
}

void CShaderProgram::SetUniform(std::string sName, const int iValue)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform1i(iLoc, iValue);
}
