#pragma once 

#include <ray/gl/Handle.hpp>
#include <ray/gl/Buffer.hpp>
#include <ray/gl/Attribute.hpp>
#include <ray/gl/Type.hpp>
#include <initializer_list>

namespace ray { namespace gl {

    class VertexArray
    {
    public:
        inline void bind() const { gl(BindVertexArray(mHandle)); }
        inline void unbind() const { gl(BindVertexArray(0)); }

        template<typename V, typename AttribScalar, typename F, size_t stride>
        void setAttributeAtOffset(GLuint offset, Attribute<V, AttribScalar> attribute, const VertexBuffer<stride, F> &vbo, GLboolean normalized=GL_FALSE)
        {
            bind();
            attribute.pointer(vbo, normalized, offset); 
            unbind();
        }

        template<typename V, typename AttribScalar, typename F, size_t stride>
        void setAttribute(Attribute<V, AttribScalar> attribute, const VertexBuffer<stride, F> &vbo, bool normalized=false)
        {
            setAttributeAtOffset(0, attribute, vbo, normalized ? GL_TRUE : GL_FALSE);
        }

        void setIndices(const ElementBuffer &ebo)
        {
            bind();
            ebo.bind();
            unbind();
        }

    private:        
        static void destroy(GLuint handle) { gl(DeleteVertexArrays(1, &handle)); }
        static void create(GLuint &handle) { gl(GenVertexArrays(1, &handle)); }
        Handle<create, destroy> mHandle;
    };

}}