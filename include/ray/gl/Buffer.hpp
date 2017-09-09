#pragma once

#include <ray/gl/Handle.hpp>
#include <initializer_list>

namespace ray { namespace gl {
        
    template<GLenum target, size_t stride, typename T>
    class Buffer 
    {     
    public:   
        Buffer() = default;              
        Buffer(const size_t size, GLuint usage=GL_STATIC_DRAW) { load(nullptr, size, usage); }
        Buffer(const std::initializer_list<T> &data, GLuint usage=GL_STATIC_DRAW) { load(data, usage); }

        void load(const std::initializer_list<T> &data, GLuint usage=GL_STATIC_DRAW) 
        {
            load(data.begin(), data.size(), usage); 
        }

        void load(const T *data, size_t count, GLuint usage=GL_STATIC_DRAW)
        {
            bind();
            gl(BufferData(target, count * sizeof(T), data, usage));
        }

        void loadAt(size_t countOffset, const T *data, size_t count)
        {
            bind();
            gl(BufferSubData(target, countOffset*sizeof(T), count*sizeof(T), &data[0]));
        }

        size_t size() const
        {
            GLint result = 0;
            bind();
            gl(GetBufferParameteriv(target, GL_BUFFER_SIZE, &result));
            return result / sizeof(T);
        }

        size_t count() const
        {
            return size() / stride;
        }

        T *mapRange(size_t offset, size_t count, GLbitfield access)
        {
            bind();
            return (T*)glMapBufferRange(target, offset*sizeof(T), count*sizeof(T), access);
        }

        T *map(GLenum access) { bind(); return reinterpret_cast<T*>(glMapBuffer(GL_ARRAY_BUFFER, access)); }

        void unmap() { bind(); gl(UnmapBuffer(target)); }
        void bind() const  { gl(BindBuffer(target, mHandle)); }
        void unbind() const { gl(BindBuffer(target, 0)); }

    private:
        static void create(GLuint &handle) { gl(GenBuffers(1, &handle)); }
        static void destroy(GLuint handle) { gl(DeleteBuffers(1, &handle)); }
        gl::Handle<create, destroy> mHandle;
    };

    template<size_t stride, typename T>
    using VertexBuffer  = Buffer<GL_ARRAY_BUFFER, stride, T>;
    
    using ElementBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, 1, GLuint>;

}}