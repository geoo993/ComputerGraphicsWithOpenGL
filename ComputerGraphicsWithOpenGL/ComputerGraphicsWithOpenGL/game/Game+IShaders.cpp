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
    sShaderFileNames.push_back("SobelEdgeDetectionShader.vert");// 21
    sShaderFileNames.push_back("SobelEdgeDetectionShader.frag");
    sShaderFileNames.push_back("FreiChenEdgeDetectionShader.vert");// 22
    sShaderFileNames.push_back("FreiChenEdgeDetectionShader.frag");
    sShaderFileNames.push_back("ScreenWaveShader.vert");// 23
    sShaderFileNames.push_back("ScreenWaveShader.frag");
    sShaderFileNames.push_back("SwirlShader.vert");// 24
    sShaderFileNames.push_back("SwirlShader.frag");
    sShaderFileNames.push_back("NightVisionLensShader.vert");// 25
    sShaderFileNames.push_back("NightVisionLensShader.frag");
    sShaderFileNames.push_back("LensCircleShader.vert");// 26
    sShaderFileNames.push_back("LensCircleShader.frag");
    sShaderFileNames.push_back("PosterizationShader.vert");// 27
    sShaderFileNames.push_back("PosterizationShader.frag");
    sShaderFileNames.push_back("DreamVisionShader.vert");// 28
    sShaderFileNames.push_back("DreamVisionShader.frag");
    sShaderFileNames.push_back("PixelationShader.vert");// 29
    sShaderFileNames.push_back("PixelationShader.frag");
    sShaderFileNames.push_back("FrostedGlassEffectShader.vert");// 30
    sShaderFileNames.push_back("FrostedGlassEffectShader.frag");
    sShaderFileNames.push_back("FrostedGlassShader.vert");// 31
    sShaderFileNames.push_back("FrostedGlassShader.frag");
    sShaderFileNames.push_back("CrosshatchingShader.vert");// 32
    sShaderFileNames.push_back("CrosshatchingShader.frag");
    sShaderFileNames.push_back("PredatorsThermalVisionShader.vert");// 33
    sShaderFileNames.push_back("PredatorsThermalVisionShader.frag");
    sShaderFileNames.push_back("ToonifyShader.vert");// 34
    sShaderFileNames.push_back("ToonifyShader.frag");
    sShaderFileNames.push_back("ShockwaveShader.vert");// 35
    sShaderFileNames.push_back("ShockwaveShader.frag");
    sShaderFileNames.push_back("FishEyeShader.vert");// 36
    sShaderFileNames.push_back("FishEyeShader.frag");
    sShaderFileNames.push_back("BarrelDistortionPixelShader.vert");// 37
    sShaderFileNames.push_back("BarrelDistortionPixelShader.frag");
    sShaderFileNames.push_back("MultiScreenFishEyeShader.vert");// 38
    sShaderFileNames.push_back("MultiScreenFishEyeShader.frag");
    sShaderFileNames.push_back("FishEyeLensShader.vert");// 39
    sShaderFileNames.push_back("FishEyeLensShader.frag");
    sShaderFileNames.push_back("FishEyeAntiFishEyeShader.vert");// 40
    sShaderFileNames.push_back("FishEyeAntiFishEyeShader.frag");
    sShaderFileNames.push_back("GaussianBlurShader.vert");// 41
    sShaderFileNames.push_back("GaussianBlurShader.frag");
    sShaderFileNames.push_back("BlurShader.vert");// 42
    sShaderFileNames.push_back("BlurShader.frag");
    sShaderFileNames.push_back("RadialBlurShader.vert");// 43
    sShaderFileNames.push_back("RadialBlurShader.frag");
    sShaderFileNames.push_back("MotionBlurShader.vert");// 44
    sShaderFileNames.push_back("MotionBlurShader.frag");
    sShaderFileNames.push_back("VignettingShader.vert");// 45
    sShaderFileNames.push_back("VignettingShader.frag");
    sShaderFileNames.push_back("BrightPartsShader.vert");// 46
    sShaderFileNames.push_back("BrightPartsShader.frag");
    sShaderFileNames.push_back("BloomShader.vert");// 47
    sShaderFileNames.push_back("BloomShader.frag");
    sShaderFileNames.push_back("LensFlareShader.vert");// 48
    sShaderFileNames.push_back("LensFlareShader.frag");
    sShaderFileNames.push_back("DepthMappingShader.vert");// 49
    sShaderFileNames.push_back("DepthMappingShader.frag");
    sShaderFileNames.push_back("LightSpaceShader.vert");// 50
    sShaderFileNames.push_back("LightSpaceShader.frag");
    sShaderFileNames.push_back("ToneMappingShader.vert");// 51
    sShaderFileNames.push_back("ToneMappingShader.frag");
    
    
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
    
    // Create the Sobel Edge Detection Shader shader program
    CShaderProgram *pSobelEdgeDetectionShader = new CShaderProgram;
    pSobelEdgeDetectionShader->CreateProgram();
    pSobelEdgeDetectionShader->AddShaderToProgram(&shShaders[45]);
    pSobelEdgeDetectionShader->AddShaderToProgram(&shShaders[46]);
    pSobelEdgeDetectionShader->LinkProgram();
    m_pShaderPrograms->push_back(pSobelEdgeDetectionShader);
    
    // Create the Frei-Chen Edge Detection Shader shader program
    CShaderProgram *pFreiChenEdgeDetectionShader = new CShaderProgram;
    pFreiChenEdgeDetectionShader->CreateProgram();
    pFreiChenEdgeDetectionShader->AddShaderToProgram(&shShaders[47]);
    pFreiChenEdgeDetectionShader->AddShaderToProgram(&shShaders[48]);
    pFreiChenEdgeDetectionShader->LinkProgram();
    m_pShaderPrograms->push_back(pEdgeDetectionShader);
    
    // Create the Screen Wave shader program
    CShaderProgram *pScreenWaveShader = new CShaderProgram;
    pScreenWaveShader->CreateProgram();
    pScreenWaveShader->AddShaderToProgram(&shShaders[49]);
    pScreenWaveShader->AddShaderToProgram(&shShaders[50]);
    pScreenWaveShader->LinkProgram();
    m_pShaderPrograms->push_back(pScreenWaveShader);
    
    // Create the Swirl shader program
    CShaderProgram *pSwirlShader = new CShaderProgram;
    pSwirlShader->CreateProgram();
    pSwirlShader->AddShaderToProgram(&shShaders[51]);
    pSwirlShader->AddShaderToProgram(&shShaders[52]);
    pSwirlShader->LinkProgram();
    m_pShaderPrograms->push_back(pSwirlShader);
    
    // Create the Night Vision Lens program
    CShaderProgram *pNightVisionLensShader = new CShaderProgram;
    pNightVisionLensShader->CreateProgram();
    pNightVisionLensShader->AddShaderToProgram(&shShaders[53]);
    pNightVisionLensShader->AddShaderToProgram(&shShaders[54]);
    pNightVisionLensShader->LinkProgram();
    m_pShaderPrograms->push_back(pNightVisionLensShader);
    
    // Create the Lens Circle shader program
    CShaderProgram *pLensCircleShader = new CShaderProgram;
    pLensCircleShader->CreateProgram();
    pLensCircleShader->AddShaderToProgram(&shShaders[55]);
    pLensCircleShader->AddShaderToProgram(&shShaders[56]);
    pLensCircleShader->LinkProgram();
    m_pShaderPrograms->push_back(pLensCircleShader);
    
    // Create the Posterization shader program
    CShaderProgram *pPosterizationShader = new CShaderProgram;
    pPosterizationShader->CreateProgram();
    pPosterizationShader->AddShaderToProgram(&shShaders[57]);
    pPosterizationShader->AddShaderToProgram(&shShaders[58]);
    pPosterizationShader->LinkProgram();
    m_pShaderPrograms->push_back(pPosterizationShader);
    
    // Create the Dream Vision shader program
    CShaderProgram *pDreamVisionShader = new CShaderProgram;
    pDreamVisionShader->CreateProgram();
    pDreamVisionShader->AddShaderToProgram(&shShaders[59]);
    pDreamVisionShader->AddShaderToProgram(&shShaders[60]);
    pDreamVisionShader->LinkProgram();
    m_pShaderPrograms->push_back(pDreamVisionShader);
    
    // Create the Pixelation shader program
    CShaderProgram *pPixelationShader = new CShaderProgram;
    pPixelationShader->CreateProgram();
    pPixelationShader->AddShaderToProgram(&shShaders[61]);
    pPixelationShader->AddShaderToProgram(&shShaders[62]);
    pPixelationShader->LinkProgram();
    m_pShaderPrograms->push_back(pPixelationShader);
    
    // Create the Frosted Glass Effect shader program
    CShaderProgram *pFrostedGlassEffectShader = new CShaderProgram;
    pFrostedGlassEffectShader->CreateProgram();
    pFrostedGlassEffectShader->AddShaderToProgram(&shShaders[63]);
    pFrostedGlassEffectShader->AddShaderToProgram(&shShaders[64]);
    pFrostedGlassEffectShader->LinkProgram();
    m_pShaderPrograms->push_back(pFrostedGlassEffectShader);
    
    // Create the Frosted Glass shader program
    CShaderProgram *pFrostedGlassShader = new CShaderProgram;
    pFrostedGlassShader->CreateProgram();
    pFrostedGlassShader->AddShaderToProgram(&shShaders[65]);
    pFrostedGlassShader->AddShaderToProgram(&shShaders[66]);
    pFrostedGlassShader->LinkProgram();
    m_pShaderPrograms->push_back(pFrostedGlassShader);
    
    // Create the Crosshatching shader program
    CShaderProgram *pCrosshatchingShader = new CShaderProgram;
    pCrosshatchingShader->CreateProgram();
    pCrosshatchingShader->AddShaderToProgram(&shShaders[67]);
    pCrosshatchingShader->AddShaderToProgram(&shShaders[68]);
    pCrosshatchingShader->LinkProgram();
    m_pShaderPrograms->push_back(pCrosshatchingShader);
    
    // Create the Predators Thermal Vision shader program
    CShaderProgram *pPredatorsThermalVisionShader = new CShaderProgram;
    pPredatorsThermalVisionShader->CreateProgram();
    pPredatorsThermalVisionShader->AddShaderToProgram(&shShaders[69]);
    pPredatorsThermalVisionShader->AddShaderToProgram(&shShaders[70]);
    pPredatorsThermalVisionShader->LinkProgram();
    m_pShaderPrograms->push_back(pPredatorsThermalVisionShader);
    
    // Create the Toonify shader program
    CShaderProgram *pToonifyShader = new CShaderProgram;
    pToonifyShader->CreateProgram();
    pToonifyShader->AddShaderToProgram(&shShaders[71]);
    pToonifyShader->AddShaderToProgram(&shShaders[72]);
    pToonifyShader->LinkProgram();
    m_pShaderPrograms->push_back(pToonifyShader);
    
    // Create the Shockwave shader program
    CShaderProgram *pShockwaveShader = new CShaderProgram;
    pShockwaveShader->CreateProgram();
    pShockwaveShader->AddShaderToProgram(&shShaders[73]);
    pShockwaveShader->AddShaderToProgram(&shShaders[74]);
    pShockwaveShader->LinkProgram();
    m_pShaderPrograms->push_back(pShockwaveShader);

    // Create the Fish Eye shader program
    CShaderProgram *pFishEyeShader = new CShaderProgram;
    pFishEyeShader->CreateProgram();
    pFishEyeShader->AddShaderToProgram(&shShaders[75]);
    pFishEyeShader->AddShaderToProgram(&shShaders[76]);
    pFishEyeShader->LinkProgram();
    m_pShaderPrograms->push_back(pFishEyeShader);
    
    // Create the Barrel Distortion Pixel shader program
    CShaderProgram *pBarrelDistortionPixelShader = new CShaderProgram;
    pBarrelDistortionPixelShader->CreateProgram();
    pBarrelDistortionPixelShader->AddShaderToProgram(&shShaders[77]);
    pBarrelDistortionPixelShader->AddShaderToProgram(&shShaders[78]);
    pBarrelDistortionPixelShader->LinkProgram();
    m_pShaderPrograms->push_back(pBarrelDistortionPixelShader);
    
    // Create the Multi Screen Fish Eye shader program
    CShaderProgram *pMultiScreenFishEyeShader = new CShaderProgram;
    pMultiScreenFishEyeShader->CreateProgram();
    pMultiScreenFishEyeShader->AddShaderToProgram(&shShaders[79]);
    pMultiScreenFishEyeShader->AddShaderToProgram(&shShaders[80]);
    pMultiScreenFishEyeShader->LinkProgram();
    m_pShaderPrograms->push_back(pMultiScreenFishEyeShader);
    
    // Create the Fish Eye Lens shader program
    CShaderProgram *pFishEyeLensShader = new CShaderProgram;
    pFishEyeLensShader->CreateProgram();
    pFishEyeLensShader->AddShaderToProgram(&shShaders[81]);
    pFishEyeLensShader->AddShaderToProgram(&shShaders[82]);
    pFishEyeLensShader->LinkProgram();
    m_pShaderPrograms->push_back(pFishEyeLensShader);
    
    // Create the Multi Screen Fish Eye shader program
    CShaderProgram *pFishEyeAntiFishEyeShader = new CShaderProgram;
    pFishEyeAntiFishEyeShader->CreateProgram();
    pFishEyeAntiFishEyeShader->AddShaderToProgram(&shShaders[83]);
    pFishEyeAntiFishEyeShader->AddShaderToProgram(&shShaders[84]);
    pFishEyeAntiFishEyeShader->LinkProgram();
    m_pShaderPrograms->push_back(pFishEyeAntiFishEyeShader);
    
    // Create the Gaussian Blur shader program
    CShaderProgram *pGaussianBlurShader = new CShaderProgram;
    pGaussianBlurShader->CreateProgram();
    pGaussianBlurShader->AddShaderToProgram(&shShaders[85]);
    pGaussianBlurShader->AddShaderToProgram(&shShaders[86]);
    pGaussianBlurShader->LinkProgram();
    m_pShaderPrograms->push_back(pGaussianBlurShader);
    
    // Create the Blur shader program
    CShaderProgram *pBlurShader = new CShaderProgram;
    pBlurShader->CreateProgram();
    pBlurShader->AddShaderToProgram(&shShaders[87]);
    pBlurShader->AddShaderToProgram(&shShaders[88]);
    pBlurShader->LinkProgram();
    m_pShaderPrograms->push_back(pBlurShader);
    
    // Create the Radial Blur shader program
    CShaderProgram *pRadialBlurShader = new CShaderProgram;
    pRadialBlurShader->CreateProgram();
    pRadialBlurShader->AddShaderToProgram(&shShaders[89]);
    pRadialBlurShader->AddShaderToProgram(&shShaders[90]);
    pRadialBlurShader->LinkProgram();
    m_pShaderPrograms->push_back(pRadialBlurShader);
    
    // Create the Motion Blur shader program
    CShaderProgram *pMotionBlurShader = new CShaderProgram;
    pMotionBlurShader->CreateProgram();
    pMotionBlurShader->AddShaderToProgram(&shShaders[91]);
    pMotionBlurShader->AddShaderToProgram(&shShaders[92]);
    pMotionBlurShader->LinkProgram();
    m_pShaderPrograms->push_back(pMotionBlurShader);
    
    // Create the Vignetting shader program
    CShaderProgram *pVignettingShader = new CShaderProgram;
    pVignettingShader->CreateProgram();
    pVignettingShader->AddShaderToProgram(&shShaders[93]);
    pVignettingShader->AddShaderToProgram(&shShaders[94]);
    pVignettingShader->LinkProgram();
    m_pShaderPrograms->push_back(pVignettingShader);
    
    // Create the Bright Parts shader program
    CShaderProgram *pBrightPartsShader = new CShaderProgram;
    pBrightPartsShader->CreateProgram();
    pBrightPartsShader->AddShaderToProgram(&shShaders[95]);
    pBrightPartsShader->AddShaderToProgram(&shShaders[96]);
    pBrightPartsShader->LinkProgram();
    m_pShaderPrograms->push_back(pBrightPartsShader);
    
    // Create the Bloom shader program
    CShaderProgram *pBloomShader = new CShaderProgram;
    pBloomShader->CreateProgram();
    pBloomShader->AddShaderToProgram(&shShaders[97]);
    pBloomShader->AddShaderToProgram(&shShaders[98]);
    pBloomShader->LinkProgram();
    m_pShaderPrograms->push_back(pBloomShader);
    
    // Create the lens Flare shader program
    CShaderProgram *pLensFlareShader = new CShaderProgram;
    pLensFlareShader->CreateProgram();
    pLensFlareShader->AddShaderToProgram(&shShaders[99]);
    pLensFlareShader->AddShaderToProgram(&shShaders[100]);
    pLensFlareShader->LinkProgram();
    m_pShaderPrograms->push_back(pLensFlareShader);
    
    // Create the Depth Mapping shader program
    CShaderProgram *pDepthMappingShader = new CShaderProgram;
    pDepthMappingShader->CreateProgram();
    pDepthMappingShader->AddShaderToProgram(&shShaders[101]);
    pDepthMappingShader->AddShaderToProgram(&shShaders[102]);
    pDepthMappingShader->LinkProgram();
    m_pShaderPrograms->push_back(pDepthMappingShader);
    
    // Create the Light Space shader program
    CShaderProgram *pLightSpaceShader = new CShaderProgram;
    pLightSpaceShader->CreateProgram();
    pLightSpaceShader->AddShaderToProgram(&shShaders[103]);
    pLightSpaceShader->AddShaderToProgram(&shShaders[104]);
    pLightSpaceShader->LinkProgram();
    m_pShaderPrograms->push_back(pLightSpaceShader);
    
    // Create the Tone Mapping shader program
    CShaderProgram *pToneMappingShader = new CShaderProgram;
    pToneMappingShader->CreateProgram();
    pToneMappingShader->AddShaderToProgram(&shShaders[105]);
    pToneMappingShader->AddShaderToProgram(&shShaders[106]);
    pToneMappingShader->LinkProgram();
    m_pShaderPrograms->push_back(pToneMappingShader);
}



