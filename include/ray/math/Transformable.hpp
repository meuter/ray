#pragma once

#include <ray/math/Vector3.hpp>
#include <ray/math/Quaternion.hpp>
#include <ray/math/Matrix.hpp>

namespace ray { namespace math {

    class Transformable 
    {	
    public:
        using vec3 = Vector3<float>;
        using quat = Quaternion<float>;
        using mat4 = Matrix<float,4,4>;

        Transformable() : mScaling{1,1,1}, mPosition{0,0,0}, mRotation{0,0,0,1} {}
        Transformable(const Transformable &other) = default;
        Transformable(Transformable &&other) = default;

        virtual ~Transformable() = default;

        Transformable &operator=(const Transformable &other) = default;
        Transformable &operator=(Transformable &&other) = default;

        vec3 left()            const                                   { return math::rotate( vec3(1,0,0), mRotation); }
        vec3 right()           const                                   { return math::rotate(-vec3(1,0,0), mRotation); }
        vec3 up()              const                                   { return math::rotate( vec3(0,1,0), mRotation); }
        vec3 down()            const                                   { return math::rotate(-vec3(0,1,0), mRotation); }
        vec3 front()           const                                   { return math::rotate( vec3(0,0,1), mRotation); }
        vec3 back()            const                                   { return math::rotate(-vec3(0,0,1), mRotation); }

        vec3 position()        const                                   { return mPosition; }
        vec3 scaling()         const                                   { return mScaling; }
        quat rotation()        const                                   { return mRotation; }

        Transformable &move(const vec3 &direction, float amount)       { mPosition += amount * direction; return (*this); } 
        Transformable &moveBy(float x, float y, float z)               { mPosition += vec3{x,y,z}; return (*this); } 
        Transformable &moveTo(const vec3 &position)                    { mPosition = position; return (*this); } 
        Transformable &moveTo(float x, float y, float z)               { mPosition = vec3{x,y,z}; return (*this); } 

        Transformable &rotate(const vec3 axis, const double &angle)    { return rotate(quat(axis, angle));  } 
        Transformable &rotate(const quat &rotation)                    { mRotation = normalize(rotation * mRotation); return (*this); } 
        Transformable &rotateTo(const vec3 axis, const double &angle)  { return rotateTo(quat(axis, angle)); } 
        Transformable &rotateTo(const quat &rotation)                  { mRotation = normalize(rotation); return (*this); } 

        Transformable &scale(float factor)                             { return scale(factor, factor, factor); } 
        Transformable &scale(const vec3 &factors)                      { return scale(factors.x, factors.y, factors.z); } 
        Transformable &scale(float fx, float fy, float fz)             { mScaling.x *= fx; mScaling.y *= fy; mScaling.z *= fz; return (*this); } 
        Transformable &scaleTo(const vec3 &scale)                      { mScaling = scale; return (*this); } 
        Transformable &scaleTo(float sx, float sy, float sz)           { mScaling = vec3{sx, sy, sz}; return (*this); } 


        mat4 modelMatrix() const { return translationMatrix() * rotationMatrix() * scalingMatrix(); }

        mat4 translationMatrix() const
        {
            return mat4
            {
                1.0f, 0.0f, 0.0f, mPosition.x,
                0.0f, 1.0f, 0.0f, mPosition.y,
                0.0f, 0.0f, 1.0f, mPosition.z,
                0.0f, 0.0f, 0.0f, 1.0f,
            };
        }
    
        mat4 scalingMatrix() const
        {
            return mat4
            {
                mScaling.x, 0.0f,       0.0f,       0.0f,
                0.0f,       mScaling.y, 0.0f,       0.0f,
                0.0f,       0.0f,       mScaling.z, 0.0f,
                0.0f,       0.0f,       0.0f,       1.0f
            };
        }
    
        mat4 rotationMatrix() const
        {
            auto l = left();
            auto u = up();
            auto f = front();
    
            return mat4 {
                l.x,  u.x,  f.x,  0.0f,
                l.y,  u.y,  f.y,  0.0f,
                l.z,  u.z,  f.z,  0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

    private:
        vec3 mScaling, mPosition;
        quat mRotation;
    };

}}