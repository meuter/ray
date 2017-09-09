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

        template<typename T, typename U>
        void setVertexAttribute(Attribute<T> attrib, const std::initializer_list<U> &vertexData)
        {
            setVertexAttribute(attrib, VertexBuffer<U>(vertexData), attrib.size()/sizeof(U), 0);
        }

        template<typename T, typename U>
        void setVertexAttribute(Attribute<T> attrib,const VertexBuffer<U> &vbo, GLuint stride, GLuint offset)
        {
            const auto nComponent = attrib.size() / sizeof(U);            
            vertexAttribute(vbo, attrib, attrib.size() / sizeof(U), getType<U>(), GL_FALSE, stride, offset);
        }

        template<typename T, typename U>
        void vertexAttribute(const VertexBuffer<U> &vbo, Attribute<T> attrib, GLint count, GLenum type, GLboolean normalized, GLsizei stride, size_t offset)
        {
            bind();
            vbo.bind();
            attrib.enable();
            attrib.pointer(count, type, normalized, stride*sizeof(U), offset*sizeof(U));
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