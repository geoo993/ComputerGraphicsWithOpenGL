//
//  Game+IInputs.cpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 06/10/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//
// https://www.youtube.com/watch?v=EE5cS8EMT78
// https://www.youtube.com/watch?v=L2aiuDDFNIk

#include "Game.h"

// Controls
CControl *controlled = nullptr; // hold the current control that is beign manipulated
MouseState mState;
KeyboardState kState;

static void OnMouseEnter_callback(GLFWwindow* window, int entered);
static void OnMouseMove_callback(GLFWwindow* window, double xpos, double ypos);
static void OnMouseDown_callback(GLFWwindow* window, int button, int action, int mods);
static void OnMouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void OnKeyDown_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


const char * const Game::ControlTypeToString(const ControlType &type){
    switch(type) {
        case ControlType::LEFTMOUSE:
            return "LEFT MOUSE";
        case ControlType::MIDDLEMOUSE:
            return "MIDDLE MOUSE";
        case ControlType::RIGHTMOUSE:
            return "RIGHT MOUSE";
        case ControlType::KEYFORWARD:
            return "KEY FORWARD";
        case ControlType::KEYBACKWARD:
            return "KEY BACKWARD";
        case ControlType::KEYLEFT:
            return "KEY LEFT";
        case ControlType::KEYRIGHT:
            return "KEY RIGHT";
        case ControlType::UNKNOWN:
            return "Unknown";
        default:
            return "";
    }
}

void Game::LoadControls() {
    
    m_gameWindow->SetInputs(OnMouseEnter_callback, OnMouseMove_callback,
                            OnMouseDown_callback, OnMouseScroll_callback,
                            OnKeyDown_callback);
     
    GUIBoxData *guiBox = new GUIBoxData();
    guiBox->x = 10;
    guiBox->y = 10;
    guiBox->width = 200;
    guiBox->height = 30;
    guiBox->textSize = 15;
    
    /// LEFT SCREEN
    CButton * useDir = (CButton *)AddControl(new CButton("Use Directional Light", guiBox));
    useDir->SetValue(&m_useDir);
    guiBox->y += guiBox->height;
    
    CButton * usePoint = (CButton *)AddControl(new CButton("Use Point Light", guiBox));
    usePoint->SetValue(&m_usePoint);
    guiBox->y += guiBox->height;
    
    CButton * useSpot = (CButton *)AddControl(new CButton("Use Spot Light", guiBox));
    useSpot->SetValue(&m_useSpot);
    guiBox->y += guiBox->height;
    
    CButton * useSmoothSpot = (CButton *)AddControl(new CButton("Use Smooth Spot Light", guiBox));
    useSmoothSpot->SetValue(&m_useSmoothSpot);
    guiBox->y += guiBox->height;
    
    CButton * useBlinn = (CButton *)AddControl(new CButton("Use Blinn Light", guiBox));
    useBlinn->SetValue(&m_useBlinn);
    guiBox->y += guiBox->height;
    
    CSlider *dirIntensity = (CSlider *)AddControl(new CSlider("Dir Light Intensity", 0.0f, 2.0f, 5, guiBox));
    dirIntensity->SetValue(&m_dirIntensity);
    guiBox->y += guiBox->height;
    
    CSlider *pointIntensity = (CSlider *)AddControl(new CSlider("Point Light Intensity", 0.0f, 100.0f, 5, guiBox));
    pointIntensity->SetValue(&m_pointIntensity);
    guiBox->y += guiBox->height;
    
    CSlider *spotIntensity = (CSlider *)AddControl(new CSlider("Spot Light Intensity", 0.0f, 100.0f, 5, guiBox));
    spotIntensity->SetValue(&m_spotIntensity);
    guiBox->y += guiBox->height;
    
    CSlider *spotCutoff = (CSlider *)AddControl(new CSlider("Spot Light Cutoff", 0.0f, 50.0f, 5, guiBox));
    spotCutoff->SetValue(&m_spotCutOff);
    guiBox->y += guiBox->height;
    
    CSlider *spotOuterCutoff = (CSlider *)AddControl(new CSlider("Spot Light Outer Cutoff", 0.0f, 50.0f, 5, guiBox));
    spotOuterCutoff->SetValue(&m_spotOuterCutOff);
    guiBox->y += guiBox->height + 5;

    CSlider *ambientLight = (CSlider *)AddControl(new CSlider("Ambient Intensity", 0.0f, 5.0f, 5, guiBox));
    ambientLight->SetValue(&m_ambient);
    guiBox->y += guiBox->height;
    
    CSlider *diffuseLight = (CSlider *)AddControl(new CSlider("Diffuse Intensity", 0.0f, 10.0f, 5, guiBox));
    diffuseLight->SetValue(&m_diffuse);
    guiBox->y += guiBox->height;
    
    CSlider *specularLight = (CSlider *)AddControl(new CSlider("Specular Intensity", 0.0f, 10.0f, 5, guiBox));
    specularLight->SetValue(&m_specular);
    guiBox->y += guiBox->height + 5;
    
    CSlider *constantAtt = (CSlider *)AddControl(new CSlider("Attenuation Constant", 0.0f, 1.0f, 5, guiBox));
    constantAtt->SetValue(&m_constant);
    guiBox->y += guiBox->height;
    
    CSlider *linearAtt = (CSlider *)AddControl(new CSlider("Attenuation Linear", 0.0f, 0.1f, 5, guiBox));
    linearAtt->SetValue(&m_linear);
    guiBox->y += guiBox->height;
    
    CSlider *quadraticAtt = (CSlider *)AddControl(new CSlider("Attenuation Quadratic", 0.0f, 0.1f, 5, guiBox));
    quadraticAtt->SetValue(&m_exponent);
    guiBox->y += guiBox->height + 5;
    
    CButton * matTexture = (CButton *)AddControl(new CButton("Material Texture", guiBox));
    matTexture->SetValue(&m_materialUseTexture);
    guiBox->y += guiBox->height;
    
    CButton * matColor = (CButton *)AddControl(new CButton("Material Color", guiBox));
    matColor->SetValue(&m_materialUseColor);
    guiBox->y += guiBox->height;
    
    CSlider *matShiniess = (CSlider *)AddControl(new CSlider("Material Shininess", 0.0f, 30.0f, 5, guiBox));
    matShiniess->SetValue(&m_materialShininess);
    guiBox->y += guiBox->height;
    
    CSlider *matUVTilling = (CSlider *)AddControl(new CSlider("Material UV Tilling", 0.0f, 5.0f, 5, guiBox));
    matUVTilling->SetValue(&m_uvTiling);
    guiBox->y += guiBox->height;
    
    CSlider *matParallax = (CSlider *)AddControl(new CSlider("Material Parallax Scale", 0.0f, 2.0f, 5, guiBox));
    matParallax->SetValue(&m_parallaxHeightScale);
    guiBox->y += guiBox->height;
    
    CButton * matRefraction = (CButton *)AddControl(new CButton("Material Refraction", guiBox));
    matRefraction->SetValue(&m_useRefraction);
    guiBox->y += guiBox->height + 5;
    
    CSlider *albedo = (CSlider *)AddControl(new CSlider("Albedo", 0.0f, 1.0f, 5, guiBox));
    albedo->SetValue(&m_albedo);
    guiBox->y += guiBox->height;
    
    CSlider *metallic = (CSlider *)AddControl(new CSlider("Metallic", 0.0f, 1.0f, 5, guiBox));
    metallic->SetValue(&m_metallic);
    guiBox->y += guiBox->height;
    
    CSlider *roughness = (CSlider *)AddControl(new CSlider("Roughness", 0.0f, 1.0f, 5, guiBox));
    roughness->SetValue(&m_roughness);
    guiBox->y += guiBox->height;
    
    CSlider *ao = (CSlider *)AddControl(new CSlider("AO", 0.0f, 1.0f, 5, guiBox));
    ao->SetValue(&m_ao);
    guiBox->y += guiBox->height;
    
    /// RIGHT SCREEN
    GLint rightStartingY = 10;
    guiBox->x = SCREEN_WIDTH - guiBox->width - 10;
    guiBox->y = rightStartingY;
    
    GLint itemHeight = guiBox->height;
    
    guiBox->height = itemHeight * m_pSkybox->GetNumberOfSkyboxes();
    CListBox * skyboxeNames = (CListBox *)AddControl(new CListBox(guiBox, itemHeight));
    for(auto const& value: m_pSkybox->GetSkyboxes()) {
        skyboxeNames->AddItem(value);
    }
    skyboxeNames->SetValue(&m_skyboxNumber, &m_changeSkybox);
    
    guiBox->height = itemHeight;
    
    GLint fogY = (guiBox->height * m_pSkybox->GetNumberOfSkyboxes()) + 5;
    guiBox->y += fogY;
    
    CButton * useFog = (CButton *)AddControl(new CButton("Use Fog", guiBox));
    useFog->SetValue(&m_useFog);
    guiBox->y += guiBox->height;
    
    CButton * showTerrain = (CButton *)AddControl(new CButton("Show Terrain", guiBox));
    showTerrain->SetValue(&m_showTerrain);
    guiBox->y += guiBox->height;
    
    CButton * useTerrain = (CButton *)AddControl(new CButton("Use Terrain", guiBox));
    useTerrain->SetValue(&m_useTerrain);
    guiBox->y += guiBox->height;
    
    /// Post Processing Effects Selection
    guiBox->width -= 100;
    CButton * previousPPFX = (CButton *)AddControl(new CButton("Prev", guiBox));
    previousPPFX->SetValue(&m_prevPPFXMode);
    
    guiBox->x += 100;
    CButton * nextPPFX = (CButton *)AddControl(new CButton("Next", guiBox));
    nextPPFX->SetValue(&m_nextPPFXMode);
    
    GLint spaceAtCoverage = 5;
    
    /// Post Processing Effects Coverage
    GLint ppfxY = (guiBox->height * m_pSkybox->GetNumberOfSkyboxes()) + (guiBox->height * 4) + spaceAtCoverage;
    //guiBox->y += ppfxY;
    guiBox->width += 100;
    guiBox->x -= 100;
    guiBox->y += guiBox->height;
    CSlider *coverage = (CSlider *)AddControl(new CSlider("PPFX Coverage", 0.0f, 1.0f, 5, guiBox));
    coverage->SetValue(&m_coverage);
    
    // PBR
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CButton * pbruseIrradiance = (CButton *)AddControl(new CButton("Use Irradiance", guiBox,
                                                                   GUIMode::DYNAMIC, false, PostProcessingEffectMode::PBR));
    pbruseIrradiance->SetValue(&m_useIrradiance);
    guiBox->y += guiBox->height;
    
    CButton * pbruseHDR = (CButton *)AddControl(new CButton("Use HDR Light", guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::PBR));
    pbruseHDR->SetValue(&m_HDR);
    guiBox->y += guiBox->height;
    
    CSlider *pbrlightExposure = (CSlider *)AddControl(new CSlider("Light Exposure", 0.0f, 1.0f, 5,
                                                                  guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::PBR));
    pbrlightExposure->SetValue(&m_exposure);
    guiBox->y += guiBox->height;
    
    CSlider *pbrGamma = (CSlider *)AddControl(new CSlider("Gamma Correction", 0.0f, 3.0f, 5,
                                                          guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::PBR));
    pbrGamma->SetValue(&m_gama);
    
    // IBL
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CButton * iiluseIrradianceMap = (CButton *)AddControl(new CButton("Use Irradiance Map", guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::IBL));
    iiluseIrradianceMap->SetValue(&m_useIrradianceMap);
    guiBox->y += guiBox->height;
    
    CButton * iiluseIrradiance = (CButton *)AddControl(new CButton("Use Irradiance", guiBox,
                                                                   GUIMode::DYNAMIC, false, PostProcessingEffectMode::IBL));
    iiluseIrradiance->SetValue(&m_useIrradiance);
    guiBox->y += guiBox->height;
    
    CButton * iiluseHDR = (CButton *)AddControl(new CButton("Use HDR Light", guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::IBL));
    iiluseHDR->SetValue(&m_HDR);
    guiBox->y += guiBox->height;
    
    CSlider *iillightExposure = (CSlider *)AddControl(new CSlider("Light Exposure", 0.0f, 1.0f, 5,
                                                                  guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::IBL));
    iillightExposure->SetValue(&m_exposure);
    guiBox->y += guiBox->height;
    
    CSlider *iilGamma = (CSlider *)AddControl(new CSlider("Gamma Correction", 0.0f, 3.0f, 5,
                                                          guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::IBL));
    iilGamma->SetValue(&m_gama);
    
    // Screen wave
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *screenWaveOffset = (CSlider *)AddControl(new CSlider("Offset", 0.0f, 2.0f, 5, guiBox,
                                                                  GUIMode::DYNAMIC, false, PostProcessingEffectMode::ScreenWave));
    screenWaveOffset->SetValue(&m_screenWaveOffset);
    
    /// Swirl
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *swirlRadius = (CSlider *)AddControl(new CSlider("Radius", 0.0f, SCREEN_HEIGHT, 5, guiBox,
                                                             GUIMode::DYNAMIC, false, PostProcessingEffectMode::Swirl));
    swirlRadius->SetValue(&m_swirlRadius);
    guiBox->y += guiBox->height;
    
    CSlider *swirlAngle = (CSlider *)AddControl(new CSlider("Angle", 0.0f, 3.14159265359f, 5, guiBox,
                                                             GUIMode::DYNAMIC, false, PostProcessingEffectMode::Swirl));
    swirlAngle->SetValue(&m_swirlAngle);

    /// Night Vision
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *nightVisionluminanceThreshold = (CSlider *)AddControl(new CSlider("Luminance Threshold", 0.0f, 1.0f, 5, guiBox,
                                                             GUIMode::DYNAMIC, false, PostProcessingEffectMode::NightVision));
    nightVisionluminanceThreshold->SetValue(&m_nightVisionluminanceThreshold);
    guiBox->y += guiBox->height;
    
    CSlider *nightVisionColorAmplification = (CSlider *)AddControl(new CSlider("Color Amplification", 0.0f, 10.0f, 5, guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::NightVision));
    nightVisionColorAmplification->SetValue(&m_nightVisionColorAmplification);
    
    /// Posterization
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *posterizationGama = (CSlider *)AddControl(new CSlider("Gama", 0.0f, 2.0f, 5, guiBox,
                                                                   GUIMode::DYNAMIC, false, PostProcessingEffectMode::Posterization));
    posterizationGama->SetValue(&m_posterizationGama);
    guiBox->y += guiBox->height;
    
    CSlider *posterizationColors = (CSlider *)AddControl(new CSlider("Number Of Colors", 0.0f, 20.0f, 5, guiBox,
                                                                    GUIMode::DYNAMIC, false, PostProcessingEffectMode::Posterization));
    posterizationColors->SetValue(&m_posterizationColors);
    
     /// Pixelate
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *pixelateSize = (CSlider *)AddControl(new CSlider("Pixel Size", 0.0f, 20.0f, 5, guiBox,
    GUIMode::DYNAMIC, false, PostProcessingEffectMode::Pixelate));
    pixelateSize->SetValue(&m_pixelateSize);
    
    /// Pixelation
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *pixelationWidth = (CSlider *)AddControl(new CSlider("Pixel Width", 0.0f, 20.0f, 5, guiBox,
                                                                   GUIMode::DYNAMIC, false, PostProcessingEffectMode::Pixelation));
    pixelationWidth->SetValue(&m_pixelWidth);
    guiBox->y += guiBox->height;
    
    CSlider *pixelationHeight = (CSlider *)AddControl(new CSlider("Pixel Height", 0.0f, 20.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::Pixelation));
    pixelationHeight->SetValue(&m_pixelHeight);
    
    /// Frosted Glass Effect
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *frostedGlassPixelX = (CSlider *)AddControl(new CSlider("Pixel X", 0.0f, 20.0f, 5, guiBox,
                                                                 GUIMode::DYNAMIC, false, PostProcessingEffectMode::FrostedGlassPixelationEffect));
    frostedGlassPixelX->SetValue(&m_frostedGlassPixelX);
    guiBox->y += guiBox->height;
    
    CSlider *frostedGlassPixelY = (CSlider *)AddControl(new CSlider("Pixel Y", 0.0f, 20.0f, 5, guiBox,
                                                                  GUIMode::DYNAMIC, false, PostProcessingEffectMode::FrostedGlassPixelationEffect));
    frostedGlassPixelY->SetValue(&m_frostedGlassPixelY);
    guiBox->y += guiBox->height;
    
    CSlider *frostedGlassFrequency = (CSlider *)AddControl(new CSlider("Frequency", 0.0f, 1.0f, 5, guiBox,
                                                                    GUIMode::DYNAMIC, false, PostProcessingEffectMode::FrostedGlassPixelationEffect));
    frostedGlassFrequency->SetValue(&m_frostedGlassFrequency);
    
    /// Frosted Glass
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *frostedGlassRandomFactor = (CSlider *)AddControl(new CSlider("Random Factor", 0.0f, 0.1f, 5, guiBox,
                                                                    GUIMode::DYNAMIC, false, PostProcessingEffectMode::FrostedGlassScreenWaveEffect));
    frostedGlassRandomFactor->SetValue(&m_frostedGlassRandomFactor);
    guiBox->y += guiBox->height;
    
    CSlider *frostedGlassRandomScale = (CSlider *)AddControl(new CSlider("Random Scale", 0.0f, 0.1f, 5, guiBox,
                                                                    GUIMode::DYNAMIC, false, PostProcessingEffectMode::FrostedGlassScreenWaveEffect));
    frostedGlassRandomScale->SetValue(&m_frostedGlassRandomScale);
    
    /// Crosshatching
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *crosshatchingOffset = (CSlider *)AddControl(new CSlider("Offset", 0.0f, 10.0f, 5, guiBox,
                                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::Crosshatching));
    crosshatchingOffset->SetValue(&m_crosshatchingOffset);
    guiBox->y += guiBox->height;
    
    CSlider *crosshatchingThreshold_1 = (CSlider *)AddControl(new CSlider("Luminance Threshold 1", 0.0f, 1.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::Crosshatching));
    crosshatchingThreshold_1->SetValue(&m_crosshatchingThreshold_1);
    guiBox->y += guiBox->height;
    
    CSlider *crosshatchingThreshold_2 = (CSlider *)AddControl(new CSlider("Luminance Threshold 2", 0.0f, 1.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::Crosshatching));
    crosshatchingThreshold_2->SetValue(&m_crosshatchingThreshold_2);
    guiBox->y += guiBox->height;
    
    CSlider *crosshatchingThreshold_3 = (CSlider *)AddControl(new CSlider("Luminance Threshold 3", 0.0f, 1.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::Crosshatching));
    crosshatchingThreshold_3->SetValue(&m_crosshatchingThreshold_3);
    guiBox->y += guiBox->height;
    
    CSlider *crosshatchingThreshold_4 = (CSlider *)AddControl(new CSlider("Luminance Threshold 4", 0.0f, 1.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::Crosshatching));
    crosshatchingThreshold_4->SetValue(&m_crosshatchingThreshold_4);
    
    // Toonify
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *toonifyLowerTres = (CSlider *)AddControl(new CSlider("Lower Threshold", 0.0f, 1.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::Toonify));
    toonifyLowerTres->SetValue(&m_toonifyLowerTres);
    guiBox->y += guiBox->height;
    
    CSlider *toonifyUpperTres = (CSlider *)AddControl(new CSlider("Upper Threshold", 0.0f, 10.0f, 5, guiBox,
                                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::Toonify));
    toonifyUpperTres->SetValue(&m_toonifyUpperTres);
    
    // Fish Eye
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *fishEyeRadius = (CSlider *)AddControl(new CSlider("Radius", 0.0f, 1.0f, 5, guiBox,
                                                                  GUIMode::DYNAMIC, false, PostProcessingEffectMode::FishEye));
    fishEyeRadius->SetValue(&m_fishEyeRadius);
    
    // Barrel Distortion
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *barrelDistortionPower = (CSlider *)AddControl(new CSlider("Power", 0.0f, 10.0f, 5, guiBox,
                                                                  GUIMode::DYNAMIC, false, PostProcessingEffectMode::BarrelDistortion));
    barrelDistortionPower->SetValue(&m_barrelDistortionPower);
    
    // MultiScreenFishEye
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *multiScreenFishEyeOffsetX = (CSlider *)AddControl(new CSlider("X Offset", 0.0f, 1.0f, 5, guiBox,
                                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::MultiScreenFishEye));
    multiScreenFishEyeOffsetX->SetValue(&m_multiScreenFishEyeOffsetX);
    guiBox->y += guiBox->height;
    
    CSlider *multiScreenFishEyeOffsetY = (CSlider *)AddControl(new CSlider("Y Offset", 0.0f, 1.0f, 5, guiBox,
                                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::MultiScreenFishEye));
    multiScreenFishEyeOffsetY->SetValue(&m_multiScreenFishEyeOffsetY);
    guiBox->y += guiBox->height;
    
    CSlider *multiScreenFishEyeRadius = (CSlider *)AddControl(new CSlider("Radius", 0.0f, 5.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::MultiScreenFishEye));
    multiScreenFishEyeRadius->SetValue(&m_multiScreenFishEyeRadius);
    guiBox->y += guiBox->height;
    
    CSlider *multiScreenFishEyeCurvature = (CSlider *)AddControl(new CSlider("Curvature", 0.0f, 10.0f, 5, guiBox,
                                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::MultiScreenFishEye));
    multiScreenFishEyeCurvature->SetValue(&m_multiScreenFishEyeCurvature);
    
    // FishEyeLens
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *fishEyeLensSize = (CSlider *)AddControl(new CSlider("Size", 0.0f, 1.0f, 5, guiBox,
                                                                           GUIMode::DYNAMIC, false, PostProcessingEffectMode::FishEyeLens));
    fishEyeLensSize->SetValue(&m_fishEyeLensSize);
    
    // Gaussian Blur
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *gaussianBlurIntensity = (CSlider *)AddControl(new CSlider("Intensity", 0.0f, 2.0f, 5, guiBox,
                                                                 GUIMode::DYNAMIC, false, PostProcessingEffectMode::GaussianBlur));
    gaussianBlurIntensity->SetValue(&m_gaussianBlurIntensity);
    
    // Radial Blur
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *radialBlurRadius = (CSlider *)AddControl(new CSlider("Radius", 0.0f, 1.0f, 5, guiBox,
                                                                           GUIMode::DYNAMIC, false, PostProcessingEffectMode::RadialBlur));
    radialBlurRadius->SetValue(&m_radialBlurRadius);
    guiBox->y += guiBox->height;
    
    CSlider *radialBlurResolution = (CSlider *)AddControl(new CSlider("Resolution", 0.0f, 10.0f, 5, guiBox,
                                                                           GUIMode::DYNAMIC, false, PostProcessingEffectMode::RadialBlur));
    radialBlurResolution->SetValue(&m_radialBlurResolution);
    
    // Motion Blur
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *motionBlurTargetFps = (CSlider *)AddControl(new CSlider("Target FPS", 0.0f, 120.0f, 5, guiBox,
                                                                  GUIMode::DYNAMIC, false, PostProcessingEffectMode::MotionBlur));
    motionBlurTargetFps->SetValue(&m_motionBlurTargetFps);
    guiBox->y += guiBox->height;
    
    CSlider *motionBlurSamples = (CSlider *)AddControl(new CSlider("Samples", 0.0f, 50.0f, 5, guiBox,
                                                                      GUIMode::DYNAMIC, false, PostProcessingEffectMode::MotionBlur));
    motionBlurSamples->SetValue(&m_motionBlurSamples);
    
    // Vignetting
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CButton * vignettingTint = (CButton *)AddControl(new CButton("Tint", guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::Vignetting));
    vignettingTint->SetValue(&m_vignettingTint);
    guiBox->y += guiBox->height;
    
    CButton * vignettingSepia = (CButton *)AddControl(new CButton("Use Sepia", guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::Vignetting));
    vignettingSepia->SetValue(&m_vignettingSepia);
    guiBox->y += guiBox->height;
    
    CSlider *vignettingRadius = (CSlider *)AddControl(new CSlider("Radius", 0.0f, 1.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::Vignetting));
    vignettingRadius->SetValue(&m_vignettingRadius);
    guiBox->y += guiBox->height;
    
    CSlider *vignettingSoftness = (CSlider *)AddControl(new CSlider("Softness", 0.0f, 1.0f, 5, guiBox,
                                                                   GUIMode::DYNAMIC, false, PostProcessingEffectMode::Vignetting));
    vignettingSoftness->SetValue(&m_vignettingSoftness);
    
    
    // BrightParts
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
        CButton * brightPartsUseHDR = (CButton *)AddControl(new CButton("Use HDR Light", guiBox,
                                                              GUIMode::DYNAMIC, false, PostProcessingEffectMode::BrightParts));
    brightPartsUseHDR->SetValue(&m_HDR);
    guiBox->y += guiBox->height;
    
    CButton * brightPartsSmoothGradient = (CButton *)AddControl(new CButton("Smooth Gradient", guiBox,
                                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::BrightParts));
    brightPartsSmoothGradient->SetValue(&m_brightPartsSmoothGradient);
    guiBox->y += guiBox->height;
    
    CSlider *brightPartsIntensity = (CSlider *)AddControl(new CSlider("Intensity", 0.0f, 2.0f, 5, guiBox,
                                                                    GUIMode::DYNAMIC, false, PostProcessingEffectMode::BrightParts));
    brightPartsIntensity->SetValue(&m_brightPartsIntensity);
    
    // Bloom
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *bloomIntensity = (CSlider *)AddControl(new CSlider("Intensity", 0.0f, 2.0f, 5, guiBox,
                                                                       GUIMode::DYNAMIC, false, PostProcessingEffectMode::Bloom));
    bloomIntensity->SetValue(&m_gaussianBlurIntensity);
    guiBox->y += guiBox->height;
    
    CButton * bloomUseHDR = (CButton *)AddControl(new CButton("Use HDR Light", guiBox,
                                                              GUIMode::DYNAMIC, false, PostProcessingEffectMode::Bloom));
    bloomUseHDR->SetValue(&m_HDR);
    guiBox->y += guiBox->height;
    
    CSlider *bloomlightExposure = (CSlider *)AddControl(new CSlider("Light Exposure", 0.0f, 1.0f, 5,
                                                                    guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::Bloom));
    bloomlightExposure->SetValue(&m_exposure);
    guiBox->y += guiBox->height;
    
    CSlider *bloomGamma = (CSlider *)AddControl(new CSlider("Gamma Correction", 0.0f, 3.0f, 5,
                                                            guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::Bloom));
    bloomGamma->SetValue(&m_gama);
    
    
    // HRDToneMapping
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CButton * hdruseHDR = (CButton *)AddControl(new CButton("Use HDR Light", guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::HDRToneMapping));
    hdruseHDR->SetValue(&m_HDR);
    guiBox->y += guiBox->height;
    
    CSlider *hrdlightExposure = (CSlider *)AddControl(new CSlider("Light Exposure", 0.0f, 1.0f, 5,
                                                                    guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::HDRToneMapping));
    hrdlightExposure->SetValue(&m_exposure);
    guiBox->y += guiBox->height;
    
    CSlider *hdrGamma = (CSlider *)AddControl(new CSlider("Gamma Correction", 0.0f, 3.0f, 5,
                                                            guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::HDRToneMapping));
    hdrGamma->SetValue(&m_gama);
    
    // Lens Flare
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    guiBox->height = itemHeight * 3;
    GLboolean m_changeLensFlare = true;
    CListBox * lensFlareNames = (CListBox *)AddControl(new CListBox(guiBox, itemHeight,
                                                                    GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareNames->AddItem("Ghost");
    lensFlareNames->AddItem("Halo");
    lensFlareNames->AddItem("Ghost & Halo");
    lensFlareNames->SetValue(&m_lensFlareNumber, &m_changeLensFlare);
    guiBox->y += guiBox->height + 5;
    guiBox->height = itemHeight;
    
    CButton * lensFlareUseDirt = (CButton *)AddControl(new CButton("Use Dirt Texture", guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareUseDirt->SetValue(&m_lensFlareUseDirt);
    guiBox->y += guiBox->height;
    
    CSlider *lensFlareIntensity = (CSlider *)AddControl(new CSlider("Intensity", 0.0f, 2.0f, 5, guiBox,
                                                                GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareIntensity->SetValue(&m_gaussianBlurIntensity);
    guiBox->y += guiBox->height;
    
    CSlider *lensFlareGhostCount = (CSlider *)AddControl(new CSlider("Ghost Count", 0.0f, 10.0f, 5, guiBox,
                                                                    GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareGhostCount->SetValue(&m_lensFlareGhostCount);
    guiBox->y += guiBox->height;
    
    CSlider *lensFlareGhostDispersal = (CSlider *)AddControl(new CSlider("Ghost Dispersal", 0.0f, 2.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareGhostDispersal->SetValue(&m_lensFlareGhostDispersal);
    guiBox->y += guiBox->height;
    
    CSlider *lensFlareGhostThreshold = (CSlider *)AddControl(new CSlider("Ghost Threshold", 0.0f, 20.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareGhostThreshold->SetValue(&m_lensFlareGhostThreshold);
    guiBox->y += guiBox->height;
    
    CSlider *lensFlareGhostDistortion = (CSlider *)AddControl(new CSlider("Ghost Distortion", 0.0f, 5.0f, 5, guiBox,
                                                                     GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareGhostDistortion->SetValue(&m_lensFlareGhostDistortion);
    guiBox->y += guiBox->height;
    
    CSlider *lensFlareHaloRadius = (CSlider *)AddControl(new CSlider("Halo Radius", 0.0f, 2.0f, 5, guiBox,
                                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareHaloRadius->SetValue(&m_lensFlareHaloRadius);
    guiBox->y += guiBox->height;
    
    CSlider *lensFlareHaloThreshold = (CSlider *)AddControl(new CSlider("Halo Threshold", 0.0f, 20.0f, 5, guiBox,
                                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::LensFlare));
    lensFlareHaloThreshold->SetValue(&m_lensFlareHaloThreshold);
    guiBox->y += guiBox->height;
    
    // SSAO
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CButton * ssaoNoiseUseLight = (CButton *)AddControl(new CButton("Use Light", guiBox,
                                                                   GUIMode::DYNAMIC, false, PostProcessingEffectMode::SSAO));
    ssaoNoiseUseLight->SetValue(&m_ssaoNoiseUseLight);
    guiBox->y += guiBox->height;
    
    CSlider *ssaoBias = (CSlider *)AddControl(new CSlider("Bias", 0.0025f, 2.0f, 5, guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::SSAO));
    ssaoBias->SetValue(&m_ssaoBias);
    guiBox->y += guiBox->height;
    
    CSlider *ssaoRadius = (CSlider *)AddControl(new CSlider("Radius", 0.005f, 100.0f, 5, guiBox,
                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::SSAO));
    ssaoRadius->SetValue(&m_ssaoRadius);
    guiBox->y += guiBox->height;
    
    CSlider *ssaoNoise = (CSlider *)AddControl(new CSlider("Noise", 0.0f, 12.0f, 5, guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::SSAO));
    ssaoNoise->SetValue(&m_ssaoNoiseSize);
    
    // Fast Approximate Anti Aliasing
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CSlider *ffaaOffset = (CSlider *)AddControl(new CSlider("Offset", 0.0f, 128.0f, 5, guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::FXAA));
    ffaaOffset->SetValue(&m_ffaaOffset);
    
    // Depth Mapping
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CButton * depthMapping = (CButton *)AddControl(new CButton("Use Ortho Camera", guiBox,
                                                                   GUIMode::DYNAMIC, false, PostProcessingEffectMode::DepthMapping));
    depthMapping->SetValue(&m_isOrthographicCamera);
    guiBox->y += guiBox->height;
    
    CButton * depthMappingFromLight = (CButton *)AddControl(new CButton("From Light View", guiBox,
                                                               GUIMode::DYNAMIC, false, PostProcessingEffectMode::DepthMapping));
    depthMappingFromLight->SetValue(&m_fromLightPosition);
    
    // Directional Shaddow Mapping
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CButton * dirShadowMappingUseHDR = (CButton *)AddControl(new CButton("Use HDR Light", guiBox,
                                                                          GUIMode::DYNAMIC, false, PostProcessingEffectMode::DirectionalShadowMapping));
    dirShadowMappingUseHDR->SetValue(&m_HDR);
    guiBox->y += guiBox->height;
    
    CSlider *dirShadowMappinglightExposure = (CSlider *)AddControl(new CSlider("Light Exposure", 0.0f, 1.0f, 5,
                                                                                guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::DirectionalShadowMapping));
    dirShadowMappinglightExposure->SetValue(&m_exposure);
    guiBox->y += guiBox->height;
    
    CSlider *dirShadowMappingGamma = (CSlider *)AddControl(new CSlider("Gamma Correction", 0.0f, 3.0f, 5,
                                                                        guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::DirectionalShadowMapping));
    dirShadowMappingGamma->SetValue(&m_gama);
    guiBox->y += guiBox->height;
    
    CButton * dirShadowMappingFromLight = (CButton *)AddControl(new CButton("From Light View", guiBox,
                                                                        GUIMode::DYNAMIC, false, PostProcessingEffectMode::DirectionalShadowMapping));
    dirShadowMappingFromLight->SetValue(&m_fromLightPosition);
    guiBox->y += guiBox->height;
    
    CSlider *dirShadowMappingBias = (CSlider *)AddControl(new CSlider("Bias", 0.0f, 0.2f, 5, guiBox,
                                                                       GUIMode::DYNAMIC, false, PostProcessingEffectMode::DirectionalShadowMapping));
    dirShadowMappingBias->SetValue(&m_dirShadowBias);
    
    // Omnidirectional Shadow Mapping
    guiBox->y = rightStartingY + ppfxY + guiBox->height + spaceAtCoverage;
    CButton * omniShadowMappingUseHDR = (CButton *)AddControl(new CButton("Use HDR Light", guiBox,
                                                            GUIMode::DYNAMIC, false, PostProcessingEffectMode::OmnidirectionalShadowMapping));
    omniShadowMappingUseHDR->SetValue(&m_HDR);
    guiBox->y += guiBox->height;
    
    CSlider *omniShadowMappinglightExposure = (CSlider *)AddControl(new CSlider("Light Exposure", 0.0f, 1.0f, 5,
                                                                  guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::OmnidirectionalShadowMapping));
    omniShadowMappinglightExposure->SetValue(&m_exposure);
    guiBox->y += guiBox->height;
    
    CSlider *omniShadowMappingGamma = (CSlider *)AddControl(new CSlider("Gamma Correction", 0.0f, 3.0f, 5,
                                                          guiBox, GUIMode::DYNAMIC, false, PostProcessingEffectMode::OmnidirectionalShadowMapping));
    omniShadowMappingGamma->SetValue(&m_gama);
    guiBox->y += guiBox->height;
    
    CButton * omniShadowMappingShowDepth = (CButton *)AddControl(new CButton("Show Depth", guiBox,
                                                                         GUIMode::DYNAMIC, false, PostProcessingEffectMode::OmnidirectionalShadowMapping));
    omniShadowMappingShowDepth->SetValue(&m_showDepth);
    guiBox->y += guiBox->height;
    
    CSlider *omniShadowMappingBias = (CSlider *)AddControl(new CSlider("Bias", 0.0f, 2.0f, 5, guiBox,
                                                           GUIMode::DYNAMIC, false, PostProcessingEffectMode::OmnidirectionalShadowMapping));
    omniShadowMappingBias->SetValue(&m_orthShadowBias);
    
}

void Game::RenderControls() {
    
     
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        CControl * control = (*it);
        if (m_currentPPFXMode == control->GetPostProcessingEffectMode()) {
            
            // // Create dynamic GUI objects
            switch(control->GetGUIType()) {
                case GUIType::BUTTON: {
                    CButton * button = (CButton *)(*it);
                    if (button->GetIsActive() == false && button->GetGUIMode() == GUIMode::DYNAMIC) {
                        button->Create();
                    }
                    break;
                }
                case GUIType::LISTBOX: {
                    CListBox * listbox = (CListBox *)(*it);
                    if (listbox->GetIsActive() == false && listbox->GetGUIMode() == GUIMode::DYNAMIC) {
                        listbox->Create();
                    }
                    break;
                }
                case GUIType::SLIDER: {
                    CSlider * slider = (CSlider *)(*it);
                    if (slider->GetIsActive() == false && slider->GetGUIMode() == GUIMode::DYNAMIC) {
                        slider->Create();
                    }
                    break;
                }
            }
            
        } else {
            if (control->GetGUIMode() == GUIMode::DYNAMIC) {
                control->SetIsActive(false);
                control->SetIsInside(false);
            }
        }
    }
    
    
    CShaderProgram *hudProgram = (*m_pShaderPrograms)[0];
    SetMaterialUniform(hudProgram, "material", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);          // Type Of Blending To Perform
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);                   // disable texture mapping
    glShadeModel(GL_SMOOTH);
    
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        
        CControl * control = (*it);
        
        /// Render GUI objects
        if (control->GetIsActive() == true) {
            if(m_enableHud) {
                control->Render(m_pFtFont, hudProgram, "material",
                            m_textColor, m_guiHighlightedColor, m_guiColor, m_guiBackgroundColor);
            }
            
            if ( (controlled != nullptr && controlled != control) || mState.m_isNavigating) {
                // our current controll is being controlled
                continue;
            }
            
            if (control->Update(mState)) {
                // Handle events of this control
                controlled = control;
                
            } else if (control == controlled) {
                // this *it control no longer has control
                controlled = nullptr;
            } else {
                // we do ot need to handle events, because control has been updated...but no messages to handle
            }
        }
        
    }
    glDisable(GL_BLEND);                // Re-Disable Blending
    glEnable(GL_DEPTH_TEST);            // Re-Enable Depth Testing
    glEnable(GL_TEXTURE_2D);            // Re-Enable Texture Mapping
    
     
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        CControl * control = (*it);
        if (m_currentPPFXMode == control->GetPostProcessingEffectMode()) {
            // Clear dynamic GUI objects
            switch(control->GetGUIType()) {
                case GUIType::BUTTON: {
                    CButton * button = (CButton *)(*it);
                    if (button->GetIsActive() == true && button->GetGUIMode() == GUIMode::DYNAMIC) {
                        button->Clear();
                    }
                    break;
                }
                case GUIType::LISTBOX: {
                    CListBox * listbox = (CListBox *)(*it);
                    if (listbox->GetIsActive() == true && listbox->GetGUIMode() == GUIMode::DYNAMIC) {
                        listbox->Clear();
                    }
                    break;
                }
                case GUIType::SLIDER: {
                    CSlider * slider = (CSlider *)(*it);
                    if (slider->GetIsActive() == true && slider->GetGUIMode() == GUIMode::DYNAMIC) {
                        slider->Clear();
                    }
                    break;
                }
            }
        }
    }
    
}

void Game::UpdateControls() {
 
    UpdateMouseControls(mState);
    UpdateKeyBoardControls(kState);
    
    // update camera
    UpdateCamera(m_deltaTime, mState, kState, mState.m_isEnabled);
    
}

void Game::ClearControls() {
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        CControl * control = (*it);
        
        if (control->GetIsActive() == false) {
            control->Clear();
        }
    }
}

void Game::RemoveControls() {
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        delete (*it);
        it = CControl::m_controls.begin();
    }
}

// glfw: whenever the mouse enters and leaves window, this callback is called
static void OnMouseEnter_callback(GLFWwindow* window, int entered) {
    /*
    if (entered) {
        std::cout << " Entered Window " << std::endl;
    } else {
        std::cout << " Exited Window " << std::endl;
    }
    */
}

// glfw: whenever a mouse button is pressed, this callback is called
void OnMouseDown_callback(GLFWwindow* window, int button, int action, int mods) {
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mState.m_leftButtonDown = ControlType::LEFTMOUSE;
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mState.m_leftButtonDown = ControlType::UNKNOWN;
    }
    
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        mState.m_middleButtonDown = ControlType::MIDDLEMOUSE;
    }
    
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
        mState.m_middleButtonDown = ControlType::UNKNOWN;
    }
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mState.m_rightButtonDown = ControlType::RIGHTMOUSE;
    }
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        mState.m_rightButtonDown = ControlType::UNKNOWN;
    }
}

// glfw: whenever the mouse moves, this callback is called
void OnMouseMove_callback(GLFWwindow* window, double xpos, double ypos)
{
    mState.x = xpos;
    mState.y = ypos;
    
    if (mState.m_leftButtonDown != ControlType::UNKNOWN)
    {
        mState.m_isDragging = true;
    }
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void OnMouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/11.anti_aliasing_offscreen/anti_aliasing_offscreen.cpp
    mState.m_scroll = yoffset;
}

void Game::UpdateMouseControls(MouseState &state){
    
    // https://stackoverflow.com/questions/37194845/using-glfw-to-capture-mouse-dragging-c
    // https://stackoverflow.com/questions/45130391/opengl-get-cursor-coordinate-on-mouse-click-in-c
    
    m_mouseX = state.x;
    m_mouseY = state.y;
    m_mouseButtonDown = state.m_leftButtonDown == ControlType::LEFTMOUSE;
    
    // reset the scroll value
    static GLint lastScroll = -1;
    if (lastScroll == -1)
    {
        lastScroll = state.m_scroll;
    }
    
    if ((GLint)state.m_scroll == lastScroll) {
        state.m_scroll = 0;
    }
    
    lastScroll = state.m_scroll;
    
    //// https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/11.anti_aliasing_offscreen/anti_aliasing_offscreen.cpp
    /// https://www.3dbuzz.com/training/view/opengl-in-depth/particle-engine/viewport-navigation
    static GLint lastX = -1;
    static GLint lastY = -1;
    
    // this is initially true, at the beginning
    if (lastX == -1 && lastY == -1)
    {
        lastX = state.x;
        lastY = state.y;
    }
    
    if (state.m_leftButtonDown == ControlType::UNKNOWN) {
        state.m_isDragging = false;
    }
    
    GLdouble xoffset = state.x - (GLdouble)lastX;
    GLdouble yoffset = (GLdouble)lastY - state.y; // reversed since y-coordinates range from bottom to top
    
    lastX = state.x;
    lastY = state.y;
    
    
    // Check if we are inside a control
    GLboolean isInsideAControl = false;
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        CControl * control = (*it);
        if (control->GetIsInside()) {
            isInsideAControl = true;
            break;
        }
    }
    
    // check to see of the left button is down and make sure that we are not controlling a control
    if(state.m_leftButtonDown == ControlType::LEFTMOUSE && isInsideAControl == false && state.m_isDragging && controlled == nullptr) {
        state.xoffset = xoffset;
        state.yoffset = yoffset;
        state.m_isNavigating = true;
    } else {
        state.m_isNavigating = false;
    }
    
    /*
    std::cout
    << " mouse down " << ControlTypeToString(state.m_leftButtonDown)
    << ", mouse is enabled " <<  Extensions::BoolToString(state.m_isEnabled)
    << ", mouse navigating " << Extensions::BoolToString(state.m_isNavigating)
    << ", mouse is dragging " <<  Extensions::BoolToString(state.m_isDragging)
    << ", mouse is inside a Control " <<  Extensions::BoolToString(isInsideAControl)
    << ", mouse x " << (state.x)
    << ", mouse y " << (state.y)
    << std::endl;
    */
}

// glfw: whenever the keyboard is pressed, this callback is called
void OnKeyDown_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action) {
        case GLFW_PRESS:
            kState.m_keyPressed = key;
            
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                    break;
                case GLFW_KEY_UP:
                    kState.m_arrowKeyDown = ControlType::KEYFORWARD;
                    break;
                case GLFW_KEY_DOWN:
                    kState.m_arrowKeyDown = ControlType::KEYBACKWARD;
                    break;
                case GLFW_KEY_LEFT:
                    kState.m_arrowKeyDown = ControlType::KEYLEFT;
                    break;
                case GLFW_KEY_RIGHT:
                    kState.m_arrowKeyDown = ControlType::KEYRIGHT;
                    break;
                default:
                    break;
            }
            break;
        case GLFW_RELEASE:
            kState.m_keyReleased = key;
            kState.m_keyPressed = -1;
            kState.m_arrowKeyDown = ControlType::UNKNOWN;
            break;
        default:
            break;
    }
}

void Game::UpdateKeyBoardControls(KeyboardState &state) {

    if (state.m_keyPressed == -1){
        state.m_keyPressTime = 0.0;
        state.m_lastKeyPressTime = -1;
        state.m_keyReleased = -1;
    }
    
    if (state.m_keyPressed != -1) {
        state.m_keyPressTime += m_timePerSecond;
        
        if ((int)state.m_lastKeyPressTime  == (int)state.m_keyPressTime)
        {
            return;
        }
        
        switch (state.m_keyPressed)
        {
            case GLFW_KEY_SPACE:
                m_enableHud = !m_enableHud;
                break;
            case GLFW_KEY_U:
                m_audioNumber = (m_audioNumber + 1) % m_audioFiles.size();
                m_changeAudio = true;
                break;
            case GLFW_KEY_I:
                m_pointLightIndex = (m_pointLightIndex + 1) % m_pointLights.size();
                break;
            case GLFW_KEY_O:
                state.m_isSlowMotion = !state.m_isSlowMotion;
                break;
            case GLFW_KEY_Q:
                std::get<0>(m_pointLights[m_pointLightIndex]).y += 25.0f;
                break;
            case GLFW_KEY_A:
                std::get<0>(m_pointLights[m_pointLightIndex]).y -= 25.0f;
                break;
            case GLFW_KEY_W:
                std::get<0>(m_pointLights[m_pointLightIndex]).x -= 25.0f;
                break;
            case GLFW_KEY_E:
                std::get<0>(m_pointLights[m_pointLightIndex]).x += 25.0f;
                break;
            case GLFW_KEY_S:
                std::get<0>(m_pointLights[m_pointLightIndex]).z -= 25.0f;
                break;
            case GLFW_KEY_D:
                std::get<0>(m_pointLights[m_pointLightIndex]).z += 25.0f;
                break;
            default:
                break;
        }
        
        state.m_lastKeyPressTime = state.m_keyPressTime;
    }

/*
    std::cout
    << "Keyboard,  "
    << ", time per second: " << m_timePerSecond
    << ", time in seconds: " << m_timeInSeconds
    << ", key pressed: " << state.m_keyPressed
    << ", key released: " << state.m_keyReleased
    << ", Mouse Move ClickSwitch " << Extensions::BoolToString(mState.m_isEnabled)
    << std::endl;
   */
}
