#pragma once
#include <d3d9.h>

#pragma comment (lib, "winmm.lib")
class Timer
{

public:
	Timer(float FPSLimite)
	{
		m_CurrentTime = 0;
		m_LastTime = 0;
		m_FPSLimite = FPSLimite;
	}
	float during()
	{
		m_CurrentTime = timeGetTime() / 1000.0f;
		float res = m_CurrentTime - m_LastTime;
		float temp = 0.4f / m_FPSLimite;
		if (res >= temp)
		{
			m_LastTime = m_CurrentTime;
			return res;
		}
		return 0;
	}
private:
	float m_LastTime;
	float m_CurrentTime;
	float m_FPSLimite;
};