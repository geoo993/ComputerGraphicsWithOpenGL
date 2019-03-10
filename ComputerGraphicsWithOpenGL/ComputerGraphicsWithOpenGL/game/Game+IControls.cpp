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

void Game::LoadControls() {
    
    m_gameWindow->SetInputs(OnMouseEnter_callback, OnMouseMove_callback,
                            OnMouseDown_callback, OnMouseScroll_callback,
                            OnKeyDown_callback);
    
    /// LEFT SCREEN
    GLint boxXLeft = 10;
    GLint boxYLeft = 10;
    GLint boxWidth = 200;
    GLint boxHeight = 30;
    GLint textSize = 15;
    
    CButton * useDir = (CButton *)AddControl(new CButton("Use Diriectional Light", textSize, boxXLeft, boxYLeft, boxWidth, boxHeight));
    useDir->SetValue(&m_useDir);
    boxYLeft += boxHeight;
    
    CButton * usePoint = (CButton *)AddControl(new CButton("Use Point Light", textSize, boxXLeft, boxYLeft, boxWidth, boxHeight));
    usePoint->SetValue(&m_usePoint);
    boxYLeft += boxHeight;
    
    CButton * useSpot = (CButton *)AddControl(new CButton("Use Spot Light", textSize, boxXLeft, boxYLeft, boxWidth, boxHeight));
    useSpot->SetValue(&m_useSpot);
    boxYLeft += boxHeight;
    
    CButton * useSmoothSpot = (CButton *)AddControl(new CButton("Use Smooth Spot Light", textSize, boxXLeft, boxYLeft, boxWidth, boxHeight));
    useSmoothSpot->SetValue(&m_useSmoothSpot);
    boxYLeft += boxHeight;
    
    CButton * useBlinn = (CButton *)AddControl(new CButton("Use Blinn Light", textSize, boxXLeft, boxYLeft, boxWidth, boxHeight));
    useBlinn->SetValue(&m_useBlinn);
    boxYLeft += boxHeight;
    
    CButton * useHDR = (CButton *)AddControl(new CButton("Use HDR Light", textSize, boxXLeft, boxYLeft, boxWidth, boxHeight));
    useHDR->SetValue(&m_HDR);
    boxYLeft += boxHeight;
    
    CSlider *dirIntensity = (CSlider *)AddControl(new CSlider("Dir Light Intensity", textSize, 0.0f, 2.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    dirIntensity->SetValue(&m_dirIntensity);
    boxYLeft += boxHeight;
    
    CSlider *pointIntensity = (CSlider *)AddControl(new CSlider("Point Light Intensity", textSize, 0.0f, 100.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    pointIntensity->SetValue(&m_pointIntensity);
    boxYLeft += boxHeight;
    
    CSlider *spotIntensity = (CSlider *)AddControl(new CSlider("Spot Light Intensity", textSize, 0.0f, 100.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    spotIntensity->SetValue(&m_spotIntensity);
    boxYLeft += boxHeight;
    
    CSlider *spotCutoff = (CSlider *)AddControl(new CSlider("Spot Light Cutoff", textSize, 0.0f, 50.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    spotCutoff->SetValue(&m_spotCutOff);
    boxYLeft += boxHeight;
    
    CSlider *spotOuterCutoff = (CSlider *)AddControl(new CSlider("Spot Light Outer Cutoff", textSize, 0.0f, 50.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    spotOuterCutoff->SetValue(&m_spotOuterCutOff);
    boxYLeft += boxHeight + 5;

    CSlider *ambientLight = (CSlider *)AddControl(new CSlider("Ambient Intensity", textSize, 0.0f, 5.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    ambientLight->SetValue(&m_ambient);
    boxYLeft += boxHeight;
    
    CSlider *diffuseLight = (CSlider *)AddControl(new CSlider("Diffuse Intensity", textSize, 0.0f, 10.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    diffuseLight->SetValue(&m_diffuse);
    boxYLeft += boxHeight;
    
    CSlider *specularLight = (CSlider *)AddControl(new CSlider("Specular Intensity", textSize, 0.0f, 10.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    specularLight->SetValue(&m_specular);
    boxYLeft += boxHeight + 5;
    
    CSlider *constantAtt = (CSlider *)AddControl(new CSlider("Attenuation Constant", textSize, 0.0f, 1.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    constantAtt->SetValue(&m_constant);
    boxYLeft += boxHeight;
    
    CSlider *linearAtt = (CSlider *)AddControl(new CSlider("Attenuation Linear", textSize, 0.0f, 0.1f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    linearAtt->SetValue(&m_linear);
    boxYLeft += boxHeight;
    
    CSlider *quadraticAtt = (CSlider *)AddControl(new CSlider("Attenuation Quadratic", textSize, 0.0f, 0.03f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    quadraticAtt->SetValue(&m_exponent);
    boxYLeft += boxHeight;
    
    CSlider *lightExposure = (CSlider *)AddControl(new CSlider("Light Exposure", textSize, 0.0f, 1.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    lightExposure->SetValue(&m_exposure);
    boxYLeft += boxHeight;
    
    CSlider *lightGamma = (CSlider *)AddControl(new CSlider("Gamma Correction", textSize, 0.0f, 4.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    lightGamma->SetValue(&m_gama);
    boxYLeft += boxHeight + 5;
    
    CButton * matTexture = (CButton *)AddControl(new CButton("Material Texture", textSize, boxXLeft, boxYLeft, boxWidth, boxHeight));
    matTexture->SetValue(&m_woodenBoxesUseTexture);
    boxYLeft += boxHeight;
    
    CSlider *matShiniess = (CSlider *)AddControl(new CSlider("Material Shininess", textSize, 0.0f, 50.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    matShiniess->SetValue(&m_materialShininess);
    boxYLeft += boxHeight;
    
    CSlider *matUVTilling = (CSlider *)AddControl(new CSlider("Material UV Tilling", textSize, 0.0f, 5.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    matUVTilling->SetValue(&m_uvTiling);
    boxYLeft += boxHeight;
    
    CSlider *matParallax = (CSlider *)AddControl(new CSlider("Material Parallax Scale", textSize, 0.0f, 2.0f, 5, boxXLeft, boxYLeft, boxWidth, boxHeight));
    matParallax->SetValue(&m_parallaxHeightScale);
    boxYLeft += boxHeight;
    
    CButton * matRefraction = (CButton *)AddControl(new CButton("Material Refraction", textSize, boxXLeft, boxYLeft, boxWidth, boxHeight));
    matRefraction->SetValue(&m_useRefraction);
    boxYLeft += boxHeight;
    
   
    /// RIGHT SCREEN
    GLint boxXRight = SCREEN_WIDTH - boxWidth - 10;
    GLint boxYRight = 10;
    CSlider *coverage = (CSlider *)AddControl(new CSlider("PPFX Coverage", textSize, 0.0f, 1.0f, 5, boxXRight, boxYRight, boxWidth, boxHeight));
    coverage->SetValue(&m_coverage);
    boxYRight += boxHeight + 5;
    
    CListBox * skyboxeNames = (CListBox *)AddControl(new CListBox(textSize, boxHeight, boxXRight, boxYRight,
                                                                   boxWidth, boxHeight * m_pSkybox->GetNumberOfSkyboxes() ));
    skyboxeNames->AddItem("Color Basement");
    skyboxeNames->AddItem("Common Room");
    skyboxeNames->AddItem("Dining Room");
    skyboxeNames->AddItem("Dark Land");
    skyboxeNames->AddItem("Valley");
    skyboxeNames->AddItem("Sky Water");
    skyboxeNames->AddItem("Peninsula Tokyo");
    skyboxeNames->AddItem("Petrol Station");
    skyboxeNames->AddItem("Yokohama Night");
    skyboxeNames->AddItem("Yokohama Day");
    skyboxeNames->AddItem("Fog");
    skyboxeNames->SetValue(&m_skyboxNumber, &m_changeSkybox);
     
}

void Game::RenderControls() {
    
    
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
        control->Render(m_pFtFont, hudProgram, "material");
        
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
    glDisable(GL_BLEND);                // Re-Disable Blending
    glEnable(GL_DEPTH_TEST);            // Re-Enable Depth Testing
    glEnable(GL_TEXTURE_2D);            // Re-Enable Texture Mapping
}

void Game::UpdateControls() {
    //    std::cout
    //    << " Light,"
    //    << ", use dir: " << (int)m_useDir
    //    << ", use point: " << (int)m_usePoint
    //    << ", use spot: " << (int)m_useSpot
    //    << std::endl;
    
    UpdateMouseControls(mState);
    UpdateKeyBoardControls(kState);
    
    // update camera
    UpdateCamera(m_deltaTime, mState, kState, mState.m_isEnabled);
    
}

void Game::RemoveControls() {
    /*
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        delete (*it);
        it = CControl::m_controls.begin();
    }
    */
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
void OnMouseDown_callback(GLFWwindow* window, int button, int action, int mods){
 
    
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
    /*
    std::cout
    << "Mouse Down with,  "
    << " button: " << button
    << " action: " << action
    << " mode: " << mods
    << " state posX: " << mState.x
    << " state posY: " << mState.y
    << std::endl;
     */
}

// glfw: whenever the mouse moves, this callback is called
void OnMouseMove_callback(GLFWwindow* window, double xpos, double ypos)
{
    /*
     std::cout
     << "Mouse Position,  "
     << " posX: " << xpos
     << " posY: " << ypos
     << std::endl;
     */
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
    /*
    //https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/11.anti_aliasing_offscreen/anti_aliasing_offscreen.cpp
    std::cout
    << "Mouse Scroll,  "
    << " posX: " << xoffset
    << " posY: " << yoffset
    << std::endl;
    */
  
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
                /*
            case GLFW_KEY_D:
                m_ssaoBias -= 0.1f;
                break;
            case GLFW_KEY_F:
                m_ssaoBias += 0.1f;
                break;
            case GLFW_KEY_G:
                m_ssaoRadius -= 0.5f;
                break;
            case GLFW_KEY_H:
                m_ssaoRadius += 0.5f;
                break;
                 */
            case GLFW_KEY_GRAVE_ACCENT: {
                m_changePPFXMode = true;
                GLint currentIndex = static_cast<GLint>(m_currentPPFXMode);
                GLint numberOfEffects = static_cast<GLint>(PostProcessingEffectMode::NumberOfPPFX);
                GLint nextIndex = (currentIndex - 1) ;
                if (nextIndex < 0)  nextIndex = numberOfEffects - 1;
                m_currentPPFXMode = static_cast<PostProcessingEffectMode>(nextIndex);
            }
                break;
            case GLFW_KEY_Z: {
                m_changePPFXMode = true;
                GLint currentIndex = static_cast<GLint>(m_currentPPFXMode);
                GLint numberOfEffects = static_cast<GLint>(PostProcessingEffectMode::NumberOfPPFX);
                GLint nextIndex = (currentIndex + 1) % numberOfEffects;
                m_currentPPFXMode = static_cast<PostProcessingEffectMode>(nextIndex);
            }
                break;
                /*
            case GLFW_KEY_X:
                m_magnitude -= 0.1f;
                break;
            case GLFW_KEY_C:
                m_magnitude += 0.1f;
                break;
            case GLFW_KEY_PERIOD:
                m_audioNumber = (m_audioNumber + 1) % 5;
                m_changeAudio = true;
                break;
                */
            case GLFW_KEY_SLASH:
                m_enableHud = !m_enableHud;
                break;
            case GLFW_KEY_SPACE:
                mState.m_isEnabled = !mState.m_isEnabled;
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
