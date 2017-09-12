#pragma once

#include <chrono>

namespace ray { namespace platform {

    using sec = std::chrono::duration<double>;
	using msec = std::chrono::duration<double, std::milli>;
	using usec = std::chrono::duration<double, std::micro>;
    using nsec = std::chrono::duration<double, std::nano>;
    
    constexpr sec operator""_sec ( unsigned long long int i ) { return sec((double)i); }
    constexpr msec operator""_msec ( unsigned long long int i ) { return msec((double)i); }
    constexpr usec operator""_usec ( unsigned long long int i ) { return usec((double)i); }
    constexpr nsec operator""_nsec ( unsigned long long int i ) { return nsec((double)i); }

}}