#pragma once

#include <ray/gl/AbstractTexture.hpp>
#include <ray/assets/Bitmap.hpp>
#include <string>

namespace ray { namespace gl {

    using samplerCube = sampler<GL_TEXTURE_CUBE_MAP>;

    class CubeMap : public AbstractTexture<GL_TEXTURE_CUBE_MAP>
    {
    protected:
        using Bitmap = assets::Bitmap;        
    public:
        CubeMap(const std::vector<std::string> &faces) { load(faces); }
    
        void load(const std::vector<std::string> &faces)
        {
            for (size_t i = 0; i < 6u; ++i)
            {   
                auto face = Bitmap(faces[i]);
                AbstractTexture::load(face.width(), face.height(), face.depth(), face.pixels(), GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, false);
            }
            setFilter(GL_LINEAR, GL_LINEAR);
            setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        }
    };
}}
