
#include "Shaders.h"


CShader::CShader()
{
    m_bLoaded = false;
}
CShader::~CShader()
{
    Release();
}

// Loads a shader, stored as a text file with filename sFile.  The shader is of type iType (vertex, fragment, geometry, etc.)
bool CShader::LoadShader(std::string sFile, int iType)
{
    std::vector<std::string> sLines;
    
    if(!GetLinesFromFile(sFile, false, &sLines)) {
        char message[1024];
        printf(message, "Cannot load shader\n%s\n", sFile.c_str());
        return false;
    }
    
    const char** sProgram = new const char*[(int)sLines.size()];
    for (int i = 0; i < (int)sLines.size(); i++)
        sProgram[i] = sLines[i].c_str();
    
    m_uiShader = glCreateShader(iType);
    
    glShaderSource(m_uiShader, (int)sLines.size(), sProgram, NULL);
    glCompileShader(m_uiShader);
    
    delete[] sProgram;
    
    int iCompilationStatus;
    glGetShaderiv(m_uiShader, GL_COMPILE_STATUS, &iCompilationStatus);
    
    if(iCompilationStatus == GL_FALSE)
    {
        char sInfoLog[1024];
        char sFinalMessage[1536];
        int iLogLength;
        glGetShaderInfoLog(m_uiShader, 1024, &iLogLength, sInfoLog);
        char sShaderType[64];
        if (iType == GL_VERTEX_SHADER)
            sprintf(sShaderType, "vertex shader");
        else if (iType == GL_FRAGMENT_SHADER)
            sprintf(sShaderType, "fragment shader");
        else if (iType == GL_GEOMETRY_SHADER)
            sprintf(sShaderType, "geometry shader");
        else if (iType == GL_TESS_CONTROL_SHADER)
            sprintf(sShaderType, "tesselation control shader");
        else if (iType == GL_TESS_EVALUATION_SHADER)
            sprintf(sShaderType, "tesselation evaluation shader");
        else
            sprintf(sShaderType, "unknown shader type");
        
        sprintf(sFinalMessage, "Error in %s!\n%s\nShader file not compiled.  The compiler returned:\n\n%s", sShaderType, sFile.c_str(), sInfoLog);
        
        return false;
    }
    m_iType = iType;
    m_bLoaded = true;
    
    return true;
}


// Loads a file into a vector of strings (vResult)
bool CShader::GetLinesFromFile(std::string sFile, bool bIncludePart, std::vector<std::string>* vResult)
{
    FILE* fp = fopen(sFile.c_str(), "rt");
    if(!fp){
        printf("Image could not be opened\n");
        return false;
    }
    
    std::string sDirectory;
    int slashIndex = -1;
    
    for (int i = (int)sFile.size()-1; i == 0; i--)
    {
        if(sFile[i] == '\\' || sFile[i] == '/')
        {
            slashIndex = i;
            break;
        }
    }
    
    sDirectory = sFile.substr(0, slashIndex+1);
    
    // Get all lines from a file
    
    char sLine[255];
    
    bool bInIncludePart = false;
    
    while(fgets(sLine, 255, fp))
    {
        std::stringstream ss(sLine);
        std::string sFirst;
        ss >> sFirst;
        if(sFirst == "#include")
        {
            std::string sFileName;
            ss >> sFileName;
            if((int)sFileName.size() > 0 && sFileName[0] == '\"' && sFileName[(int)sFileName.size()-1] == '\"')
            {
                sFileName = sFileName.substr(1, (int)sFileName.size()-2);
                GetLinesFromFile(sDirectory+sFileName, true, vResult);
            }
        }
        else if(sFirst == "#include_part")
            bInIncludePart = true;
        else if(sFirst == "#definition_part")
            bInIncludePart = false;
        else if(!bIncludePart || (bIncludePart && bInIncludePart))
            vResult->push_back(sLine);
    }
    fclose(fp);
    
    return true;
}


// Returns true if the shader was loaded and compiled
bool CShader::IsLoaded()
{
    return m_bLoaded;
}

// Returns the ID of the shader
GLuint CShader::GetShaderID()
{
    return m_uiShader;
}

// Deletes the shader and frees GPU memory
void CShader::DeleteShader()
{
    if(!IsLoaded())
        return;
    m_bLoaded = false;
    glDeleteShader(m_uiShader);
}

void CShader::Release() {
    
}
