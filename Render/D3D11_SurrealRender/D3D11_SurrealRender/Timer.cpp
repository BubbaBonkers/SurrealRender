#include "Timer.h"

// Constructor.
Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

// Get time elapsed in miliseconds since the last tick.  
double Timer::GetElapsedMiliseconds()
{
	if (bActivated)
	{
		auto Elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return Elapsed.count();
	}
	else
	{
		auto Elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return Elapsed.count();
	}
}

// Restart the Timer object's count.
void Timer::Restart()
{
	bActivated = true;
	start = std::chrono::high_resolution_clock::now();
}

// Start the Timer object's count.
bool Timer::Start()
{
	if (bActivated)
	{
		return false;
	}
	else
	{
		start = std::chrono::high_resolution_clock::now();
		bActivated = true;
		return true;
	}
}

// Stop the Timer object's count.
bool Timer::Stop()
{
	if (bActivated)
	{
		return false;
	}
	else
	{
		stop = std::chrono::high_resolution_clock::now();
		bActivated = false;
		return true;
	}
}
