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
    sShaderFileNames.push_back("NormalMappingShader.vert");//6
    sShaderFileNames.push_back("NormalMappingShader.frag");
    sShaderFileNames.push_back("BumpMappingShader.vert");//7
    sShaderFileNames.push_back("BumpMappingShader.frag");
    sShaderFileNames.push_back("ParallaxNormalMappingShader.vert");//8
    sShaderFileNames.push_back("ParallaxNormalMappingShader.frag");
    sShaderFileNames.push_back("EnvironmentMappingShader.vert");//9
    sShaderFileNames.push_back("EnvironmentMappingShader.frag");
    sShaderFileNames.push_back("ChromaticAberrationShader.vert");//10
    sShaderFileNames.push_back("ChromaticAberrationShader.frag");
    sShaderFileNames.push_back("ExplosionShader.vert");//11
    sShaderFileNames.push_back("ExplosionShader.geom");
    sShaderFileNames.push_back("ExplosionShader.frag");
    sShaderFileNames.push_back("PorcupineRenderingShader.vert");//12
    sShaderFileNames.push_back("PorcupineRenderingShader.geom");
    sShaderFileNames.push_back("PorcupineRenderingShader.frag");
    sShaderFileNames.push_back("WireFrameShader.vert");//13
    sShaderFileNames.push_back("WireFrameShader.geom");
    sShaderFileNames.push_back("WireFrameShader.frag");
    sShaderFileNames.push_back("ToonShader.vert");//14
    sShaderFileNames.push_back("ToonShader.frag");
    sShaderFileNames.push_back("ImageProcessingShader.vert");// 15
    sShaderFileNames.push_back("ImageProcessingShader.frag");
    sShaderFileNames.push_back("ColorInversionShader.vert");// 16
    sShaderFileNames.push_back("ColorInversionShader.frag");
    sShaderFileNames.push_back("GrayScaleShader.vert");// 17
    sShaderFileNames.push_back("GrayScaleShader.frag");
    sShaderFileNames.push_back("KernelShader.vert");// 18
    sShaderFileNames.push_back("KernelShader.frag");
    sShaderFileNames.push_back("KernelBlurShader.vert");// 19
    sShaderFileNames.push_back("KernelBlurShader.frag");
    sShaderFileNames.push_back("EdgeDetectionShader.vert");// 20
    sShaderFileNames.push_back("EdgeDetectionShader.frag");
    
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
    
    // Create the Normal mapping shader program
    CShaderProgram *pNormalMapProgram = new CShaderProgram;
    pNormalMapProgram->CreateProgram();
    pNormalMapProgram->AddShaderToProgram(&shShaders[12]);
    pNormalMapProgram->AddShaderToProgram(&shShaders[13]);
    pNormalMapProgram->LinkProgram();
    m_pShaderPrograms->push_back(pNormalMapProgram);
    
    // Create the Bump mapping shader program
    CShaderProgram *pBumpMapProgram = new CShaderProgram;
    pBumpMapProgram->CreateProgram();
    pBumpMapProgram->AddShaderToProgram(&shShaders[14]);
    pBumpMapProgram->AddShaderToProgram(&shShaders[15]);
    pBumpMapProgram->LinkProgram();
    m_pShaderPrograms->push_back(pBumpMapProgram);
    
    // Create the Parallax Normal mapping shader program
    CShaderProgram *pParallaxNormalMappingProgram = new CShaderProgram;
    pParallaxNormalMappingProgram->CreateProgram();
    pParallaxNormalMappingProgram->AddShaderToProgram(&shShaders[16]);
    pParallaxNormalMappingProgram->AddShaderToProgram(&shShaders[17]);
    pParallaxNormalMappingProgram->LinkProgram();
    m_pShaderPrograms->push_back(pParallaxNormalMappingProgram);
 
    // Create the Environment mapping shader program
    CShaderProgram *pEnvironmentMapProgram = new CShaderProgram;
    pEnvironmentMapProgram->CreateProgram();
    pEnvironmentMapProgram->AddShaderToProgram(&shShaders[18]);
    pEnvironmentMapProgram->AddShaderToProgram(&shShaders[19]);
    pEnvironmentMapProgram->LinkProgram();
    m_pShaderPrograms->push_back(pEnvironmentMapProgram);
    
    // Create the Chromatic Aberration mapping shader program
    CShaderProgram *pChromaticAberrationMapProgram = new CShaderProgram;
    pChromaticAberrationMapProgram->CreateProgram();
    pChromaticAberrationMapProgram->AddShaderToProgram(&shShaders[20]);
    pChromaticAberrationMapProgram->AddShaderToProgram(&shShaders[21]);
    pChromaticAberrationMapProgram->LinkProgram();
    m_pShaderPrograms->push_back(pChromaticAberrationMapProgram);
    
    // Create the Explosion shader program
    CShaderProgram *pExplosionShader = new CShaderProgram;
    pExplosionShader->CreateProgram();
    pExplosionShader->AddShaderToProgram(&shShaders[22]);
    pExplosionShader->AddShaderToProgram(&shShaders[23]);
    pExplosionShader->AddShaderToProgram(&shShaders[24]);
    pExplosionShader->LinkProgram();
    m_pShaderPrograms->push_back(pExplosionShader);
    
    // Create the Porcupine Rendering shader program
    CShaderProgram *pPorcupineRenderingShader = new CShaderProgram;
    pPorcupineRenderingShader->CreateProgram();
    pPorcupineRenderingShader->AddShaderToProgram(&shShaders[25]);
    pPorcupineRenderingShader->AddShaderToProgram(&shShaders[26]);
    pPorcupineRenderingShader->AddShaderToProgram(&shShaders[27]);
    pPorcupineRenderingShader->LinkProgram();
    m_pShaderPrograms->push_back(pPorcupineRenderingShader);
    
    // Create the Wireframe shader program
    CShaderProgram *pWireFrameShader = new CShaderProgram;
    pWireFrameShader->CreateProgram();
    pWireFrameShader->AddShaderToProgram(&shShaders[28]);
    pWireFrameShader->AddShaderToProgram(&shShaders[29]);
    pWireFrameShader->AddShaderToProgram(&shShaders[30]);
    pWireFrameShader->LinkProgram();
    m_pShaderPrograms->push_back(pWireFrameShader);
    
    // Create the Toon shader program
    CShaderProgram *pToonShader = new CShaderProgram;
    pToonShader->CreateProgram();
    pToonShader->AddShaderToProgram(&shShaders[31]);
    pToonShader->AddShaderToProgram(&shShaders[32]);
    pToonShader->LinkProgram();
    m_pShaderPrograms->push_back(pToonShader);
    
    // Create the Image Processing shader program
    CShaderProgram *pImageProcessingShader = new CShaderProgram;
    pImageProcessingShader->CreateProgram();
    pImageProcessingShader->AddShaderToProgram(&shShaders[33]);
    pImageProcessingShader->AddShaderToProgram(&shShaders[34]);
    pImageProcessingShader->LinkProgram();
    m_pShaderPrograms->push_back(pImageProcessingShader);
    
    // Create the Color Inversion shader program
    CShaderProgram *pColorInversionShader = new CShaderProgram;
    pColorInversionShader->CreateProgram();
    pColorInversionShader->AddShaderToProgram(&shShaders[35]);
    pColorInversionShader->AddShaderToProgram(&shShaders[36]);
    pColorInversionShader->LinkProgram();
    m_pShaderPrograms->push_back(pColorInversionShader);
    
    // Create the Gray Scale shader program
    CShaderProgram *pGrayScaleShader = new CShaderProgram;
    pGrayScaleShader->CreateProgram();
    pGrayScaleShader->AddShaderToProgram(&shShaders[37]);
    pGrayScaleShader->AddShaderToProgram(&shShaders[38]);
    pGrayScaleShader->LinkProgram();
    m_pShaderPrograms->push_back(pGrayScaleShader);
    
    // Create the Kernel Shader shader program
    CShaderProgram *pKernelShader = new CShaderProgram;
    pKernelShader->CreateProgram();
    pKernelShader->AddShaderToProgram(&shShaders[39]);
    pKernelShader->AddShaderToProgram(&shShaders[40]);
    pKernelShader->LinkProgram();
    m_pShaderPrograms->push_back(pKernelShader);
    
    // Create the Kernel Blur Shader shader program
    CShaderProgram *pKernelBlurShader = new CShaderProgram;
    pKernelBlurShader->CreateProgram();
    pKernelBlurShader->AddShaderToProgram(&shShaders[41]);
    pKernelBlurShader->AddShaderToProgram(&shShaders[42]);
    pKernelBlurShader->LinkProgram();
    m_pShaderPrograms->push_back(pKernelBlurShader);
    
    // Create the Edge Detection Shader shader program
    CShaderProgram *pEdgeDetectionShader = new CShaderProgram;
    pEdgeDetectionShader->CreateProgram();
    pEdgeDetectionShader->AddShaderToProgram(&shShaders[43]);
    pEdgeDetectionShader->AddShaderToProgram(&shShaders[44]);
    pEdgeDetectionShader->LinkProgram();
    m_pShaderPrograms->push_back(pEdgeDetectionShader);
}



