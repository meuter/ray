#pragma once

#include <boost/math/constants/constants.hpp>
#include <cmath>

namespace ray { namespace math {

    using rad = long double;
        
    constexpr const rad PI = boost::math::constants::pi<rad>();
    constexpr const rad PI_OVER_2 = PI / 2.0;
    constexpr const rad PI_OVER_180 = PI / 180.0;

    constexpr rad operator""_deg ( long double s ) { return s*PI_OVER_180; }
    constexpr rad operator""_deg ( unsigned long long int i ) { return (double)(i)*PI_OVER_180;  }

}}