#pragma once

#include <ray/assets/Wavefront.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/Texture.hpp>

namespace ray { namespace entities {

    class Mesh
    {
        static constexpr size_t N_FLOATS_PER_VERTEX = 8;
    public:
        Mesh(const assets::Wavefront &object) { load(object); }
        Mesh(const std::string &filename) { load(filename); }

        void load(const std::string &filename)
        {
            load(assets::Wavefront(filename));
        }

        void load(const assets::Wavefront &object)
        {
            mVertexBuffer.reserve(N_FLOATS_PER_VERTEX*object.totalVertexCount());
            auto mapped = mVertexBuffer.map(GL_WRITE_ONLY);
            for (auto shape = 0u; shape < object.shapeCount(); ++shape)
            {
                for (auto triangle = 0u; triangle < object.triangleCount(shape); triangle++)
                {
                    for (auto vertex = 0; vertex < 3; ++vertex)
                    {
                        auto position = object.getPosition(shape, triangle, vertex);
                        (*mapped++) = position.x;
                        (*mapped++) = position.y;
                        (*mapped++) = position.z;
                        auto texcoord = object.getTexCoord(shape, triangle, vertex);
                        (*mapped++) = texcoord.u; 
                        (*mapped++) = texcoord.v; 
                        auto normal   = object.getNormal(shape, triangle, vertex);
                        (*mapped++) = normal.x;
                        (*mapped++) = normal.y;
                        (*mapped++) = normal.z;                        
                    }
                }
            }
            mVertexBuffer.unmap();

            // TODO(cme): if there are no texture, load default white
            for (size_t material = 0; material < object.materialCount(); ++material)
                mDiffuseTextures.push_back(gl::Texture(object.getDiffuseTextureFilename((int)material)));
        }
        
        void draw() const
        {
            mVertexArray.bind();
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mVertexBuffer.vertexCount());
            mVertexArray.unbind();
        }

        void bindPosition(gl::Attribute<math::vec3> position) const
        {
            mVertexArray.bindAttributeAtOffset(0, position, mVertexBuffer);
        }

        void bindTexCoord(gl::Attribute<math::vec2> texCoord) const
        {
            mVertexArray.bindAttributeAtOffset(3, texCoord, mVertexBuffer);
        }

        void bindNormal(gl::Attribute<math::vec3> normal) const
        {
            mVertexArray.bindAttributeAtOffset(5, normal, mVertexBuffer);
        }

        const gl::Texture &diffuseTexture(int index=0) const
        {
            return mDiffuseTextures[index];
        }

    private:
        gl::VertexBuffer<float, N_FLOATS_PER_VERTEX> mVertexBuffer;
        gl::VertexArray mVertexArray;
        std::vector<gl::Texture> mDiffuseTextures;    
    };

}}
