#pragma once

#include <ray/math/Scalar.hpp>

namespace ray { namespace math {

    template<template<typename U> class Vector, typename S>
    struct Rectangle
    {
        using vector    = Vector<S>;
        using scalar    = Scalar<S>;
        using rectangle = Rectangle<Vector,S>;
        
        vector min, max;
    
        constexpr auto operator+=(const vector &shift)  { min+=shift; max+=shift; return (*this); }
        constexpr auto operator-=(const vector &shift)  { min-=shift; max-=shift; return (*this); }
        constexpr auto operator*=(const vector &factor) { max = min + size()*factor; return (*this); }
        constexpr auto operator/=(const vector &factor) { max = min + size()/factor; return (*this); }
        constexpr auto operator*=(const scalar &factor) { max = min + size()*factor; return (*this); }
        constexpr auto operator/=(const scalar &factor) { max = min + size()/factor; return (*this); }
        
        vector size()   const { return max - min; }
    };

    // TODO(cme): test me
    template<typename T, template<typename U> class Vector, typename F> constexpr auto rect_cast(const Rectangle<Vector,F> &r) { return Rectangle<Vector,T>{ vector_cast<T>(r.min), vector_cast<T>(r.max) }; }
        
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator+(const Rectangle<Vector,S1> &r, const Vector<S2> &v) { return Rectangle<Vector, widest<S1,S2>>{ r.min+v, r.max+v}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator-(const Rectangle<Vector,S1> &r, const Vector<S2> &v) { return Rectangle<Vector, widest<S1,S2>>{ r.min-v, r.max-v}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator+(const Vector<S2> &v, const Rectangle<Vector,S1> &r) { return Rectangle<Vector, widest<S1,S2>>{ r.min+v, r.max+v}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator-(const Vector<S2> &v, const Rectangle<Vector,S1> &r) { return Rectangle<Vector, widest<S1,S2>>{ r.min-v, r.max-v}; }
    
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator*(const Rectangle<Vector,S1> &a, const Scalar<S2> &b) { return Rectangle<Vector,widest<S1,S2>>{ S2(1)*a.min,a.min+a.size()*b}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator/(const Rectangle<Vector,S1> &a, const Scalar<S2> &b) { return Rectangle<Vector,widest<S1,S2>>{ S2(1)*a.min,a.min+a.size()/b}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator*(const Scalar<S2> &b, const Rectangle<Vector,S1> &a) { return Rectangle<Vector,widest<S1,S2>>{ S2(1)*a.min,a.min+b*a.size()}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator/(const Scalar<S2> &b, const Rectangle<Vector,S1> &a) { return Rectangle<Vector,widest<S1,S2>>{ S2(1)*a.min,a.min+b/a.size()}; }
    
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator*(const Rectangle<Vector,S1> &r, const Vector<S2> &v) { return Rectangle<Vector, widest<S1,S2>>{ S2(1)*r.min, r.min+r.size()*v}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator/(const Rectangle<Vector,S1> &r, const Vector<S2> &v) { return Rectangle<Vector, widest<S1,S2>>{ S2(1)*r.min, r.min+r.size()/v}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator*(const Vector<S2> &v, const Rectangle<Vector,S1> &r) { return Rectangle<Vector, widest<S1,S2>>{ S2(1)*r.min, r.min+v*r.size()}; }
    template<template<typename U> class Vector, typename S1, typename S2> constexpr auto operator/(const Vector<S2> &v, const Rectangle<Vector,S1> &r) { return Rectangle<Vector, widest<S1,S2>>{ S2(1)*r.min, r.min+v/r.size()}; }
    
    template<template<typename U> class Vector, typename S1> auto &operator<<(std::ostream &out, const Rectangle<Vector,S1> &r) { return (out << "[" << r.min << "->" << r.max << "]"); }

    template<typename S1, typename S2, template<typename U> class Vector> constexpr auto widen_cast(const Rectangle<Vector,S1> &source) { return Rectangle<Vector,widest<S1,S2>>{ widest<S1,S2>(source.min), widest<S1,S2>(source.min) }; }
}}