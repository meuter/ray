#pragma once

#include <iostream>
#include <ray/platform/Print.hpp>

namespace ray { namespace platform {

    namespace details {

        template<typename ...Args>
        [[ noreturn]] inline void _panic(const std::string &file, int line, const std::string &format, Args&&... args)
        {
            fprint(std::cerr, "%s:%d: ", file, line);
            fprintln(format, std::forward<Args>(args)...);
            std::exit(-1);
        }
        
    }
}}

#ifndef NDEBUG
#   define panic(format, ...)                  ray::platform::details::_panic(__FILE__, __LINE__, format, ##__VA_ARGS__)
#   define panicif(condition, format, ...)     if (condition) ray::platform::details::_panic(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#   define panic(format, ...)
#   define panicif(condition, format, ...)
#endif
