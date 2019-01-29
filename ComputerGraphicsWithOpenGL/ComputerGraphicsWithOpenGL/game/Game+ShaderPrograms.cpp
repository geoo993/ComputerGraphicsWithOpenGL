//
//  Game+ShaderPrograms.cpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 24/07/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Game.h"


void Game::CreateShaderPrograms(const std::string &path){
    
    // Load shaders
    std::vector<CShader> shShaders;
    std::vector<std::string> sShaderFileNames;
    sShaderFileNames.push_back("HudShader.vert");//0
    sShaderFileNames.push_back("HudShader.frag");
    sShaderFileNames.push_back("SkyBoxShader.vert");//1
    sShaderFileNames.push_back("SkyBoxShader.frag");
    sShaderFileNames.push_back("BasicShader.vert");//2
    sShaderFileNames.push_back("BasicShader.frag");
    
    
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
    
    // Create a shader program for fonts
    CShaderProgram *pSkyboxProgram = new CShaderProgram;
    pSkyboxProgram->CreateProgram();
    pSkyboxProgram->AddShaderToProgram(&shShaders[2]);
    pSkyboxProgram->AddShaderToProgram(&shShaders[3]);
    pSkyboxProgram->LinkProgram();
    m_pShaderPrograms->push_back(pSkyboxProgram);
    
    // Create the phong shader program
    CShaderProgram *pBasicProgram = new CShaderProgram;
    pBasicProgram->CreateProgram();
    pBasicProgram->AddShaderToProgram(&shShaders[4]);
    pBasicProgram->AddShaderToProgram(&shShaders[5]);
    pBasicProgram->LinkProgram();
    m_pShaderPrograms->push_back(pBasicProgram);
 
}



