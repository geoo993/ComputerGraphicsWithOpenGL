#include "HighResolutionTimer.h"

//https://stackoverflow.com/questions/10866561/cocoa-opengl-unlocked-framerate/10881400#10881400
//http://www.songho.ca/misc/timer/timer.html

CHighResolutionTimer::CHighResolutionTimer() :
m_started(false), elapsedTime(0.0)
{
}

CHighResolutionTimer::~CHighResolutionTimer()
{
    Release();
}

void CHighResolutionTimer::Start()
{
	m_started = true;
    
    // start timer
    gettimeofday(&m_t1, nullptr);
}

double CHighResolutionTimer::Elapsed()
{
	if (!m_started)// if false
		return 0.0;

    // stop timer
    gettimeofday(&m_t2, nullptr);
    
    // compute and print the elapsed time in millisec
    elapsedTime = (m_t2.tv_sec - m_t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (m_t2.tv_usec - m_t1.tv_usec) / 1000.0;   // us to ms
    //std::cout << elapsedTime << " ms.\n";
    
    return elapsedTime;
}

void CHighResolutionTimer::Release() {
    
}
