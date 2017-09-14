#pragma once

#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace assets {

    struct Color 
    {
        using u8 = math::u8;
        using vec4 = math::vec4;

        Color(u8 r, u8 g, u8 b, u8 a=0xFF) : r(r), g(g), b(b), a(a) {}
        Color(const math::vec4 &c) : r(c.r*255), g(c.g*255), b(c.b*255), a(c.a*255) {}
        Color(u8 v): Color(v,v,v,v) {}
        u8 r,g,b,a;
    };

    #define SPREAD_COLOR(c) c.r, c.g, c.b, c.a

    static const auto RED    = Color(0xFF,0x00,0x00,0xFF);
    static const auto GREEN  = Color(0x00,0xFF,0x00,0xFF);
    static const auto BLUE   = Color(0x00,0x00,0xFF,0xFF);
    static const auto YELLOW = Color(0xFF,0xFF,0x00,0xFF);
    static const auto PINK   = Color(0xFF,0x00,0xFF,0xFF);
    static const auto TEAL   = Color(0x00,0xFF,0xFF,0xFF);
    static const auto BLACK  = Color(0x00,0x00,0x00,0xFF);
    static const auto WHITE  = Color(0xFF,0xFF,0xFF,0xFF);        
    
}}