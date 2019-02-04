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
{
    Release();
}

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

// A collection of functions to set uniform variables inside shaders

// Setting Uniform Buffer Objects

void CShaderProgram::SetUniformBlock(std::string uniformName, const GLint &bindingPoint)
{
    //    unsigned int iLoc = glGetUniformBlockIndex(GetProgramID(), uniformName.c_str());
    //    glUniformBlockBinding(GetProgramID(), iLoc, bindingPoint);
    int iLoc = glGetUniformBlockIndex(m_uiProgram, uniformName.c_str());
    glUniformBlockBinding(m_uiProgram, iLoc, bindingPoint);
}


// Setting floats

void CShaderProgram::SetUniform(std::string sName, GLfloat * fValues, const GLint & iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform1fv(iLoc, iCount, fValues);
}

void CShaderProgram::SetUniform(std::string sName, const GLfloat &fValue)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

void CShaderProgram::SetUniform(std::string sName, glm::vec2* vVectors, const GLint & iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
}

void CShaderProgram::SetUniform(std::string sName, const glm::vec2 vVector)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

void CShaderProgram::SetUniform(std::string sName, glm::vec3* vVectors, const GLint & iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

void CShaderProgram::SetUniform(std::string sName, const glm::vec3 vVector)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

void CShaderProgram::SetUniform(std::string sName, glm::vec4* vVectors, const GLint & iCount)
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

void CShaderProgram::SetUniform(std::string sName, glm::mat3* mMatrices, const GLint & iCount)
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

void CShaderProgram::SetUniform(std::string sName, glm::mat4* mMatrices, const GLint & iCount)
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

void CShaderProgram::SetUniform(std::string sName, GLint * iValues, const GLint & iCount)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform1iv(iLoc, iCount, iValues);
}

void CShaderProgram::SetUniform(std::string sName, const GLint &iValue)
{
    int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
    glUniform1i(iLoc, iValue);
}

void CShaderProgram::Release() {
    
}
