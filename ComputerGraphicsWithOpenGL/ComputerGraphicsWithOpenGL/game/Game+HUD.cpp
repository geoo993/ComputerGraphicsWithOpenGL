//
//  Game+HUD.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::RenderHUD(){
    
    CShaderProgram *fontProgram = (*m_pShaderPrograms)[0];
    
    // Use the font shader program and render the text
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    fontProgram->UseProgram();
    fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
    fontProgram->SetUniform("textColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    DisplayFrameRate(m_enableHud, fontProgram);
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
}

void Game::DisplayFrameRate( bool enableHud, CShaderProgram *fontProgram)
{

    int width = m_gameWindow.GetWidth();
    int height = m_gameWindow.GetHeight();
    
    // Increase the elapsed time and frame counter
    m_elapsedTime += m_deltaTime;
    ++m_frameCount;
    
   // m_timeInSeconds += (float) (0.01f * m_deltaTime);
    m_timePerSecond = (float)(m_deltaTime / 1000.0f);
    m_timeInSeconds += m_timePerSecond;
    m_timeInMilliSeconds += (float) (m_deltaTime);
    
    // Now we want to subtract the current time by the last time that was stored
    // to see if the time elapsed has been over a second, which means we found our FPS.
    if (m_elapsedTime > 1000)
    {
        m_elapsedTime = 0;
        m_framesPerSecond = m_frameCount;
        
        // Reset the frames per second
        m_frameCount = 0;
    }
    
    /*
     std::cout << std::endl;
     std::cout << "deltatime: " <<m_deltaTime << std::endl;
     std::cout << "elapsedTime: " << m_elapsedTime<< std::endl;
     std::cout << "time: " << m_timeInSeconds << std::endl;
     std::cout << "glfw getTime: " << glfwGetTime() << std::endl;
     
     float time = (float)m_elapsedTime / 1000.0f * 2.0f * 3.14159f * 0.75f;
     std::cout << "timeFromElapsed: " << time << std::endl;
     */
    
    
    if (m_framesPerSecond > 0) {
        
        if (enableHud) {
            m_pFtFont->Render(fontProgram, 20, height - 20, 20, "FPS: %d", m_framesPerSecond);
        }
        
    }
    
}
