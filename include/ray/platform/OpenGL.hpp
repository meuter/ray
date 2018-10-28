#pragma once

#include <glad/glad.h>
#include <ray/platform/Panic.hpp>

// NOTE: these seems to be missing from the original glad.h
#define GL_TEXTURE_SWIZZLE_R              0x8E42
#define GL_TEXTURE_SWIZZLE_G              0x8E43
#define GL_TEXTURE_SWIZZLE_B              0x8E44
#define GL_TEXTURE_SWIZZLE_A              0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA           0x8E46


namespace ray { namespace platform {
    
    namespace details 
    {
#ifndef NDEBUG        
        static void checkGlError(const char *file, int line, const char *call)
        {
            auto error = glGetError();
            if (error != GL_NO_ERROR) 
            {
                fprint(std::cerr, "%s:%d: OpenGL error returned '%d' by\n\n\t> gl%s\n\n", file, line, error, call);
                std::abort();
            }
                
        }
#endif        
    }
    
}}

#ifndef NDEBUG
#   define gl(x) do { gl##x; ray::platform::details::checkGlError(__FILE__, __LINE__, #x); } while(false)
#else
#   define gl(x) gl##x;
#endif
