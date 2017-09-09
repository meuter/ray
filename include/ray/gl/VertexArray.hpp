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

        template<typename V, typename F>
        void setVertexAttribute(Attribute<V, F> attribute, const std::initializer_list<F> &vertexData)
        {
            setVertexAttribute(attribute, VertexBuffer<F>(vertexData), attribute.scalarCount(), 0);
        }

        template<typename V, typename F>
        void setVertexAttribute(Attribute<V, F> attribute, const VertexBuffer<F> &vbo, GLuint stride, GLuint offset, GLboolean normalized=GL_FALSE)
        {
            bind();
            vbo.bind();
            attribute.enable();
            attribute.pointer(normalized, stride, offset); 
            unbind();
        }

        void setVertexIndices(const std::initializer_list<GLuint> &indices)  
        {
            setVertexIndices(ElementBuffer(indices));
        }

        void setVertexIndices(const ElementBuffer &ebo)
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