#pragma once 

#include <ray/gl/Handle.hpp>
#include <ray/gl/Buffer.hpp>
#include <ray/gl/Attribute.hpp>
#include <initializer_list>

namespace ray { namespace gl {

    class VertexArray
    {
    public:
        using IBO = ElementBuffer;
        
        inline void bind() const { glBindVertexArray(mHandle); }
        inline void unbind() const { glBindVertexArray(0); }

        void setVertexAttribute(Attribute attrib, GLuint nComponent, const std::initializer_list<GLfloat> &vertexData)
        {
            setVertexAttribute(attrib, nComponent, VertexBuffer<GLfloat>(vertexData));
        }

        void setVertexAttribute(Attribute attrib, GLuint nComponent, const VertexBuffer<GLfloat> &vbo, GLuint stride=0, GLuint offset=0)
        {
            if (stride == 0) stride = nComponent;
            vertexAttribute(vbo, attrib, nComponent, GL_FLOAT, GL_FALSE, stride*sizeof(GLfloat), offset*sizeof(GLfloat));
        }

        template<typename T>
        void vertexAttribute(const VertexBuffer<T> &vbo, Attribute attrib, GLint count, GLenum type, GLboolean normalized, GLsizei stride, size_t offset)
        {
            bind();
            vbo.bind();
            attrib.enable();
            attrib.pointer(count, type, normalized, stride, offset);
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
        static void destroy(GLuint handle) { glDeleteVertexArrays(1, &handle); }
        static void create(GLuint &handle) { glGenVertexArrays(1, &handle); }
        Handle<create, destroy> mHandle;
    };

}}