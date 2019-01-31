//
//  Game+PostProcessing.cpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 18/09/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Game.h"

/// render to post processing effect frame buffer
void Game::InitialiseFrameBuffers(const GLuint &width , const GLuint &height) {
    
}

/// render to post processing effect scene
void Game::RenderPPFXScene(const PostProcessingEffectMode &mode) {
    
    
}

/// render to screen with frame buffer
void Game::RenderToScreen(CShaderProgram *pShaderProgram, const bool & useQuad) {
    
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
        case PostProcessingEffectMode::EdgeDetection:
            return "Edge Detection";
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
        case PostProcessingEffectMode::FrostedGlass:
            return "Frosted Glass";
        case PostProcessingEffectMode::FrostedGlassExtra:
            return "Frosted Glass Extra";
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
        case PostProcessingEffectMode::EdgeDetection:
            RenderPPFXScene(PostProcessingEffectMode::EdgeDetection);
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
        case PostProcessingEffectMode::FrostedGlass:
            RenderPPFXScene(PostProcessingEffectMode::FrostedGlass);
            break;
        case PostProcessingEffectMode::FrostedGlassExtra:
            RenderPPFXScene(PostProcessingEffectMode::FrostedGlassExtra);
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
            //RenderScene(PostProcessingEffectMode::GaussianBlur);
            //m_horizontal = !m_horizontal;
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

