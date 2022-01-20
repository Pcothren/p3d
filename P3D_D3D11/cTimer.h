#pragma once

#include <chrono>

namespace mvPlot {

	class cTimer
	{

	public:

		cTimer();

		float mark();
		float peek() const;
		float now() const;
	
	private:

		std::chrono::steady_clock::time_point m_start;
		std::chrono::steady_clock::time_point m_last;
	};
}