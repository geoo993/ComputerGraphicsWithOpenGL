#pragma once

#include "../TimerBase.h"

class CHighResolutionTimer 
{
public:
	CHighResolutionTimer();
	~CHighResolutionTimer();

	void Start();
	double Elapsed();

private:
    void Release();
    
    timeval m_t1, m_t2;
    double elapsedTime;
	bool m_started;
};
