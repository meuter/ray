#pragma once

#include <ray/components/Transformable.hpp>
#include <ray/assets/Wavefront.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/Texture.hpp>

namespace ray { namespace rendering {

    class Mesh: public components::Transformable
    {
    public:
        Mesh(const assets::Wavefront &object) { load(object); }
        Mesh(const std::string &filename) { load(filename); }

        void load(const std::string &filename)
        {
            load(assets::Wavefront(filename));
        }

        void load(const assets::Wavefront &object)
        {
            mVertexBuffer.reserve(5*object.totalVertexCount());
            auto mapped = mVertexBuffer.map(GL_WRITE_ONLY);
            for (auto shape = 0u; shape < object.shapeCount(); ++shape)
            {
                for (auto triangle = 0u; triangle < object.triangleCount(shape); triangle++)
                {
                    for (auto vertex = 0; vertex < 3; ++vertex)
                    {
                        auto position = object.getPosition(shape, triangle, vertex);
                        auto texcoord = object.getTexCoord(shape, triangle, vertex);
                        (*mapped++) = position.x;
                        (*mapped++) = position.y;
                        (*mapped++) = position.z;
                        (*mapped++) = texcoord.u; 
                        (*mapped++) = texcoord.v; 
                    }
                }
            }
            mVertexBuffer.unmap();

            // TODO(cme): if there are no texture, load default white
            for (size_t material = 0u; material < object.materialCount(); ++material)
                mDiffuseTextures.push_back(gl::Texture(object.getDiffuseTextureFilename(material)));
        }
        
        void draw() const
        {
            mVertexArray.bind();
            glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer.vertexCount());
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

        const gl::Texture &diffuseTexture(int index=0) const
        {
            return mDiffuseTextures[index];
        }

    private:
        gl::VertexBuffer<float, 5> mVertexBuffer;
        gl::VertexArray mVertexArray;
        std::vector<gl::Texture> mDiffuseTextures;    
    };

}}
