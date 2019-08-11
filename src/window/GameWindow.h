#pragma once

#include "../WindowBase.h"

class CGameWindow {
public:
    
    CGameWindow();
    ~CGameWindow();
    
    void CreateWindow(const std::string &appName = "OpenGL Window",
                      const int &w = SCREEN_WIDTH,
                      const int &h = SCREEN_HEIGHT,
                      const bool &fullscreen = false);
    void CreateGameWindow(const std::string &screenTitle);
    bool InitOpenGL();
    bool InitGLFW();
    
    void Set(const std::string &appName, const int &w, const int &h, const bool &fullscreen);
    
    int GetWidth() const { return m_width; }
    int GetHeight()  const { return m_height; }
    GLFWwindow * GetWindow() const { return m_window; }
    
    bool Fullscreen() const { return m_fullscreen; }
    float Ratio() { return (float)m_width / m_height;}
    
    void SetInputs(const GLFWcursorenterfun &cbfunEnter,
                   const GLFWcursorposfun &cbfunMouseMove,
                   const GLFWmousebuttonfun &cbfunMouseClick,
                   const GLFWscrollfun &cbfunMouseScroll,
                   const GLFWkeyfun &cbfunKey);
    void SetCursorVisible( const bool &isVisible );
    void CenterTheWindow();
    void PreRendering();
    void PostRendering();
    void SetViewport();
    void SetViewport(const int & width, const int & height);
    bool ShouldClose();
    void ClearBuffers(const ClearBuffersType &bufferType = ClearBuffersType::COLORDEPTH);
    void SwapBuffers();
    void DestroyWindow();
    
    
private:
    
    CGameWindow(const CGameWindow &other);
    CGameWindow &operator=(const CGameWindow &other);
    
    bool  m_fullscreen;
    
    int m_width, m_height;
    
    GLFWwindow * m_window = nullptr;
    
    std::string m_appName;
    
};


