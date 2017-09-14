#pragma once

#include <ray/math/Vector3.hpp>
#include <ray/math/Quaternion.hpp>
#include <ray/math/Matrix.hpp>
#include <ray/math/Transform.hpp>

namespace ray { namespace math {

    class Transformable 
    {	
    public:
        using vec3 = Vector3<float>;
        using quat = Quaternion<float>;
        using mat4 = Matrix<float,4,4>;

        Transformable() : mScale{1,1,1}, mPosition{0,0,0}, mOrientation{0,0,0,1} {}
        Transformable(const Transformable &other) = default;
        Transformable(Transformable &&other) = default;

        virtual ~Transformable() = default;

        Transformable &operator=(const Transformable &other) = default;
        Transformable &operator=(Transformable &&other) = default;

        vec3 left()            const                                   { return mOrientation.left(); }
        vec3 right()           const                                   { return mOrientation.right(); }
        vec3 up()              const                                   { return mOrientation.up(); }
        vec3 down()            const                                   { return mOrientation.down(); }
        vec3 front()           const                                   { return mOrientation.front(); }
        vec3 back()            const                                   { return mOrientation.back(); }

        vec3 position()        const                                   { return mPosition; }
        vec3 scale()           const                                     { return mScale; }
        quat orientation()     const                                   { return mOrientation; }

        Transformable &move(const vec3 &direction, float amount)       { mPosition += amount * direction; return (*this); } 
        Transformable &moveBy(float x, float y, float z)               { mPosition += vec3{x,y,z}; return (*this); } 
        Transformable &moveTo(const vec3 &position)                    { mPosition = position; return (*this); } 
        Transformable &moveTo(float x, float y, float z)               { mPosition = vec3{x,y,z}; return (*this); } 

        Transformable &rotate(const vec3 axis, const double &angle)    { return rotate(quat(axis, angle));  } 
        Transformable &rotate(const quat &rotation)                    { mOrientation = normalize(rotation * mOrientation); return (*this); } 
        Transformable &rotateTo(const vec3 axis, const double &angle)  { return rotateTo(quat(axis, angle)); } 
        Transformable &rotateTo(const quat &rotation)                  { mOrientation = normalize(rotation); return (*this); } 

        Transformable &scale(float factor)                             { return scale(factor, factor, factor); } 
        Transformable &scale(const vec3 &factors)                      { return scale(factors.x, factors.y, factors.z); } 
        Transformable &scale(float fx, float fy, float fz)             { mScale.x *= fx; mScale.y *= fy; mScale.z *= fz; return (*this); } 
        Transformable &scaleTo(const vec3 &scale)                      { mScale = scale; return (*this); } 
        Transformable &scaleTo(float sx, float sy, float sz)           { mScale = vec3{sx, sy, sz}; return (*this); } 


        mat4 modelMatrix() const { return translationMatrix() * rotationMatrix() * scalingMatrix(); }
        mat4 translationMatrix() const { return translation(mPosition); }
        mat4 scalingMatrix() const { return scaling(mScale); }
        mat4 rotationMatrix() const { return rotation(mOrientation); }

    private:
        vec3 mScale, mPosition;
        quat mOrientation;
    };

}}
