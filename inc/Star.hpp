#ifndef __STAR_STAR_HPP
#define __STAR_STAR_HPP

#include <atlas/utils/Geometry.hpp>

#include <vector>
#include <iterator>

#include "ShaderPaths.hpp"
#include "math.hpp"

#define UNIVERSE_SEED   42

// There was never a star to begin with

class StarField : public atlas::utils::Geometry
{
        public:
                // Generate a bunch of random stars
                StarField(size_t starCount);

                ~StarField();

                void renderGeometry(atlas::math::Matrix4 projection,
                                atlas::math::Matrix4 view) override;

                void updateGeometry(atlas::utils::Time const&t) override;

                void stepGeometry(atlas::utils::Time const&t);

                void resetGeometry() override;


        private:
                GLuint mVao;
                GLuint mVbo;

                struct Star
                {
                        glm::vec3 _pos;     // pos
                        glm::vec3 _vel;     // velocity
                        glm::vec4 _color;   // rgba
                        float _mass;
                        float _radius;
                };

                // TODO: Look into acceleration structures for this data
                std::vector<struct Star> mStars;

};


#endif//__STAR_STAR_HPP
