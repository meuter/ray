#pragma once

#include <ray/platform/Time.hpp>

namespace ray { namespace platform {

	class Stopwatch
	{
        using HighResClock = std::chrono::high_resolution_clock;
        using TimePoint    = std::chrono::time_point<HighResClock>;

	public:	
		Stopwatch() : mStart(HighResClock::now()) {}

		sec lap() 
		{
			auto newTime = HighResClock::now();
			auto result  = newTime - mStart;
			mStart = newTime;
			return result;
		}

	private:
		TimePoint mStart;
	};

}}