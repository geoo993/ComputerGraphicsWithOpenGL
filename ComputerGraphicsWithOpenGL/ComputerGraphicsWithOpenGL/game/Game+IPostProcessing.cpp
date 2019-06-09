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
    m_currentPPFXMode = PostProcessingEffectMode::PBR;
    m_coverage = 1.0f;
    
    m_pFBOs.push_back(new CFrameBufferObject);
    m_pFBOs.push_back(new CFrameBufferObject);
    m_pFBOs.push_back(new CFrameBufferObject);
    m_pFBOs.push_back(new CFrameBufferObject);
    m_pFBOs.push_back(new CFrameBufferObject);
    m_pFBOs.push_back(new CFrameBufferObject);
    m_pFBOs.push_back(new CFrameBufferObject);
    LoadFrameBuffers(width, height);
}

/// create frame buffers
void Game::LoadFrameBuffers(const GLuint &width , const GLuint &height) {

    m_pFBOs[0]->CreateFramebuffer(width, height, FrameBufferType::Default);
    m_pFBOs[1]->CreateFramebuffer(width, height, FrameBufferType::GeometryBuffer);
    m_pFBOs[2]->CreateFramebuffer(width, height, FrameBufferType::PingPongRendering);
    m_pFBOs[3]->CreateFramebuffer(width, height, FrameBufferType::DepthMapping);
    m_pFBOs[4]->CreateFramebuffer(width, height, FrameBufferType::Default);
    m_pFBOs[5]->CreateFramebuffer(width, height, FrameBufferType::SSAO);
    m_pFBOs[6]->CreateFramebuffer(width, height, FrameBufferType::SSAO);
}

/// actvate frame buffer and stop rendering to the default framebuffer
void Game::ActivateFBO(const PostProcessingEffectMode &mode) {
    
    // start by deleting the current fbos when the ppfx mode is changed and then we render again
    if (m_changePPFXMode == true) {
        
        const GLint width = m_gameWindow->GetWidth();
        const GLint height = m_gameWindow->GetHeight();
        
        for (GLuint i = 0; i < m_pFBOs.size(); i++) {
            m_pFBOs[i]->Release();
        }
        
        currentFBO->Release();
        
        LoadFrameBuffers(width, height);
        
        m_changePPFXMode = false;
    }
    
    FrameBufferType fboType = GetFBOtype(mode);
    
    switch(fboType) {
        case FrameBufferType::Default:
            currentFBO = m_pFBOs[0];
            // //bind to framebuffer and draw scene as we normally would to color texture
            //binding the fbo as render target stops rendering to the default framebuffer and you'll see that your screen turns black because the scene is no longer rendered to the default framebuffer. all rendering operations will store their result in the attachments of the newly created framebuffer.
            currentFBO->Bind(true);     // prepare frame buffer 0
            break;
        case FrameBufferType::GeometryBuffer:
            currentFBO = m_pFBOs[1];
            currentFBO->Bind(true);     // prepare frame buffer 1
            break;
        case FrameBufferType::PingPongRendering:break;
        case FrameBufferType::DepthMapping:
            currentFBO = m_pFBOs[3];
            currentFBO->Bind(true);     // prepare frame buffer 3
            break;
        default: break;
    }

}

/// render to post processing effect scene
void Game::RenderPPFXScene(const PostProcessingEffectMode &mode) {
    
    switch(mode) {
        case PostProcessingEffectMode::PBR: {
          
            CShaderProgram *pImageProcessingProgram = (*m_pShaderPrograms)[15];
            RenderToScreen(pImageProcessingProgram);
            
            return;
        }
        case PostProcessingEffectMode::IBL: {
            CShaderProgram *pImageProcessingProgram = (*m_pShaderPrograms)[15];
            RenderToScreen(pImageProcessingProgram);
            return;
        }
        case PostProcessingEffectMode::BlinnPhong: {
            CShaderProgram *pImageProcessingProgram = (*m_pShaderPrograms)[15];
            RenderToScreen(pImageProcessingProgram);
            return;
        }
        case PostProcessingEffectMode::ColorInversion: {
            
            CShaderProgram *pColorInversionProgram = (*m_pShaderPrograms)[16];
            SetColorInversionUniform(pColorInversionProgram);
            RenderToScreen(pColorInversionProgram);
            return;
        }
        case PostProcessingEffectMode::GrayScale: {
            
            CShaderProgram *pGrayScaleProgram = (*m_pShaderPrograms)[17];
            SetGrayScaleUniform(pGrayScaleProgram);
            RenderToScreen(pGrayScaleProgram);
            return;
        }
        case PostProcessingEffectMode::Kernel: {
            CShaderProgram *pKernelProgram = (*m_pShaderPrograms)[18];
            SetKernelUniform(pKernelProgram);
            RenderToScreen(pKernelProgram);
            return;
        }
        case PostProcessingEffectMode::KernelBlur: {
            CShaderProgram *pKernelBlurProgram = (*m_pShaderPrograms)[19];
            SetKernelBlurUniform(pKernelBlurProgram);
            RenderToScreen(pKernelBlurProgram);
            return;
        }
        case PostProcessingEffectMode::SobelEdgeDetection: {
            CShaderProgram *pSobelEdgeDetectionProgram = (*m_pShaderPrograms)[21];
            SetEdgeDetectionUniform(pSobelEdgeDetectionProgram);
            RenderToScreen(pSobelEdgeDetectionProgram);
            return;
        }
        case PostProcessingEffectMode::FreiChenEdgeDetection: {
            CShaderProgram *pFreiChenEdgeDetectionProgram = (*m_pShaderPrograms)[22];
            SetEdgeDetectionUniform(pFreiChenEdgeDetectionProgram);
            RenderToScreen(pFreiChenEdgeDetectionProgram);
            return;
        }
        case PostProcessingEffectMode::ScreenWave: {
            CShaderProgram *pScreenWaveProgram = (*m_pShaderPrograms)[23];
            SetScreenWaveUniform(pScreenWaveProgram);
            RenderToScreen(pScreenWaveProgram);
            return;
        }
        case PostProcessingEffectMode::Swirl: {
            CShaderProgram *pSwirlProgram = (*m_pShaderPrograms)[24];
            SetSwirlUniform(pSwirlProgram);
            RenderToScreen(pSwirlProgram);
            return;
        }
        case PostProcessingEffectMode::NightVision: {
            CShaderProgram *pNightVisionProgram = (*m_pShaderPrograms)[25];
            SetNightVisionUniform(pNightVisionProgram);
            RenderToScreen(pNightVisionProgram);
            return;
        }
        case PostProcessingEffectMode::LensCircle: {
            CShaderProgram *pLensCircleProgram = (*m_pShaderPrograms)[26];
            SetLensCircleUniform(pLensCircleProgram);
            RenderToScreen(pLensCircleProgram);
            return;
        }
        case PostProcessingEffectMode::Posterization: {
            CShaderProgram *pPosterizationProgram = (*m_pShaderPrograms)[27];
            SetPosterizationUniform(pPosterizationProgram);
            RenderToScreen(pPosterizationProgram);
            return;
        }
        case PostProcessingEffectMode::DreamVision: {
            CShaderProgram *pDreamVisionProgram = (*m_pShaderPrograms)[28];
            SetDreamVisionUniform(pDreamVisionProgram);
            RenderToScreen(pDreamVisionProgram);
            return;
        }
        case PostProcessingEffectMode::Pixelate: {
            CShaderProgram *pPixelateProgram = (*m_pShaderPrograms)[64];
            SetPixelateUniform(pPixelateProgram);
            RenderToScreen(pPixelateProgram);
            return;
        }
        case PostProcessingEffectMode::Pixelation: {
            CShaderProgram *pPixelationProgram = (*m_pShaderPrograms)[29];
            SetPixelationUniform(pPixelationProgram);
            RenderToScreen(pPixelationProgram);
            return;
        }
        case PostProcessingEffectMode::KnittedPixelation: {
            CShaderProgram *pKnittedPixelationProgram = (*m_pShaderPrograms)[69];
            SetKnittedPixelationUniform(pKnittedPixelationProgram);
            RenderToScreen(pKnittedPixelationProgram);
            return;
        }
        case PostProcessingEffectMode::FrostedGlassEffect: {
            CShaderProgram *pFrostedGlassProgram = (*m_pShaderPrograms)[30];
            SetFrostedGlassEffectUniform(pFrostedGlassProgram);
            RenderToScreen(pFrostedGlassProgram);
            return;
        }
        case PostProcessingEffectMode::FrostedGlass: {
            CShaderProgram *pFrostedGlassExtraProgram = (*m_pShaderPrograms)[31];
            SetFrostedGlassUniform(pFrostedGlassExtraProgram);
            RenderToScreen(pFrostedGlassExtraProgram);
            return;
        }
        case PostProcessingEffectMode::Crosshatching: {
            CShaderProgram *pCrosshatchingProgram = (*m_pShaderPrograms)[32];
            SetCrosshatchingUniform(pCrosshatchingProgram);
            RenderToScreen(pCrosshatchingProgram);
            return;
        }
        case PostProcessingEffectMode::PredatorsThermalVision: {
            CShaderProgram *pPredatorsThermalVisionProgram = (*m_pShaderPrograms)[33];
            SetPredatorsThermalVisionUniform(pPredatorsThermalVisionProgram);
            RenderToScreen(pPredatorsThermalVisionProgram);
            return;
        }
        case PostProcessingEffectMode::Toonify: {
            CShaderProgram *pToonifyProgram = (*m_pShaderPrograms)[34];
            SetToonifyUniform(pToonifyProgram);
            RenderToScreen(pToonifyProgram);
            return;
        }
        case PostProcessingEffectMode::Shockwave: {
            CShaderProgram *pShockwaveProgram = (*m_pShaderPrograms)[35];
            SetShockwaveUniform(pShockwaveProgram);
            RenderToScreen(pShockwaveProgram);
            return;
        }
        case PostProcessingEffectMode::FishEye: {
            CShaderProgram *pFishEyeProgram = (*m_pShaderPrograms)[36];
            SetFishEyeUniform(pFishEyeProgram);
            RenderToScreen(pFishEyeProgram);
            return;
        }
        case PostProcessingEffectMode::BarrelDistortion: {
            CShaderProgram *pBarrelDistortionProgram = (*m_pShaderPrograms)[37];
            SetBarrelDistortionUniform(pBarrelDistortionProgram);
            RenderToScreen(pBarrelDistortionProgram);
            return;
        }
        case PostProcessingEffectMode::MultiScreenFishEye: {
            CShaderProgram *pMultiScreenFishEyeProgram = (*m_pShaderPrograms)[38];
            SetMultiScreenFishEyeUniform(pMultiScreenFishEyeProgram);
            RenderToScreen(pMultiScreenFishEyeProgram);
            return;
        }
        case PostProcessingEffectMode::FishEyeLens: {
            CShaderProgram *pFishEyeLensProgram = (*m_pShaderPrograms)[39];
            SetFishEyeLensUniform(pFishEyeLensProgram);
            RenderToScreen(pFishEyeLensProgram);
            return;
        }
        case PostProcessingEffectMode::FishEyeAntiFishEye: {
            CShaderProgram *pFishEyeAntiFishEyeProgram = (*m_pShaderPrograms)[40];
            SetFishEyeAntiFishEyeUniform(pFishEyeAntiFishEyeProgram);
            RenderToScreen(pFishEyeAntiFishEyeProgram);
            return;
        }
        case PostProcessingEffectMode::GaussianBlur: {
            CShaderProgram *pGaussianBlurProgram = (*m_pShaderPrograms)[41];
            pGaussianBlurProgram->UseProgram();
            bool horizontal = true; // 0 is false aand 1 is true
            
            // Second Pass - BLUR in many iterations
            {
                bool first_iteration = true;
                int amount = 9; // number of times we blur
                for (GLuint i = 0; i < amount; i++)
                {
                    currentFBO = m_pFBOs[2];
                    currentFBO->BindPingPong(horizontal, true); // prepare ping pong frame buffer
                    
                    SetGaussianBlurUniform(pGaussianBlurProgram, horizontal);
                    
                    // blur textures that are in the depthMap texture unit
                    if (first_iteration) {
                        currentFBO = m_pFBOs[1];
                        RenderToScreen(pGaussianBlurProgram, FrameBufferType::GeometryBuffer, 0, TextureType::DEPTH);
                    } else {
                        currentFBO = m_pFBOs[2];
                        RenderToScreen(pGaussianBlurProgram, FrameBufferType::PingPongRendering, !horizontal, TextureType::DEPTH);
                    }
                    
                    horizontal = !horizontal;
                    if (first_iteration) first_iteration = false;
                }
            }
            ResetFrameBuffer();
            
            // Third Pass - Final Blur to screen
            {
                currentFBO = m_pFBOs[1];
                currentFBO->BindHDRTexture(0, static_cast<GLint>(TextureType::AMBIENT)); // bind the earlier (scene rendering) rendering from the hrd frame buffer
                
                // taking the blured texture and showing it after setviewport
                currentFBO = m_pFBOs[2];
                SetGaussianBlurUniform(pGaussianBlurProgram, horizontal);
                RenderToScreen(pGaussianBlurProgram, FrameBufferType::PingPongRendering, horizontal, TextureType::DEPTH);
            }
            return;
        }
        case PostProcessingEffectMode::Blur: {
            CShaderProgram *pBlurProgram = (*m_pShaderPrograms)[42];
            SetBlurUniform(pBlurProgram);
            RenderToScreen(pBlurProgram);
            return;
        }
        case PostProcessingEffectMode::RadialBlur: {
            CShaderProgram *pRadialBlurProgram = (*m_pShaderPrograms)[43];
            SetRadialBlurUniform(pRadialBlurProgram);
            RenderToScreen(pRadialBlurProgram);
            return;
        }
        case PostProcessingEffectMode::MotionBlur: {
            // Second Pass - Render Scene as usual
            {
                currentFBO = m_pFBOs[3];
                currentFBO->Bind(true); // prepare depth frame buffer (3)
                RenderScene(true);
            }
            
            ResetFrameBuffer();
            
            // Third Pass - Motion Blur
            {
                CShaderProgram *pMotionBlurProgram = (*m_pShaderPrograms)[44];
                SetMotionBlurUniform(pMotionBlurProgram);
                SetCameraUniform(pMotionBlurProgram, "camera", m_pCamera);
                
                // bind depth texture
                currentFBO = m_pFBOs[3];
                currentFBO->BindDepthTexture(static_cast<GLint>(TextureType::DEPTH));
                
                currentFBO = m_pFBOs[0];
                RenderToScreen(pMotionBlurProgram, FrameBufferType::Default, 0, TextureType::AMBIENT);
            }
            return;
        }
        case PostProcessingEffectMode::DepthMapping: {
            
            // Second Pass - Render Scene as usual
            {
                currentFBO = m_pFBOs[3];
                currentFBO->Bind(true); // prepare depth frame buffer (3)
                RenderScene(true);
            }
            
            ResetFrameBuffer();
            
            // Third Pass - DepthMapping
            {
                CShaderProgram *pDepthMappingProgram = (*m_pShaderPrograms)[50];
                SetDepthMappingUniform(pDepthMappingProgram);
                
                // bind depth texture
                currentFBO = m_pFBOs[3];
                currentFBO->BindDepthTexture(static_cast<GLint>(TextureType::DEPTH));
                
                currentFBO = m_pFBOs[0];
                RenderToScreen(pDepthMappingProgram, FrameBufferType::Default, 0, TextureType::AMBIENT);
            }
            return;
        }
        case PostProcessingEffectMode::Vignetting: {
            CShaderProgram *pVignettingProgram = (*m_pShaderPrograms)[45];
            SetVignettingUniform(pVignettingProgram);
            RenderToScreen(pVignettingProgram);
            return;
        }
        case PostProcessingEffectMode::BrightParts: {
            CShaderProgram *pBrightPartsProgram = (*m_pShaderPrograms)[46];
            SetBrightPartsUniform(pBrightPartsProgram);
            
            currentFBO = m_pFBOs[1];
            currentFBO->BindHDRTexture(0, static_cast<GLint>(TextureType::AMBIENT)); // bind the earlier (scene rendering) rendering from the hrd frame buff
            RenderToScreen(pBrightPartsProgram, FrameBufferType::GeometryBuffer, 1, TextureType::DEPTH);
            return;
        }
        case PostProcessingEffectMode::Bloom: {
            
            bool horizontal = true; // 0 is false aand 1 is true
            // Second Pass - BLUR Bright Parts
            {
                CShaderProgram *pGaussianBlurProgram = (*m_pShaderPrograms)[41];
                pGaussianBlurProgram->UseProgram();
                
                bool first_iteration = true;
                int amount = 10; // number of times we blur
                for (GLuint i = 0; i < amount; i++)
                {
                    currentFBO = m_pFBOs[2];
                    currentFBO->BindPingPong(horizontal, true); // prepare ping pong frame buffer
                    
                    SetGaussianBlurUniform(pGaussianBlurProgram, horizontal);
                    
                    // blur textures that are in the depthMap texture unit
                    if (first_iteration) {
                        currentFBO = m_pFBOs[1];
                        RenderToScreen(pGaussianBlurProgram, FrameBufferType::GeometryBuffer, 1, TextureType::DEPTH); // providing the bright parts textures at the first iteration
                    } else {
                        currentFBO = m_pFBOs[2];
                        RenderToScreen(pGaussianBlurProgram, FrameBufferType::PingPongRendering, !horizontal, TextureType::DEPTH);
                    }
                    horizontal = !horizontal;
                    if (first_iteration) first_iteration = false;
                }
            }
            
            ResetFrameBuffer();
            
            // Third Pass - BLOOM
            {
                CShaderProgram *pBloomProgram = (*m_pShaderPrograms)[47];
                SetBloomUniform(pBloomProgram);
                
                currentFBO = m_pFBOs[1];
                currentFBO->BindHDRTexture(0, static_cast<GLint>(TextureType::AMBIENT)); // bind the earlier (scene rendering) rendering from the hrd frame buffer
                
                currentFBO = m_pFBOs[2];
                RenderToScreen(pBloomProgram, FrameBufferType::PingPongRendering, !horizontal, TextureType::DEPTH);
            }
            return;
        }
        case PostProcessingEffectMode::HDRToneMapping: {
            CShaderProgram *pHDRToneMappingProgram = (*m_pShaderPrograms)[52];
            SetHRDToneMappingUniform(pHDRToneMappingProgram);
            RenderToScreen(pHDRToneMappingProgram, FrameBufferType::GeometryBuffer, 0, TextureType::AMBIENT);
            return;
        }
        case PostProcessingEffectMode::LensFlare: {
            /*
             The screen space technique comprises the following 4 steps:
             
             1. Downsample the scene image.     // Bright parts
             2. Blur.                           // Blur image
             3. Generate lens flare features.   // Lens flare
             4. Upsample/composite.             // Bring all together
             
             There are a couple of important considerations to make regarding the overall rendering pipeline:
             
             - Any post process motion blur or depth of field effect must be applied prior to combining the lens flare, so that the lens flare features don't participate in those effects. Technically the lens flare features would exhibit some motion blur, however it's incompatible with post process motion techniques. As a compromise, you could implement the lens flare using an accumulation buffer.
             - The lens flare should be applied before any tonemapping operation. This makes physical sense, as tonemapping simulates the reaction of the film/CMOS to the incoming light, of which the lens flare is a constituent part
             */
            bool horizontal = true; // 0 is false aand 1 is true
            // Second Pass - BLUR Bright Parts
            {
                CShaderProgram *pGaussianBlurProgram = (*m_pShaderPrograms)[41];
                pGaussianBlurProgram->UseProgram();
                
                bool first_iteration = true;
                int amount = 10; // number of times we blur
                for (GLuint i = 0; i < amount; i++)
                {
                    currentFBO = m_pFBOs[2];
                    currentFBO->BindPingPong(horizontal, true); // prepare ping pong frame buffer
                    
                    SetGaussianBlurUniform(pGaussianBlurProgram, horizontal);
                    
                    // blur textures that are in the depthMap texture unit
                    if (first_iteration) {
                        currentFBO = m_pFBOs[1];
                        RenderToScreen(pGaussianBlurProgram, FrameBufferType::GeometryBuffer, 1, TextureType::DEPTH); // providing the bright parts textures at the first iteration
                    } else {
                        currentFBO = m_pFBOs[2];
                        RenderToScreen(pGaussianBlurProgram, FrameBufferType::PingPongRendering, !horizontal, TextureType::DEPTH);
                    }
                    horizontal = !horizontal;
                    if (first_iteration) first_iteration = false;
                }
            }
            
            ResetFrameBuffer();
            
            // Third Pass - Flare Ghost
            {
                currentFBO = m_pFBOs[4];
                currentFBO->Bind(true); // prepare default frame buffer (5)
                
                CShaderProgram *pLensFlareGhostProgram = (*m_pShaderPrograms)[48];
                SetLensFlareGhostUniform(pLensFlareGhostProgram);
                
                currentFBO = m_pFBOs[2];
                RenderToScreen(pLensFlareGhostProgram, FrameBufferType::PingPongRendering, !horizontal, TextureType::DEPTH);
            }
            
            ResetFrameBuffer();
            
            // Forth Pass - Lens Flare Composition
            {
                CShaderProgram *pLensFlareProgram = (*m_pShaderPrograms)[49];
                SetLensFlareUniform(pLensFlareProgram);
                
                currentFBO = m_pFBOs[4];
                currentFBO->BindTexture(static_cast<GLint>(TextureType::LENS));
                
                currentFBO = m_pFBOs[1];
                RenderToScreen(pLensFlareProgram, FrameBufferType::GeometryBuffer, 0, TextureType::AMBIENT);
            }
            return;
        }
        case PostProcessingEffectMode::FXAA: {
            CShaderProgram *pFastApproximateAntiAliasingProgram = (*m_pShaderPrograms)[53];
            SetFastApproximateAntiAliasingUniform(pFastApproximateAntiAliasingProgram);
            RenderToScreen(pFastApproximateAntiAliasingProgram);
            return;
        }
        case PostProcessingEffectMode::DeferredRendering: {
            // Second Pass - Deferred Rendering
            {
                CShaderProgram *pDeferredRenderingProgram= (*m_pShaderPrograms)[55];
                SetDeferredRenderingUniform(pDeferredRenderingProgram, m_woodenBoxesUseTexture);
                
                // Render Lighting Scene
                SetCameraUniform(pDeferredRenderingProgram, "camera", m_pCamera);
                SetLightUniform(pDeferredRenderingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
                
                // Add Default Lights
                RenderLight(pDeferredRenderingProgram, m_pCamera);
                
                // Bind Textures
                currentFBO = m_pFBOs[1];
                currentFBO->BindPositionTexture(static_cast<GLint>(TextureType::DISPLACEMENT));
                currentFBO->BindNormalTexture(static_cast<GLint>(TextureType::NORMAL));
                currentFBO->BindAlbedoTexture(static_cast<GLint>(TextureType::DIFFUSE));
                
                RenderToScreen(pDeferredRenderingProgram, FrameBufferType::GeometryBuffer, 0, TextureType::AMBIENT);
                
            }
            
            // Blit to default frame buffer
            currentFBO->BlitToDepthBuffer(0);
            ResetFrameBuffer(false);
            
            {
                /// Render Lamps
                CShaderProgram *pLampProgram = (*m_pShaderPrograms)[4];
                for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
                    SetMaterialUniform(pLampProgram, "material", m_pointLightColors[i]);
                    RenderLamp(pLampProgram, m_pointLightPositions[i], 10.0f);
                }
            }
            
            return;
        }
        case PostProcessingEffectMode::SSAO: {
            /*
             In reality, light scatters in all kinds of directions with varying intensities so the indirectly lit parts of a scene should also have varying intensities, instead of a constant ambient component. One type of indirect lighting approximation is called ambient occlusion that tries to approximate indirect lighting by darkening creases, holes and surfaces that are close to each other. These areas are largely occluded by surrounding geometry and thus light rays have less places to escape, hence the areas appear darker.
             */
            
            // Second Pass - Screen Space Ambient Occlusion
            {
                
                currentFBO = m_pFBOs[5];
                currentFBO->Bind(true);     // prepare frame buffer 5
                
                CShaderProgram *pScreenSpaceAmbientOcclusionProgram= (*m_pShaderPrograms)[56];
                SetScreenSpaceAmbientOcclusionUniform(pScreenSpaceAmbientOcclusionProgram);
                
                // Bind Textures
                currentFBO = m_pFBOs[1];
                currentFBO->BindPositionTexture(static_cast<GLint>(TextureType::DISPLACEMENT));
                currentFBO->BindNormalTexture(static_cast<GLint>(TextureType::NORMAL));
                currentFBO->BindAlbedoTexture(static_cast<GLint>(TextureType::DIFFUSE));
            
                RenderToScreen(pScreenSpaceAmbientOcclusionProgram, FrameBufferType::GeometryBuffer, 0, TextureType::AMBIENT);
                
            }
            
            ResetFrameBuffer();
            
            // Third Pass - Screen Space Ambient Occlusion Blur
            {
                currentFBO = m_pFBOs[6];
                currentFBO->Bind(true);     // prepare frame buffer 5
                
                CShaderProgram *pScreenSpaceAmbientOcclusionBlurProgram= (*m_pShaderPrograms)[57];
                SetScreenSpaceAmbientOcclusionBlurUniform(pScreenSpaceAmbientOcclusionBlurProgram);
        
                currentFBO = m_pFBOs[5];
                RenderToScreen(pScreenSpaceAmbientOcclusionBlurProgram, FrameBufferType::SSAO, 0, TextureType::AO);
            }
            
            ResetFrameBuffer();
            
            // Fourth Pass - Screen Space Ambient Occlusion Lighting
            {
                CShaderProgram *pScreenSpaceAmbientOcclusionLightingProgram= (*m_pShaderPrograms)[58];
                SetScreenSpaceAmbientOcclusionLightingUniform(pScreenSpaceAmbientOcclusionLightingProgram, m_woodenBoxesUseTexture);
                
                // Render Lighting Scene
                SetCameraUniform(pScreenSpaceAmbientOcclusionLightingProgram, "camera", m_pCamera);
                SetLightUniform(pScreenSpaceAmbientOcclusionLightingProgram, m_useDir, m_usePoint, m_useSpot, m_useSmoothSpot, m_useBlinn);
                
                // Add Default Lights
                RenderLight(pScreenSpaceAmbientOcclusionLightingProgram, m_pCamera);
                
                currentFBO = m_pFBOs[5];
                currentFBO->BindTexture(static_cast<GLint>(TextureType::AO));
                
                currentFBO = m_pFBOs[1];
                currentFBO->BindPositionTexture(static_cast<GLint>(TextureType::DISPLACEMENT));
                currentFBO->BindNormalTexture(static_cast<GLint>(TextureType::NORMAL));
                currentFBO->BindAlbedoTexture(static_cast<GLint>(TextureType::DIFFUSE));
                RenderToScreen(pScreenSpaceAmbientOcclusionLightingProgram, FrameBufferType::GeometryBuffer, 0, TextureType::AMBIENT);
            }
            
            return;
        }
        case PostProcessingEffectMode::RainDrops: {
            CShaderProgram *pRainDropsProgram = (*m_pShaderPrograms)[59];
            SetRainDropsUniform(pRainDropsProgram);
            RenderToScreen(pRainDropsProgram);
            return;
        }
        case PostProcessingEffectMode::PaletteQuantizationAndDithering: {
            CShaderProgram *pPaletteQuantizationAndDitheringProgram = (*m_pShaderPrograms)[61];
            SetPaletteQuantizationAndDitheringUniform(pPaletteQuantizationAndDitheringProgram);
            RenderToScreen(pPaletteQuantizationAndDitheringProgram);
            return;
        }
        case PostProcessingEffectMode::DistortedTV: {
            CShaderProgram *pDistortedTVProgram = (*m_pShaderPrograms)[62];
            SetDistortedTVUniform(pDistortedTVProgram);
            RenderToScreen(pDistortedTVProgram);
            return;
        }
        case PostProcessingEffectMode::RGBDisplay: {
            CShaderProgram *pRGBDisplayProgram = (*m_pShaderPrograms)[63];
            SetRGBDisplayUniform(pRGBDisplayProgram);
            RenderToScreen(pRGBDisplayProgram);
            return;
        }
        case PostProcessingEffectMode::RetroParallax: {
            CShaderProgram *pRetroParallaxProgram = (*m_pShaderPrograms)[65];
            SetRetroParallaxUniform(pRetroParallaxProgram);
            RenderToScreen(pRetroParallaxProgram);
            return;
        }
        case PostProcessingEffectMode::ScaryRetroParallax: {
            CShaderProgram *pScaryRetroParallaxProgram = (*m_pShaderPrograms)[66];
            SetScaryRetroParallaxUniform(pScaryRetroParallaxProgram);
            RenderToScreen(pScaryRetroParallaxProgram);
            return;
        }
        case PostProcessingEffectMode::MoneyFilter: {
            CShaderProgram *pMoneyFilterProgram = (*m_pShaderPrograms)[67];
            SetMoneyFilterUniform(pMoneyFilterProgram);
            RenderToScreen(pMoneyFilterProgram);
            return;
        }
        case PostProcessingEffectMode::MicroprismMosaic: {
            CShaderProgram *pMicroprismMosaicProgram = (*m_pShaderPrograms)[68];
            SetMicroprismMosaicUniform(pMicroprismMosaicProgram);
            RenderToScreen(pMicroprismMosaicProgram);
            return;
        }
        case PostProcessingEffectMode::BayerMatrixDithering: {
            CShaderProgram *pBayerMatrixDitheringProgram = (*m_pShaderPrograms)[70];
            SetBayerMatrixDitheringUniform(pBayerMatrixDitheringProgram);
            RenderToScreen(pBayerMatrixDitheringProgram);
            return;
        }
        case PostProcessingEffectMode::JuliaFreak: {
            CShaderProgram *pJuliaFreakProgram = (*m_pShaderPrograms)[71];
            SetJuliaFreakUniform(pJuliaFreakProgram);
            RenderToScreen(pJuliaFreakProgram);
            return;
        }
        case PostProcessingEffectMode::HeartBlend: {
            CShaderProgram *pHeartBlendProgram = (*m_pShaderPrograms)[72];
            SetHeartBlendUniform(pHeartBlendProgram);
            RenderToScreen(pHeartBlendProgram);
            return;
        }
        case PostProcessingEffectMode::EMInterference: {
            CShaderProgram *pEMInterferencedProgram = (*m_pShaderPrograms)[73];
            SetEMInterferenceUniform(pEMInterferencedProgram);
            RenderToScreen(pEMInterferencedProgram);
            return;
        }
        case PostProcessingEffectMode::CubicLensDistortion: {
            CShaderProgram *pCubicLensDistortionProgram = (*m_pShaderPrograms)[74];
            SetCubicLensDistortionUniform(pCubicLensDistortionProgram);
            RenderToScreen(pCubicLensDistortionProgram);
            return;
        }
        case PostProcessingEffectMode::CelShaderish: {
            CShaderProgram *pCelShaderishProgram = (*m_pShaderPrograms)[75];
            SetCelShaderishUniform(pCelShaderishProgram);
            RenderToScreen(pCelShaderishProgram);
            return;
        }
        case PostProcessingEffectMode::CartoonVideo: {
            CShaderProgram *pCartoonVideoProgram = (*m_pShaderPrograms)[76];
            SetCartoonVideoUniform(pCartoonVideoProgram);
            RenderToScreen(pCartoonVideoProgram);
            return;
        }
        default: return;
        
    }
    
}

/// render to screen with frame buffer
void Game::RenderToScreen(CShaderProgram *pShaderProgram, const FrameBufferType &fboType,
                          const GLuint &bufferIndex, const TextureType &textureType) {
    switch(fboType) {
        case FrameBufferType::Default:
            currentFBO->BindTexture(static_cast<GLint>(textureType)); // ambient
            break;
        case FrameBufferType::DepthMapping:
            currentFBO->BindDepthTexture(static_cast<GLint>(textureType)); // depth
            break;
        case FrameBufferType::PingPongRendering:
            currentFBO->BindPingPongTexture(bufferIndex, static_cast<GLint>(textureType));
            break;
        case FrameBufferType::GeometryBuffer:
            currentFBO->BindHDRTexture(bufferIndex, static_cast<GLint>(textureType));
            break;
        default: break;
    }
                              
    SetMaterialUniform(pShaderProgram, "material", m_woodenBoxesColor, m_materialShininess);
    SetImageProcessingUniform(pShaderProgram, true);
    m_pQuad->Render(false);
}

void Game::ResetFrameBuffer(const GLboolean &clearBuffers) {
    // It is useful to switch back to the default framebuffer for this to easily see your results.
    // Unbind to render to our default framebuffer or switching back to the default buffer at 0.
    // To make sure all rendering operations will have a visual impact on the main window we need to make the default framebuffer active again by binding to 0:
    // essentially, we just bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    m_gameWindow->SetViewport();
    
    // clear all relevant buffers, set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
    if (clearBuffers) m_gameWindow->ClearBuffers();
    
    // disable depth test so screen-space quad isn't discarded due to depth test.
    glDisable(GL_DEPTH_TEST);
}

// Render scene method runs
void Game::RenderPPFX(const PostProcessingEffectMode &mode)
{
    //  Post Processing Effects
    // render the result on the default frame buffer using a full screen quad with post proccessing effects
    switch(mode) {
        case PostProcessingEffectMode::PBR:
            RenderPPFXScene(PostProcessingEffectMode::PBR);
            break;
        case PostProcessingEffectMode::IBL:
            RenderPPFXScene(PostProcessingEffectMode::IBL);
            break;
        case PostProcessingEffectMode::BlinnPhong:
            RenderPPFXScene(PostProcessingEffectMode::BlinnPhong);
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
        case PostProcessingEffectMode::Pixelate:
            RenderPPFXScene(PostProcessingEffectMode::Pixelate);
            break;
        case PostProcessingEffectMode::Pixelation:
            RenderPPFXScene(PostProcessingEffectMode::Pixelation);
            break;
        case PostProcessingEffectMode::KnittedPixelation:
            RenderPPFXScene(PostProcessingEffectMode::KnittedPixelation);
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
        case PostProcessingEffectMode::DepthMapping:
            RenderPPFXScene(PostProcessingEffectMode::DepthMapping);
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
        case PostProcessingEffectMode::HDRToneMapping:
            RenderPPFXScene(PostProcessingEffectMode::HDRToneMapping);
            break;
        case PostProcessingEffectMode::LensFlare:
            RenderPPFXScene(PostProcessingEffectMode::LensFlare);
            break;
        case PostProcessingEffectMode::FXAA:
            RenderPPFXScene(PostProcessingEffectMode::FXAA);
            break;
        case PostProcessingEffectMode::DeferredRendering:
            RenderPPFXScene(PostProcessingEffectMode::DeferredRendering);
            break;
        case PostProcessingEffectMode::SSAO:
            RenderPPFXScene(PostProcessingEffectMode::SSAO);
            break;
        case PostProcessingEffectMode::RainDrops:
            RenderPPFXScene(PostProcessingEffectMode::RainDrops);
            break;
        case PostProcessingEffectMode::PaletteQuantizationAndDithering:
            RenderPPFXScene(PostProcessingEffectMode::PaletteQuantizationAndDithering);
            break;
        case PostProcessingEffectMode::DistortedTV:
            RenderPPFXScene(PostProcessingEffectMode::DistortedTV);
            break;
        case PostProcessingEffectMode::RGBDisplay:
            RenderPPFXScene(PostProcessingEffectMode::RGBDisplay);
            break;
        case PostProcessingEffectMode::RetroParallax:
            RenderPPFXScene(PostProcessingEffectMode::RetroParallax);
            break;
        case PostProcessingEffectMode::ScaryRetroParallax:
            RenderPPFXScene(PostProcessingEffectMode::ScaryRetroParallax);
            break;
        case PostProcessingEffectMode::MoneyFilter:
            RenderPPFXScene(PostProcessingEffectMode::MoneyFilter);
            break;
        case PostProcessingEffectMode::MicroprismMosaic:
            RenderPPFXScene(PostProcessingEffectMode::MicroprismMosaic);
            break;
        case PostProcessingEffectMode::BayerMatrixDithering:
            RenderPPFXScene(PostProcessingEffectMode::BayerMatrixDithering);
            break;
        case PostProcessingEffectMode::JuliaFreak:
            RenderPPFXScene(PostProcessingEffectMode::JuliaFreak);
            break;
        case PostProcessingEffectMode::HeartBlend:
            RenderPPFXScene(PostProcessingEffectMode::HeartBlend);
            break;
        case PostProcessingEffectMode::EMInterference:
            RenderPPFXScene(PostProcessingEffectMode::EMInterference);
            break;
        case PostProcessingEffectMode::CubicLensDistortion:
            RenderPPFXScene(PostProcessingEffectMode::CubicLensDistortion);
            break;
        case PostProcessingEffectMode::CelShaderish:
            RenderPPFXScene(PostProcessingEffectMode::CelShaderish);
            break;
        case PostProcessingEffectMode::CartoonVideo:
            RenderPPFXScene(PostProcessingEffectMode::CartoonVideo);
            break;
    }
    
}

/// convert post processing effect to string
const char * const Game::PostProcessingEffectToString(const PostProcessingEffectMode &mode){
    
    //  Post Processing Effects
    // render the result on the default frame buffer using a full screen quad with post proccessing effects
    switch(mode) {
        case PostProcessingEffectMode::PBR:
        return "Physically Based Rendering";
        case PostProcessingEffectMode::IBL:
        return "Image Based Lighting";
        case PostProcessingEffectMode::BlinnPhong:
        return "Blinn-Phong Lighting";
        case PostProcessingEffectMode::ColorInversion:
        return "Color Inversion";
        case PostProcessingEffectMode::GrayScale:
        return "GrayScale";
        case PostProcessingEffectMode::Kernel:
        return "Kernel";
        case PostProcessingEffectMode::KernelBlur:
        return "Kernel Blur";
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
        case PostProcessingEffectMode::Pixelate:
        return "Pixelate";
        case PostProcessingEffectMode::Pixelation:
        return "Pixelation";
        case PostProcessingEffectMode::KnittedPixelation:
        return "Knitted Pixelation";
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
        case PostProcessingEffectMode::DepthMapping:
            return "Depth Mapping";
        case PostProcessingEffectMode::Vignetting:
            return "Vignetting";
        case PostProcessingEffectMode::BrightParts:
            return "Bright Parts";
        case PostProcessingEffectMode::Bloom:
            return "Bloom";
        case PostProcessingEffectMode::HDRToneMapping:
            return "HDR Tone Mapping";
        case PostProcessingEffectMode::LensFlare:
            return "Lens Flare";
        case PostProcessingEffectMode::FXAA:
            return "Fast Approximate Anti-Aliasing (FXAA)";
        case PostProcessingEffectMode::DeferredRendering:
            return "Deferred Rendering";
        case PostProcessingEffectMode::SSAO:
            return "Screen Space Ambient Occlusion";
        case PostProcessingEffectMode::RainDrops:
            return "Rain Drops";
        case PostProcessingEffectMode::PaletteQuantizationAndDithering:
            return "Palette Quantization And Dithering";
        case PostProcessingEffectMode::DistortedTV:
            return "Distorted TV";
        case PostProcessingEffectMode::RGBDisplay:
            return "RGB Display";
        case PostProcessingEffectMode::RetroParallax:
            return "Retro Parallax";
        case PostProcessingEffectMode::ScaryRetroParallax:
            return "Scary Retro Parallax";
        case PostProcessingEffectMode::MoneyFilter:
            return "Money Filter";
        case PostProcessingEffectMode::MicroprismMosaic:
            return "Microprism Mosaic";
        case PostProcessingEffectMode::BayerMatrixDithering:
            return "Bayer Matrix Dithering";
        case PostProcessingEffectMode::JuliaFreak:
            return "Julia Freak";
        case PostProcessingEffectMode::HeartBlend:
            return "Heart Blend";
        case PostProcessingEffectMode::EMInterference:
            return "EM Interference";
        case PostProcessingEffectMode::CubicLensDistortion:
            return "Cubic Lens Distortion";
        case PostProcessingEffectMode::CelShaderish:
            return "Cel Shaderish";
        case PostProcessingEffectMode::CartoonVideo:
            return "Cartoon Video";
        default:
        return "";
    }
    
}

FrameBufferType Game::GetFBOtype(const PostProcessingEffectMode &mode){
    
    switch(mode) {
        case PostProcessingEffectMode::GaussianBlur:
        return FrameBufferType::GeometryBuffer;
        case PostProcessingEffectMode::BrightParts:
        return FrameBufferType::GeometryBuffer;
        case PostProcessingEffectMode::Bloom:
        return FrameBufferType::GeometryBuffer;
        case PostProcessingEffectMode::HDRToneMapping:
        return FrameBufferType::GeometryBuffer;
        case PostProcessingEffectMode::LensFlare:
        return FrameBufferType::GeometryBuffer;
        case PostProcessingEffectMode::DeferredRendering:
        return FrameBufferType::GeometryBuffer;
        case PostProcessingEffectMode::SSAO:
        return FrameBufferType::GeometryBuffer;
        default:
        return FrameBufferType::Default;
    }
    
}

