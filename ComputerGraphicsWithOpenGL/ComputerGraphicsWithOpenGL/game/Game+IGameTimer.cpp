//
//  Game+IGameTimer.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 31/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

void Game::UpdateSystemTime() {
    // https://en.cppreference.com/w/cpp/chrono/time_point/time_since_epoch
    // https://stackoverflow.com/questions/15957805/extract-year-month-day-etc-from-stdchronotime-point-in-c
    
    // system time
    typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24> >::type> days;
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration tp = now.time_since_epoch();
    
    days d = std::chrono::duration_cast<days>(tp);
    tp -= d;
    std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(tp);
    tp -= h;
    std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(tp);
    tp -= m;
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(tp);
    tp -= s;
    
    /*
    std::cout << d.count() << "d " << h.count() << ':' << m.count() << ':' << s.count();
    std::cout << " " << tp.count() << std::endl;
    */
    
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm utc_tm = *gmtime(&tt);
    /*
    //tm local_tm = *localtime(&tt);
    std::cout << utc_tm.tm_year + 1900 << '-';
    std::cout << utc_tm.tm_mon + 1 << '-';
    std::cout << utc_tm.tm_mday << ' ';
    std::cout << utc_tm.tm_hour << ':';
    std::cout << utc_tm.tm_min << ':';
    std::cout << utc_tm.tm_sec << '\n' << '\n';
    */
    
    // (year, month, day, time in seconds)
    m_date = glm::vec4((GLfloat)(utc_tm.tm_year + 1900), (GLfloat)(utc_tm.tm_mon + 1), (GLfloat)utc_tm.tm_mday, (GLfloat)tp.count() );
}

void Game::UpdateGameTime() {
    
    // Increase the elapsed time and frame counter
    m_elapsedTime += m_deltaTime;
    ++m_frameCount;
    
    // m_timeInSeconds += (float) (0.01f * m_deltaTime);
    m_timePerSecond = (float)(m_deltaTime / 1000.0f);
    m_timeInSeconds += m_timePerSecond;
    m_timeInMilliSeconds += (float) (m_deltaTime);
    
    m_channelTime = 1.0f; // Time for channel (if video or sound), in seconds
    
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
    std::cout << "deltatime: " << m_deltaTime << std::endl;
    std::cout << "elapsedTime: " << m_elapsedTime<< std::endl;
    std::cout << "time per second: " << m_timePerSecond << std::endl;
    std::cout << "time in seconds: " << m_timeInSeconds << std::endl;
    std::cout << "glfw getTime: " << glfwGetTime() << std::endl;
    std::cout << "frames per second: " << m_framesPerSecond << std::endl;
     
    float time = (float)m_elapsedTime / 1000.0f * 2.0f * 3.14159f * 0.75f;
    std::cout << "timeFromElapsed: " << time << std::endl;
    */
    
}
