#pragma once

#include <ray/gl/VertexArray.hpp>

namespace ray { namespace entities {

    class Cube : public gl::VertexArray 
    {
        using vec3 = math::vec3;
        using vec2 = math::vec2;
    public:
        Cube()
        {
            mVertexBuffer.load({
                ////////////////////////////////////////////////////////////
                // Position             TexCoord        Normal    
                ////////////////////////////////////////////////////////////
                // Front face
                -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f, 1.0f,
                0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f,  0.0f, 1.0f,
                0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f,  0.0f, 1.0f,
               -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     0.0f,  0.0f, 1.0f,
               -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f, 1.0f,
    
               // Back face
               -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f, -1.0f,
                0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f,
               -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f, -1.0f,
               -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f, -1.0f,
    
               // Left face
               -0.5f,  0.5f, -0.5f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
               -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     1.0f,  0.0f,  0.0f,
               -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
               -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
               -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0f,  0.0f,  0.0f,
               -0.5f,  0.5f, -0.5f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
    
               // Right face 
               0.5f,  0.5f, -0.5f,     1.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
               0.5f,  0.5f, +0.5f,     1.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
               0.5f, -0.5f, +0.5f,     0.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
               0.5f, -0.5f, +0.5f,     0.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
               0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
               0.5f,  0.5f, -0.5f,     1.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
    
               // Bottom face
               -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,     0.0f, 1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,    1.0f, 1.0f,     0.0f, 1.0f,  0.0f,
                0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f, 1.0f,  0.0f,
                0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f, 1.0f,  0.0f,
               -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, 1.0f,  0.0f,
               -0.5f, -0.5f, +0.5f,    0.0f, 1.0f,     0.0f, 1.0f,  0.0f,
    
               // Top face
               -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  -1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  -1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
               -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
               -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  -1.0f,  0.0f
            });
        }
    
        void bindPosition(gl::Attribute<vec3> position) const { bindAttributeAtOffset(0, position, mVertexBuffer);  }
        void bindTexCoord(gl::Attribute<vec2> texCoord) const { bindAttributeAtOffset(3, texCoord, mVertexBuffer);  }
        void bindNormal(gl::Attribute<vec3> normal)     const { bindAttributeAtOffset(5, normal, mVertexBuffer);    }
    
        void draw() const
        {
            bind();
            glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer.vertexCount());
            unbind();
        }
    
    private:
        gl::VertexBuffer<float,8> mVertexBuffer;
    };
    

}}