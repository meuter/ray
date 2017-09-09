#pragma once

#include <ray/glfw/glad.hpp>

namespace ray { namespace gl {

    template<void create(GLuint &), void destroy(GLuint)>
    class Handle
    {
        using Ref = GLuint(&);
        using ConstRef = const GLuint(&);
    public:
        inline Handle() { create(value); }
        inline Handle(const Handle &other) = delete;
        inline Handle(Handle &&other) : value(other) { other.value = 0; }
        inline ~Handle() { if (value) destroy(value); }
        inline Handle &operator=(const Handle &other) = delete;
        inline Handle &operator=(Handle &&other) = delete;
        inline GLuint *operator &() { return &value; }
        inline const GLuint *operator&() const { return &value; }
        inline operator Ref() { return value; }
        inline operator ConstRef() const { return value; }
    private:
        GLuint value;
    };

}}