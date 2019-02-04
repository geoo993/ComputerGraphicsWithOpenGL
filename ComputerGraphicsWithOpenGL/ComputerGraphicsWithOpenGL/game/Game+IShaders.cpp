//
//  Game+ShaderPrograms.cpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 24/07/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Game.h"

void Game::LoadShaderPrograms(const std::string &path) {
    
    // Load shaders
    std::vector<CShader> shShaders;
    std::vector<std::string> sShaderFileNames;
    sShaderFileNames.push_back("HudShader.vert");//0
    sShaderFileNames.push_back("HudShader.frag");
    sShaderFileNames.push_back("SkyBoxShader.vert");//1
    sShaderFileNames.push_back("SkyBoxShader.frag");
    sShaderFileNames.push_back("TerrainShader.vert");//2
    sShaderFileNames.push_back("TerrainShader.frag");
    sShaderFileNames.push_back("PhysicallyBasedRenderingShader.vert");//3
    sShaderFileNames.push_back("PhysicallyBasedRenderingShader.frag");
    sShaderFileNames.push_back("LampShader.vert");//4
    sShaderFileNames.push_back("LampShader.frag");
    sShaderFileNames.push_back("LightShader.vert");//5
    sShaderFileNames.push_back("LightShader.frag");
    
    for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
        std::string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
        int iShaderType;
        if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
        else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
        else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
        else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
        else iShaderType = GL_TESS_EVALUATION_SHADER;
        
        CShader shader;
        shader.LoadShader(path+"/shaders/"+sShaderFileNames[i], iShaderType);
        //cout << shader.IsLoaded() << " " << shader.GetShaderID() << endl;
        shShaders.push_back(shader);
    }
    
    // Create a shader program for fonts
    CShaderProgram *pFontProgram = new CShaderProgram;
    pFontProgram->CreateProgram();
    pFontProgram->AddShaderToProgram(&shShaders[0]);
    pFontProgram->AddShaderToProgram(&shShaders[1]);
    pFontProgram->LinkProgram();
    m_pShaderPrograms->push_back(pFontProgram);
    
    // Create a skyboxshader program
    CShaderProgram *pSkyboxProgram = new CShaderProgram;
    pSkyboxProgram->CreateProgram();
    pSkyboxProgram->AddShaderToProgram(&shShaders[2]);
    pSkyboxProgram->AddShaderToProgram(&shShaders[3]);
    pSkyboxProgram->LinkProgram();
    m_pShaderPrograms->push_back(pSkyboxProgram);
    
    // Create the terrain shader program
    CShaderProgram *pTerrainProgram = new CShaderProgram;
    pTerrainProgram->CreateProgram();
    pTerrainProgram->AddShaderToProgram(&shShaders[4]);
    pTerrainProgram->AddShaderToProgram(&shShaders[5]);
    pTerrainProgram->LinkProgram();
    m_pShaderPrograms->push_back(pTerrainProgram);
    
    // Create the Physically Based Rendering shader program
    CShaderProgram *pPBRProgram = new CShaderProgram;
    pPBRProgram->CreateProgram();
    pPBRProgram->AddShaderToProgram(&shShaders[6]);
    pPBRProgram->AddShaderToProgram(&shShaders[7]);
    pPBRProgram->LinkProgram();
    m_pShaderPrograms->push_back(pPBRProgram);
    
    // Create the Lamp shader program
    CShaderProgram *pLampProgram = new CShaderProgram;
    pLampProgram->CreateProgram();
    pLampProgram->AddShaderToProgram(&shShaders[8]);
    pLampProgram->AddShaderToProgram(&shShaders[9]);
    pLampProgram->LinkProgram();
    m_pShaderPrograms->push_back(pLampProgram);
    
    // Create the Light shader program
    CShaderProgram *pLightProgram = new CShaderProgram;
    pLightProgram->CreateProgram();
    pLightProgram->AddShaderToProgram(&shShaders[10]);
    pLightProgram->AddShaderToProgram(&shShaders[11]);
    pLightProgram->LinkProgram();
    m_pShaderPrograms->push_back(pLightProgram);
 
}



