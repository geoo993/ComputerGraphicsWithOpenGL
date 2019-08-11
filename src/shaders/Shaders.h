#pragma once

#include "../ShadersBase.h"

// A class that provides a wrapper around an OpenGL shader
class CShader
{
public:
	CShader();
    ~CShader();

	bool LoadShader(std::string sFile, int iType);
	void DeleteShader();

	bool GetLinesFromFile(std::string sFile, bool bIncludePart, std::vector<std::string>* vResult);

	bool IsLoaded();
	uint GetShaderID();

    void Release();
private:
	uint m_uiShader; // ID of shader
	int m_iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool m_bLoaded; // Whether shader was loaded and compiled
};
