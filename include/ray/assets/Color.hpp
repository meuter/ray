#pragma once

#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace assets {

    struct Color 
    {
        using u8 = math::u8;
        using vec4 = math::vec4;

        constexpr Color(u8 r, u8 g, u8 b, u8 a=0xFF) : r(r), g(g), b(b), a(a) {}
        constexpr Color(const Color &c) = default;
        constexpr Color(const math::vec4 &c) : r(c.r*255), g(c.g*255), b(c.b*255), a(c.a*255) {}
        constexpr Color(u8 v): Color(v,v,v,v) {}
        u8 r,g,b,a;        

        constexpr Color &operator=(const Color &other) = default;        
        constexpr auto operator+=(const Color &other) { r+=other.r; g+=other.g; b+=other.b; a+=other.a; return (*this); }
        constexpr auto operator-=(const Color &other) { r-=other.r; g-=other.g; b-=other.b; a-=other.a; return (*this); }
        constexpr auto operator*=(const Color &other) { r*=other.r; g*=other.g; b*=other.b; a*=other.a; return (*this); }
        constexpr auto operator/=(const Color &other) { r/=other.r; g/=other.g; b/=other.b; a/=other.a; return (*this); }

        template<typename S> constexpr auto operator*=(const math::Scalar<S> &factor)  { r*=factor; g*=factor; b*=factor; a*=factor; return (*this); }
        template<typename S> constexpr auto operator/=(const math::Scalar<S> &divider) { r/=divider; g/=divider; b/=divider; a/=divider; return (*this); }

        operator vec4() const { return vec4(r,g,b,a) / 255.0f; }
    };

    constexpr auto operator+(const Color &a, const Color &b) { return Color(a.r+b.r, a.g+b.g, a.b+b.b, a.a+b.a); }
    constexpr auto operator-(const Color &a, const Color &b) { return Color(a.r-b.r, a.g-b.g, a.b-b.b, a.a-b.a); }
    constexpr auto operator*(const Color &a, const Color &b) { return Color(a.r*b.r, a.g*b.g, a.b*b.b, a.a*b.a); }
    constexpr auto operator/(const Color &a, const Color &b) { return Color(a.r/b.r, a.g/b.g, a.b/b.b, a.a/b.a); }

    template<typename S> constexpr auto operator/(const Color &c, const math::Scalar<S> &d) { return Color(c.r/d, c.g/d, c.b/d, c.a/d); }
    template<typename S> constexpr auto operator*(const Color &c, const math::Scalar<S> &f) { return Color(c.r*f, c.g*f, c.b*f, c.a*f); }
    template<typename S> constexpr auto operator/(const math::Scalar<S> &d, const Color &c) { return Color(d/c.r, d/c.g, d/c.b, d/c.a); }
    template<typename S> constexpr auto operator*(const math::Scalar<S> &f, const Color &c) { return Color(f*c.r, f*c.g, f*c.b, f*c.a); }
    
    constexpr auto operator==(const Color &b, const Color &a) { return (a.r==b.r && a.g==b.g && a.b==b.b && a.a==b.a); }
    constexpr auto operator!=(const Color &b, const Color &a) { return !(a==b); }

    static const auto RED        = Color(0xFF,0x00,0x00,0xFF);
    static const auto GREEN      = Color(0x00,0xFF,0x00,0xFF);
    static const auto BLUE       = Color(0x00,0x00,0xFF,0xFF);
    static const auto YELLOW     = Color(0xFF,0xFF,0x00,0xFF);
    static const auto PINK       = Color(0xFF,0x00,0xFF,0xFF);
    static const auto TEAL       = Color(0x00,0xFF,0xFF,0xFF);

    static const auto DARK_RED    = RED*0.5f;
    static const auto DARK_GREEN  = GREEN*0.5f;
    static const auto DARK_BLUE   = BLUE*0.5f;
    static const auto DARK_YELLOW = YELLOW*0.5f;
    static const auto DARK_PINK   = PINK*0.5f;
    static const auto DARK_TEAL   = TEAL*0.5f;
    
    static const auto BLACK      = Color(0x00,0x00,0x00,0xFF);
    static const auto WHITE      = Color(0xFF,0xFF,0xFF,0xFF);        
    static const auto GRAY       = WHITE*0.5f;
    static const auto LIGHT_GRAY = WHITE*0.25f;
    static const auto DARK_GRAY  = WHITE*0.75f;
    
}}