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
    AddControl(new CButton("Run Away!", 20, 300, 200, 200, 50));
    CListBox * listNames = (CListBox *)AddControl(new CListBox(20, 50, 300, 300, 200, 200));
    listNames->AddItem("First Item");
    listNames->AddItem("Second Item");
    listNames->AddItem("Third Item");
    listNames->AddItem("Fourth Item");
    
    CSlider *slider = (CSlider *)AddControl(new CSlider("The Great One!",
                                                       20, 0.0f, 100.0f, 5, 300, 520, 200, 50));
    slider->SetValue(&m_coverage);
    
    m_gameWindow->SetInputs(OnMouseEnter_callback, OnMouseMove_callback,
                            OnMouseDown_callback, OnMouseScroll_callback,
                            OnKeyDown_callback);
}

void Game::UpdateControls() {
    
    //UpdateMouseControls(mouseButton, mouseAction);
    //UpdateKeyBoardControls(keyPressedCode, keyReleasedCode, keyPressedAction);
    
    /// Update mouse
    glfwGetCursorPos(m_gameWindow->GetWindow(), &mState.x, &mState.y);
    
    
    /// Update Keyboard
    if (kState.m_keyAction == GLFW_RELEASE){
       kState.m_keyDown = -1;
    }
    
    if (kState.m_keyDown == -1){
        
        kState.m_keyReleased = -1;
    }
    
    if (kState.m_keyDown  != -1){
        
    }
}


void Game::RenderControls(CFreeTypeFont *font, CShaderProgram *fontProgram, const std::string &material) {
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        
        CControl * control = (*it);
        control->Render(font, fontProgram, material);
        
        if (controlled != nullptr && controlled != control) {
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

void Game::RemoveControls() {
    for (std::list<CControl *>::iterator it = CControl::m_controls.begin(); it != CControl::m_controls.end(); it++) {
        delete (*it);
        it = CControl::m_controls.begin();
    }
}

// glfw: whenever the mouse enters and leaves window, this callback is called
static void OnMouseEnter_callback(GLFWwindow* window, int entered) {
    if (entered) {
        std::cout << " Entered Window " << std::endl;
    } else {
        std::cout << " Exited Window " << std::endl;
    }
}

// glfw: whenever the mouse moves, this callback is called
void OnMouseMove_callback(GLFWwindow* window, double xpos, double ypos)
{
    /*
    // https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/11.anti_aliasing_offscreen/anti_aliasing_offscreen.cpp
    if (firstMouse)
    {
        mouseLastX = xpos;
        mouseLastY = ypos;
        firstMouse = false;
    }
    
    mouseXoffset = xpos - mouseLastX;
    mouseYoffset = mouseLastY - ypos; // reversed since y-coordinates go from bottom to top
    
    mouseLastX = xpos;
    mouseLastY = ypos;
    */
    std::cout
    << "Mouse Position,  "
    << " posX: " << xpos
    << " posY: " << ypos
    << std::endl;
    
}

// glfw: whenever a mouse button is pressed, this callback is called
void OnMouseDown_callback(GLFWwindow* window, int button, int action, int mods){
    /*
    mouseButton = button;
    mouseAction = action;
    */
    
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
    
    std::cout
    << "Mouse Down with,  "
    << " button: " << button
    << " action: " << action
    << " mode: " << mods
    << " state posX: " << mState.x
    << " state posY: " << mState.y
    //<< " state left down: " << mState.m_leftButtonDown
    //<< " state middle down: " << mState.m_middleButtonDown
    //<< " state right down: " << mState.m_rightButtonDown
    << std::endl;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void OnMouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/11.anti_aliasing_offscreen/anti_aliasing_offscreen.cpp
    std::cout
    << "Mouse Scroll,  "
    << " posX: " << xoffset
    << " posY: " << yoffset
    << std::endl;
    
}

// glfw: whenever the keyboard is pressed, this callback is called
void OnKeyDown_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout
    << "Keyboard presses"
    << " key: " << key
    << " scan code: " << scancode
    << " action: " << action
    << " mode: " << mods
    << std::endl;
    
    kState.m_keyAction = action;
    keyPressedAction = action;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    
    switch (action) {
        case GLFW_PRESS:
            kState.m_keyDown = key;
            keyPressedCode = key;
            break;
        case GLFW_RELEASE:
            kState.m_keyReleased = key;
            keyReleasedCode = key;
            break;
        default:
            break;
    }
    
}

void Game::UpdateMouseControls(const int &button, const int &action){
    
    /*
    // https://stackoverflow.com/questions/37194845/using-glfw-to-capture-mouse-dragging-c
    // https://stackoverflow.com/questions/45130391/opengl-get-cursor-coordinate-on-mouse-click-in-c
    
    // Mouse Move
    if (m_mouseMouseMoveClickSwitch == true) {
        m_enableMouseMovement = false;
        //std::cout << "Mouse x: " << m_mouseX << " and mouse y: " << m_mouseY << std::endl;
    }
    // Mouse click
    if ( m_mouseMouseMoveClickSwitch == false){
        m_enableMouseMovement = true;
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        
        if(GLFW_PRESS == action){
            m_mouseButtonDown = true;
            
        }else if(GLFW_RELEASE == action){
            // Mouse click
            if ( m_mouseMouseMoveClickSwitch == false && m_mouseButtonDown == true){
                m_isKeyPressRestriction = !m_isKeyPressRestriction;
                //glfwGetCursorPos(gameWindow.GetWindow(), &m_mouseX, &m_mouseY);
                //std::cout << "Mouse x: " << m_mouseX << " and mouse y: " << m_mouseY << std::endl;
            }
            
            m_mouseButtonDown = false;
        }
        
    }
    
    if(m_mouseButtonDown) {
        // do your drag here
    }
    glfwGetCursorPos(m_gameWindow->GetWindow(), &m_mouseX, &m_mouseY);
    m_gameWindow->SetCursorVisible(m_isMouseCursorVisible);
     */
    
}

void Game::UpdateKeyBoardControls(int &keyPressed, int &keyReleased, int &keyAction){

    /*
    if (keyAction == GLFW_RELEASE){
        keyPressed = -1;
    }
    
    if (keyPressed == -1){
        m_keyPressTime = 0.0;
        m_lastKeyPressTime = -1;
        keyReleased = -1;
    }
    
    if (keyPressed != -1){
        
        m_keyPressTime += 0.06;
        
        //m_lastKeyPress == keyPressedCode ||
        if ((int)m_lastKeyPressTime  == (int)m_keyPressTime && m_isKeyPressRestriction == true)
        {
            return;
        }
        
        switch (keyPressed)
        {
            case GLFW_KEY_WORLD_1:
                m_HDR = !m_HDR;
               break;
            case GLFW_KEY_1 :   
                m_materialShininess -= 5.0f;
                break;
            case GLFW_KEY_2:
                m_materialShininess += 5.0f;
                break;
            case GLFW_KEY_3:
                m_dirIntensity -= 0.2f;
                break;
            case GLFW_KEY_4:
                m_dirIntensity += 0.2f;
                break;
            case GLFW_KEY_5:
                m_pointIntensity -= 2.0f;
                break;
            case GLFW_KEY_6:
                m_pointIntensity += 2.0f;
                break;
            case GLFW_KEY_7:
                m_spotIntensity -= 2.0f;
                break;
            case GLFW_KEY_8: 
                m_spotIntensity += 2.0f;
                break;
            case GLFW_KEY_9:
                m_woodenBoxesUseTexture = !m_woodenBoxesUseTexture;
                break;
            case GLFW_KEY_0:
                m_useDir = !m_useDir;
                break;
            case GLFW_KEY_MINUS:
                m_usePoint = !m_usePoint;
                break;
            case GLFW_KEY_EQUAL:
                m_useSpot = !m_useSpot;
                break;
            case GLFW_KEY_Q:
                m_useSmoothSpot = !m_useSmoothSpot;
                break;
            case GLFW_KEY_W:
                m_useBlinn = !m_useBlinn;
                break;
            case GLFW_KEY_E:
                m_ambient -= 0.2f;
                break;
            case GLFW_KEY_R:
                m_ambient += 0.2f;
                break;
            case GLFW_KEY_T:
                m_diffuse -= 0.2f;
                break;
            case GLFW_KEY_Y:
                m_diffuse += 0.2f;
                break;
            case GLFW_KEY_U:
                m_specular -= 0.2f;
                break;
            case GLFW_KEY_I:
                m_specular += 0.2f;
                break;
            case GLFW_KEY_O:
                m_spotCutOff -= 0.2f;
                break;
            case GLFW_KEY_P:
                m_spotCutOff += 0.2f;
                break;
            case GLFW_KEY_LEFT_BRACKET:
                m_spotOuterCutOff -= 0.2f;
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                m_spotOuterCutOff += 0.2f;
                break;
            case GLFW_KEY_A:
                m_coverage -= 0.1f;
                m_coverage = glm::clamp(m_coverage, 0.0f, 1.0f);
                break;
            case GLFW_KEY_S:
                m_coverage += 0.1f;
                m_coverage = glm::clamp(m_coverage, 0.0f, 1.0f);
                break;
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
            case GLFW_KEY_J:
                
                break;
            case GLFW_KEY_K:
                
                break;
            case GLFW_KEY_L:
                m_isMouseCursorVisible = !m_isMouseCursorVisible;
                break;
            case GLFW_KEY_SEMICOLON:
                m_useRefraction = !m_useRefraction;
                break;
            case GLFW_KEY_APOSTROPHE:
                m_exposure += 0.1f;
                if (m_exposure > 1.000001f) m_exposure = 0.0f;
                break;
            case GLFW_KEY_BACKSLASH:
                m_gama += 0.2f;
                if (m_gama > 5.0f) m_gama = 0.0f;
                break;
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
            case GLFW_KEY_X:
                m_magnitude -= 0.1f;
                break;
            case GLFW_KEY_C:
                m_magnitude += 0.1f;
                break;
            case GLFW_KEY_V:
                m_uvTiling -= 0.1f;
                break;
            case GLFW_KEY_B:
                m_uvTiling += 0.1f;
                break;
            case GLFW_KEY_N:
                m_parallaxHeightScale -= 0.1f;
                break;
            case GLFW_KEY_M:
                m_parallaxHeightScale += 0.1f;
                break;
            case GLFW_KEY_COMMA:
                m_skyboxNumber = (m_skyboxNumber + 1) % m_pSkybox->GetNumberOfSkyboxes();
                m_changeSkybox = true;
                break;
            case GLFW_KEY_PERIOD:
                m_audioNumber = (m_audioNumber + 1) % 5;
                m_changeAudio = true;
                break;
            case GLFW_KEY_SLASH:
                m_enableHud = !m_enableHud;
            case GLFW_KEY_SPACE:
                m_mouseMouseMoveClickSwitch = !m_mouseMouseMoveClickSwitch;
                break;
            default:
                break;
        }
        
        //std::cout << " keypresstime: " << (int)m_keyPressTime << ", lastkeypresstime: " << (int)m_lastKeyPressTime << std::endl;
        //std::cout << " keypress: " << keyPressedCode << ", lastkeypress: " << m_lastKeyPress << std::endl;
        
        m_lastKeyPressTime = m_keyPressTime;
        m_lastKeyPress = keyPressed;
    }
    */
    
}
