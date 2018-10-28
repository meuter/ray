#pragma once

#include <ray/math/LinearAlgebra.hpp>
#include <ray/platform/FileSystem.hpp>
#include <ray/platform/Panic.hpp>
#include <tiny_obj_loader.h>

namespace ray { namespace assets {

    class Wavefront 
    {
    public:
        Wavefront(const std::string &filename) : mBaseDirectory(platform::fs::parent(filename))
        {
            std::string error;
            bool success;

            success = tinyobj::LoadObj(&mAttributes, &mShapes, &mMaterials, &error, filename.c_str(), mBaseDirectory.c_str());        
            panicif(!success, "could not load '%s': %s", filename, error);
            // TODO(cme): log warning if any
        }

        size_t totalVertexCount() const 
        {
            size_t result = 0;
            for (auto &shape: mShapes)
                result += shape.mesh.indices.size();
            return result;        
        }

        size_t shapeCount() const
        {
            return mShapes.size();
        }

        size_t triangleCount(int shape) const
        {
            return mShapes[shape].mesh.indices.size() / 3;
        }

        math::vec3 getPosition(int shape, int triangle, int vertex) const
        {
            auto k = 3*mShapes[shape].mesh.indices[3*triangle+vertex].vertex_index;
            return math::vec3(mAttributes.vertices[k], mAttributes.vertices[k+1], mAttributes.vertices[k+2]);
        }

        math::vec2 getTexCoord(int shape, int triangle, int vertex) const
        {
            auto k = 2*mShapes[shape].mesh.indices[3*triangle+vertex].texcoord_index;
            return math::vec2(mAttributes.texcoords[k], mAttributes.texcoords[k+1]);
        }

        math::vec3 getNormal(int shape, int triangle, int vertex) const
        {
            if (mAttributes.normals.size() == 0) return math::vec3(0);
            auto k = 3*mShapes[shape].mesh.indices[3*triangle+vertex].normal_index;
            return math::vec3(mAttributes.normals[k], mAttributes.normals[k+1], mAttributes.normals[k+2]);
        }

        size_t materialCount() const
        {
            return mMaterials.size();
        }

        const std::string getDiffuseTextureFilename(int material) const
        {
            return platform::fs::join(mBaseDirectory, mMaterials[material].diffuse_texname);
        }

    private:
        std::vector<tinyobj::material_t> mMaterials;
        tinyobj::attrib_t mAttributes;
        std::vector<tinyobj::shape_t> mShapes;
        std::string mBaseDirectory;
    };

}}
