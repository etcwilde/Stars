#ifndef __GRID_HPP
#define __GRID_HPP

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/Shader.hpp>

#include <vector>

#include "ShaderPaths.hpp"

class Grid : public atlas::utils::Geometry
{
        public:
                Grid();
                ~Grid();

                void renderGeometry(atlas::math::Matrix4 proj,
                                atlas::math::Matrix4 view) override;

        private:
                GLuint mVao;
                GLuint mVbo;
                size_t mVertexCount;
                glm::vec3 mColor;
};


#endif//__GRID_HPP
