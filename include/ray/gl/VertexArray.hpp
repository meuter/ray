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

        template<typename V, typename F, size_t stride>
        void bindAttributeAtOffset(GLuint offset, Attribute<V> attribute, const VertexBuffer<F, stride> &vbo, GLboolean normalized=GL_FALSE) const
        {
            bind();
            attribute.bind(vbo, normalized, offset); 
            unbind();
        }

        template<typename V, typename F, size_t stride>
        void bindAttribute(Attribute<V> attribute, const VertexBuffer<F, stride> &vbo, bool normalized=false) const
        {
            bindAttributeAtOffset(0, attribute, vbo, normalized);
        }

        void bindIndices(const ElementBuffer &ebo) const 
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