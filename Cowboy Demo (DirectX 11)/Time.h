#pragma once

#include <chrono>

class Time
{
public:
	static void Update();
	static double GetDeltaTime() { return m_delta_time; }
	static int GetFPS() { return 1.0 / m_delta_time; }

private:
	static std::chrono::steady_clock::time_point m_current_time;
	static std::chrono::steady_clock::time_point m_previous_time;
	static double m_delta_time;
};