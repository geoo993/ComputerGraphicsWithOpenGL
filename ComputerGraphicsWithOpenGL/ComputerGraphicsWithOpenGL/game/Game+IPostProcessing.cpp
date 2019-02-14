//
//  Game+PostProcessing.cpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 18/09/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Game.h"

/// initialise frame buffer elements
void Game::InitialiseFrameBuffers(const GLuint &width , const GLuint &height) {
    // post processing
    m_currentPPFXMode = PostProcessingEffectMode::DefaultFrameBuffer;
    m_coverage = 1.0f;
    
    m_pFBOs.push_back(new CFrameBufferObject);
    m_pFBOs.push_back(new CFrameBufferObject);
    m_pFBOs.push_back(new CFrameBufferObject);
    
    LoadFrameBuffers(width, height);
}

/// create frame buffers
void Game::LoadFrameBuffers(const GLuint &width , const GLuint &height) {

    m_pFBOs[0]->CreateFramebuffer(width, height, FrameBufferType::Default);
    m_pFBOs[1]->CreateFramebuffer(width, height, FrameBufferType::HighDynamicRangeMultipleRenderTargets);
    m_pFBOs[2]->CreateFramebuffer(width, height, FrameBufferType::PingPongRendering);
}

/// actvate frame buffer and stop rendering to the default framebuffer
void Game::ActivateFBO(const FrameBufferType &type) {
    
    // start by deleting the current fbos when the ppfx mode is changed and then we render again
    if (m_changePPFXMode == true) {
        
        const GLint width = m_gameWindow.GetWidth();
        const GLint height = m_gameWindow.GetHeight();
        
        for (GLuint i = 0; i < m_pFBOs.size(); i++) {
            m_pFBOs[i]->Release();
        }
        
        currentFBO->Release();
        
        LoadFrameBuffers(width, height);
        
        m_changePPFXMode = false;
    }
    
    switch(type) {
        case FrameBufferType::Default:
            currentFBO = m_pFBOs[0];
            // //bind to framebuffer and draw scene as we normally would to color texture
            //binding the fbo as render target stops rendering to the default framebuffer and you'll see that your screen turns black because the scene is no longer rendered to the default framebuffer. all rendering operations will store their result in the attachments of the newly created framebuffer.
            currentFBO->Bind();
            break;
        case FrameBufferType::MultiSampling: break;
        case FrameBufferType::DirectionalShadowMapping: break;
        case FrameBufferType::PointShadowMapping: break;
        case FrameBufferType::HighDynamicRangeMapping: break;
        case FrameBufferType::HighDynamicRangeMultipleRenderTargets:
            currentFBO = m_pFBOs[1];
            break;
        case FrameBufferType::PingPongRendering:
            currentFBO = m_pFBOs[2];
        case FrameBufferType::DeferredRendering: break;
        case FrameBufferType::SSAORendering: break;
        case FrameBufferType::SSAOProcessing: break;
        case FrameBufferType::SSAOBlur: break;
        default: break;
    }

}

/// render to post processing effect scene
void Game::RenderPPFXScene(const PostProcessingEffectMode &mode) {
    
    switch(mode) {
        case PostProcessingEffectMode::DefaultFrameBuffer: {
            
            CShaderProgram *pImageProcessingProgram = (*m_pShaderPrograms)[15];
            RenderToScreen(pImageProcessingProgram, true);
            return;
        }
        case PostProcessingEffectMode::ColorInversion: {
            
            CShaderProgram *pColorInversionProgram = (*m_pShaderPrograms)[16];
            SetColorInversionUniform(pColorInversionProgram);
            RenderToScreen(pColorInversionProgram, true);
            return;
        }
        case PostProcessingEffectMode::GrayScale: {
            
            CShaderProgram *pGrayScaleProgram = (*m_pShaderPrograms)[17];
            SetGrayScaleUniform(pGrayScaleProgram);
            RenderToScreen(pGrayScaleProgram, true);
            return;
        }
        case PostProcessingEffectMode::Kernel: {
            CShaderProgram *pKernelProgram = (*m_pShaderPrograms)[18];
            SetKernelUniform(pKernelProgram);
            RenderToScreen(pKernelProgram, true);
            return;
        }
        case PostProcessingEffectMode::KernelBlur: {
            CShaderProgram *pKernelBlurProgram = (*m_pShaderPrograms)[19];
            SetKernelBlurUniform(pKernelBlurProgram);
            RenderToScreen(pKernelBlurProgram, true);
            return;
        }
        case PostProcessingEffectMode::EdgeDetection: {
            CShaderProgram *pEdgeDetectionProgram = (*m_pShaderPrograms)[20];
            SetEdgeDetectionUniform(pEdgeDetectionProgram);
            RenderToScreen(pEdgeDetectionProgram, true);
            return;
        }
        case PostProcessingEffectMode::SobelEdgeDetection: {
            CShaderProgram *pSobelEdgeDetectionProgram = (*m_pShaderPrograms)[21];
            SetEdgeDetectionUniform(pSobelEdgeDetectionProgram);
            RenderToScreen(pSobelEdgeDetectionProgram, true);
            return;
        }
        case PostProcessingEffectMode::FreiChenEdgeDetection: {
            CShaderProgram *pFreiChenEdgeDetectionProgram = (*m_pShaderPrograms)[22];
            SetEdgeDetectionUniform(pFreiChenEdgeDetectionProgram);
            RenderToScreen(pFreiChenEdgeDetectionProgram, true);
            return;
        }
        case PostProcessingEffectMode::ScreenWave: {
            CShaderProgram *pScreenWaveProgram = (*m_pShaderPrograms)[23];
            SetScreenWaveUniform(pScreenWaveProgram);
            RenderToScreen(pScreenWaveProgram, true);
            return;
        }
        case PostProcessingEffectMode::Swirl: {
            CShaderProgram *pSwirlProgram = (*m_pShaderPrograms)[24];
            SetSwirlUniform(pSwirlProgram);
            RenderToScreen(pSwirlProgram, true);
            return;
        }
        case PostProcessingEffectMode::NightVision: {
            CShaderProgram *pNightVisionProgram = (*m_pShaderPrograms)[25];
            SetNightVisionUniform(pNightVisionProgram);
            RenderToScreen(pNightVisionProgram, true);
            return;
        }
        case PostProcessingEffectMode::LensCircle: {
            CShaderProgram *pLensCircleProgram = (*m_pShaderPrograms)[26];
            SetLensCircleUniform(pLensCircleProgram);
            RenderToScreen(pLensCircleProgram, true);
            return;
        }
        case PostProcessingEffectMode::Posterization: {
            CShaderProgram *pPosterizationProgram = (*m_pShaderPrograms)[27];
            SetPosterizationUniform(pPosterizationProgram);
            RenderToScreen(pPosterizationProgram, true);
            return;
        }
        case PostProcessingEffectMode::DreamVision: {
            CShaderProgram *pDreamVisionProgram = (*m_pShaderPrograms)[28];
            SetDreamVisionUniform(pDreamVisionProgram);
            RenderToScreen(pDreamVisionProgram, true);
            return;
        }
        case PostProcessingEffectMode::Pixelation: {
            CShaderProgram *pPixelationProgram = (*m_pShaderPrograms)[29];
            SetPixelationUniform(pPixelationProgram);
            RenderToScreen(pPixelationProgram, true);
            return;
        }
        case PostProcessingEffectMode::FrostedGlassEffect: {
            CShaderProgram *pFrostedGlassProgram = (*m_pShaderPrograms)[30];
            SetFrostedGlassEffectUniform(pFrostedGlassProgram);
            RenderToScreen(pFrostedGlassProgram, true);
            return;
        }
        case PostProcessingEffectMode::FrostedGlass: {
            CShaderProgram *pFrostedGlassExtraProgram = (*m_pShaderPrograms)[31];
            SetFrostedGlassUniform(pFrostedGlassExtraProgram);
            RenderToScreen(pFrostedGlassExtraProgram, true);
            return;
        }
        case PostProcessingEffectMode::Crosshatching: {
            CShaderProgram *pCrosshatchingProgram = (*m_pShaderPrograms)[32];
            SetCrosshatchingUniform(pCrosshatchingProgram);
            RenderToScreen(pCrosshatchingProgram, true);
            return;
        }
        case PostProcessingEffectMode::PredatorsThermalVision: {
            CShaderProgram *pPredatorsThermalVisionProgram = (*m_pShaderPrograms)[33];
            SetPredatorsThermalVisionUniform(pPredatorsThermalVisionProgram);
            RenderToScreen(pPredatorsThermalVisionProgram, true);
            return;
        }
        case PostProcessingEffectMode::Toonify: {
            CShaderProgram *pToonifyProgram = (*m_pShaderPrograms)[34];
            SetToonifyUniform(pToonifyProgram);
            RenderToScreen(pToonifyProgram, true);
            return;
        }
        case PostProcessingEffectMode::Shockwave: {
            CShaderProgram *pShockwaveProgram = (*m_pShaderPrograms)[35];
            SetShockwaveUniform(pShockwaveProgram);
            RenderToScreen(pShockwaveProgram, true);
            return;
        }
        case PostProcessingEffectMode::FishEye: {
            CShaderProgram *pFishEyeProgram = (*m_pShaderPrograms)[36];
            SetFishEyeUniform(pFishEyeProgram);
            RenderToScreen(pFishEyeProgram, true);
            return;
        }
        case PostProcessingEffectMode::BarrelDistortion: {
            CShaderProgram *pBarrelDistortionProgram = (*m_pShaderPrograms)[37];
            SetBarrelDistortionUniform(pBarrelDistortionProgram);
            RenderToScreen(pBarrelDistortionProgram, true);
            return;
        }
        case PostProcessingEffectMode::MultiScreenFishEye: {
            CShaderProgram *pMultiScreenFishEyeProgram = (*m_pShaderPrograms)[38];
            SetMultiScreenFishEyeUniform(pMultiScreenFishEyeProgram);
            RenderToScreen(pMultiScreenFishEyeProgram, true);
            return;
        }
        case PostProcessingEffectMode::FishEyeLens: {
            CShaderProgram *pFishEyeLensProgram = (*m_pShaderPrograms)[39];
            SetFishEyeLensUniform(pFishEyeLensProgram);
            RenderToScreen(pFishEyeLensProgram, true);
            return;
        }
        case PostProcessingEffectMode::FishEyeAntiFishEye: {
            CShaderProgram *pFishEyeAntiFishEyeProgram = (*m_pShaderPrograms)[40];
            SetFishEyeAntiFishEyeUniform(pFishEyeAntiFishEyeProgram);
            RenderToScreen(pFishEyeAntiFishEyeProgram, true);
            return;
        }
        case PostProcessingEffectMode::GaussianBlur: {
            CShaderProgram *pGaussianBlurProgram = (*m_pShaderPrograms)[41];
            pGaussianBlurProgram->UseProgram();
            
             bool horizontal = true, first_iteration = true;
             int amount = 2;
             for (GLuint i = 0; i < amount; i++)
             {
                 if (!first_iteration) {
                     m_pFBOs[2]->BindPingPong(horizontal);
                 }
                 
                 SetGaussianBlurUniform(pGaussianBlurProgram, true);
                 //RenderToScreen(pGaussianBlurProgram, true);
                 SetMaterialUniform(pGaussianBlurProgram, "material", glm::vec3(1.0f, 1.0f, 0.0f));
                 SetImageProcessingUniform(pGaussianBlurProgram, true);
                 
                 if (first_iteration) {
                     m_pFBOs[0]->BindTexture(0);
                 } else {
                     m_pFBOs[2]->BindPingPongTexture(!horizontal, 0);
                 }
             
                 RenderQuad(pGaussianBlurProgram);
                 
                 horizontal = !horizontal;
                 if (first_iteration) first_iteration = false;
             }
            //glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return;
        }
        case PostProcessingEffectMode::Blur: {
            CShaderProgram *pBlurProgram = (*m_pShaderPrograms)[42];
            SetBlurUniform(pBlurProgram);
            RenderToScreen(pBlurProgram, true);
            return;
        }
        case PostProcessingEffectMode::RadialBlur: {
            CShaderProgram *pRadialBlurProgram = (*m_pShaderPrograms)[43];
            SetRadialBlurUniform(pRadialBlurProgram);
            RenderToScreen(pRadialBlurProgram, true);
            return;
        }
        case PostProcessingEffectMode::MotionBlur: {
            CShaderProgram *pMotionBlurProgram = (*m_pShaderPrograms)[44];
            SetMotionBlurUniform(pMotionBlurProgram);
            RenderToScreen(pMotionBlurProgram, true);
            return;
        }
        case PostProcessingEffectMode::Vignetting: {
            CShaderProgram *pVignettingProgram = (*m_pShaderPrograms)[45];
            SetVignettingUniform(pVignettingProgram);
            RenderToScreen(pVignettingProgram, true);
            return;
        }
        case PostProcessingEffectMode::BrightParts: {
            CShaderProgram *pBrightPartsProgram = (*m_pShaderPrograms)[46];
            SetBrightPartsUniform(pBrightPartsProgram);
            RenderToScreen(pBrightPartsProgram, true);
            return;
        }
        case PostProcessingEffectMode::Bloom: {
            CShaderProgram *pBloomProgram = (*m_pShaderPrograms)[47];
            SetBloomUniform(pBloomProgram);
            RenderToScreen(pBloomProgram, true);
            return;
        }
        case PostProcessingEffectMode::LensFlare: {
            CShaderProgram *pLensFlareProgram = (*m_pShaderPrograms)[48];
            SetLensFlareUniform(pLensFlareProgram);
            RenderToScreen(pLensFlareProgram, true);
            return;
        }
        case PostProcessingEffectMode::FXAA: {
            
            return;
        }
        case PostProcessingEffectMode::SSAO: {
            
            return;
        }
        default: return;
        
    }
    
}

/// render to screen with frame buffer
void Game::RenderToScreen(CShaderProgram *pShaderProgram, const bool & useQuad) {
    
    currentFBO->BindTexture(0);
    
    SetMaterialUniform(pShaderProgram, "material", glm::vec3(1.0f, 1.0f, 0.0f));
    SetImageProcessingUniform(pShaderProgram, true);
    RenderQuad(pShaderProgram);
}

// Render scene method runs
void Game::RenderPPFX(const PostProcessingEffectMode &mode)
{
    //  Post Processing Effects
    // render the result on the default frame buffer using a full screen quad with post proccessing effects
    switch(mode) {
        case PostProcessingEffectMode::DefaultFrameBuffer:
            RenderPPFXScene(PostProcessingEffectMode::DefaultFrameBuffer);
            break;
        case PostProcessingEffectMode::ColorInversion:
            RenderPPFXScene(PostProcessingEffectMode::ColorInversion);
            break;
        case PostProcessingEffectMode::GrayScale:
            RenderPPFXScene(PostProcessingEffectMode::GrayScale);
            break;
        case PostProcessingEffectMode::Kernel:
            RenderPPFXScene(PostProcessingEffectMode::Kernel);
            break;
        case PostProcessingEffectMode::KernelBlur:
            RenderPPFXScene(PostProcessingEffectMode::KernelBlur);
            break;
        case PostProcessingEffectMode::EdgeDetection:
            RenderPPFXScene(PostProcessingEffectMode::EdgeDetection);
            break;
        case PostProcessingEffectMode::SobelEdgeDetection:
            RenderPPFXScene(PostProcessingEffectMode::SobelEdgeDetection);
            break;
        case PostProcessingEffectMode::FreiChenEdgeDetection:
            RenderPPFXScene(PostProcessingEffectMode::FreiChenEdgeDetection);
            break;
        case PostProcessingEffectMode::ScreenWave:
            RenderPPFXScene(PostProcessingEffectMode::ScreenWave);
            break;
        case PostProcessingEffectMode::Swirl:
            RenderPPFXScene(PostProcessingEffectMode::Swirl);
            break;
        case PostProcessingEffectMode::NightVision:
            RenderPPFXScene(PostProcessingEffectMode::NightVision);
            break;
        case PostProcessingEffectMode::LensCircle:
            RenderPPFXScene(PostProcessingEffectMode::LensCircle);
            break;
        case PostProcessingEffectMode::Posterization:
            RenderPPFXScene(PostProcessingEffectMode::Posterization);
            break;
        case PostProcessingEffectMode::DreamVision:
            RenderPPFXScene(PostProcessingEffectMode::DreamVision);
            break;
        case PostProcessingEffectMode::Pixelation:
            RenderPPFXScene(PostProcessingEffectMode::Pixelation);
            break;
        case PostProcessingEffectMode::FrostedGlassEffect:
            RenderPPFXScene(PostProcessingEffectMode::FrostedGlassEffect);
            break;
        case PostProcessingEffectMode::FrostedGlass:
            RenderPPFXScene(PostProcessingEffectMode::FrostedGlass);
            break;
        case PostProcessingEffectMode::Crosshatching:
            RenderPPFXScene(PostProcessingEffectMode::Crosshatching);
            break;
        case PostProcessingEffectMode::PredatorsThermalVision:
            RenderPPFXScene(PostProcessingEffectMode::PredatorsThermalVision);
            break;
        case PostProcessingEffectMode::Toonify:
            RenderPPFXScene(PostProcessingEffectMode::Toonify);
            break;
        case PostProcessingEffectMode::Shockwave:
            RenderPPFXScene(PostProcessingEffectMode::Shockwave);
            break;
        case PostProcessingEffectMode::FishEye:
            RenderPPFXScene(PostProcessingEffectMode::FishEye);
            break;
        case PostProcessingEffectMode::BarrelDistortion:
            RenderPPFXScene(PostProcessingEffectMode::BarrelDistortion);
            break;
        case PostProcessingEffectMode::MultiScreenFishEye:
            RenderPPFXScene(PostProcessingEffectMode::MultiScreenFishEye);
            break;
        case PostProcessingEffectMode::FishEyeLens:
            RenderPPFXScene(PostProcessingEffectMode::FishEyeLens);
            break;
        case PostProcessingEffectMode::FishEyeAntiFishEye:
            RenderPPFXScene(PostProcessingEffectMode::FishEyeAntiFishEye);
            break;
        case PostProcessingEffectMode::GaussianBlur:
            RenderPPFXScene(PostProcessingEffectMode::GaussianBlur);
            break;
        case PostProcessingEffectMode::Blur:
            RenderPPFXScene(PostProcessingEffectMode::Blur);
            break;
        case PostProcessingEffectMode::RadialBlur:
            RenderPPFXScene(PostProcessingEffectMode::RadialBlur);
            break;
        case PostProcessingEffectMode::MotionBlur:
            RenderPPFXScene(PostProcessingEffectMode::MotionBlur);
            break;
        case PostProcessingEffectMode::Vignetting:
            RenderPPFXScene(PostProcessingEffectMode::Vignetting);
            break;
        case PostProcessingEffectMode::BrightParts:
            RenderPPFXScene(PostProcessingEffectMode::BrightParts);
            break;
        case PostProcessingEffectMode::Bloom:
            RenderPPFXScene(PostProcessingEffectMode::Bloom);
            break;
        case PostProcessingEffectMode::LensFlare:
            RenderPPFXScene(PostProcessingEffectMode::LensFlare);
            break;
        case PostProcessingEffectMode::FXAA:
            RenderPPFXScene(PostProcessingEffectMode::FXAA);
            break;
        case PostProcessingEffectMode::SSAO:
            RenderPPFXScene(PostProcessingEffectMode::SSAO);
            break;
    }
    
}

/// convert post processing effect to string
const char * const Game::PostProcessingEffectToString(const PostProcessingEffectMode &mode){
    
    //  Post Processing Effects
    // render the result on the default frame buffer using a full screen quad with post proccessing effects
    switch(mode) {
        case PostProcessingEffectMode::DefaultFrameBuffer:
        return "Default FrameBuffer";
        case PostProcessingEffectMode::ColorInversion:
        return "Color Inversion";
        case PostProcessingEffectMode::GrayScale:
        return "GrayScale";
        case PostProcessingEffectMode::Kernel:
        return "Kernel";
        case PostProcessingEffectMode::KernelBlur:
        return "Kernel Blur";
        case PostProcessingEffectMode::EdgeDetection:
        return "Edge Detection";
        case PostProcessingEffectMode::SobelEdgeDetection:
            return "Sobel Edge Detection";
        case PostProcessingEffectMode::FreiChenEdgeDetection:
            return "Frei-Chen Edge Detection";
        case PostProcessingEffectMode::ScreenWave:
        return "Screen Wave";
        case PostProcessingEffectMode::Swirl:
        return "Swirl";
        case PostProcessingEffectMode::NightVision:
        return "Night Vision";
        case PostProcessingEffectMode::LensCircle:
        return "Lens Circle";
        case PostProcessingEffectMode::Posterization:
        return "Posterization";
        case PostProcessingEffectMode::DreamVision:
        return "Dream Vision";
        case PostProcessingEffectMode::Pixelation:
        return "Pixelation";
        case PostProcessingEffectMode::FrostedGlassEffect:
        return "Frosted Glass Effect";
        case PostProcessingEffectMode::FrostedGlass:
        return "Frosted Glass";
        case PostProcessingEffectMode::Crosshatching:
        return "Crosshatching";
        case PostProcessingEffectMode::PredatorsThermalVision:
        return "Predators Thermal Vision";
        case PostProcessingEffectMode::Toonify:
        return "Toonify";
        case PostProcessingEffectMode::Shockwave:
        return "Shockwave";
        case PostProcessingEffectMode::FishEye:
        return "Fish Eye";
        case PostProcessingEffectMode::BarrelDistortion:
        return "Barrel Distortion";
        case PostProcessingEffectMode::MultiScreenFishEye:
        return "MultiScreen Fish Eye";
        case PostProcessingEffectMode::FishEyeLens:
        return "Fish Eye Lens";
        case PostProcessingEffectMode::FishEyeAntiFishEye:
        return "Fish Eye / Anti-Fish Eye";
        case PostProcessingEffectMode::GaussianBlur:
        return "Gaussian Blur";
        case PostProcessingEffectMode::Blur:
        return "Blur";
        case PostProcessingEffectMode::RadialBlur:
        return "Radial Blur";
        case PostProcessingEffectMode::MotionBlur:
        return "Motion Blur";
        case PostProcessingEffectMode::Vignetting:
        return "Vignetting";
        case PostProcessingEffectMode::BrightParts:
        return "Bright Parts";
        case PostProcessingEffectMode::Bloom:
        return "Bloom";
        case PostProcessingEffectMode::LensFlare:
        return "Lens Flare";
        case PostProcessingEffectMode::FXAA:
        return "FXAA";
        case PostProcessingEffectMode::SSAO:
        return "Screen Space Ambient Occlusion";
        default :
        return "";
    }
    
}

