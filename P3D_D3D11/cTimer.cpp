#include "cTimer.h"

namespace mvPlot {

	using namespace std::chrono;

	cTimer::cTimer()
	{
		m_last = steady_clock::now();
		m_start = steady_clock::now();
	}

	float cTimer::mark()
	{
		const auto old = m_last;
		m_last = steady_clock::now();
		const duration<float> frameTime = m_last - old;
		return frameTime.count();
	}

	float cTimer::peek() const
	{
		return duration<float>(steady_clock::now() - m_last).count();
	}

	float cTimer::now() const
	{
		const duration<float> totalTime = steady_clock::now() - m_start;
		return totalTime.count();
	}

}