#include "Star.hpp"


// TODO: Implement with count -- Get working for now
StarField::StarField(size_t count)
{
        USING_ATLAS_GL_NS;

        // Generate Geometry

        // Start working with one star

        mStars.push_back({glm::vec3(0, 0, 0), glm::vec3(0, 0, 2.8), glm::vec4(1.f, 0.7f, 0.2f, 1.f), 3.89f, 4.f});
        mStars.push_back({glm::vec3(4, 1, 0), glm::vec3(0, 0, -2.8), glm::vec4(1.f, 0.7f, 0.2f, 1.f), 3.89f, 4.f});
        mStars.push_back({glm::vec3(8, 0, 0), glm::vec3(0, 0, -5.8), glm::vec4(1.f, 0.5f, 0.4f, 1.f), 0.59f, 1.f});
        mStars.push_back({glm::vec3(-4, -1, 0), glm::vec3(0, 0, 5.8), glm::vec4(0.2f, 0.3f, 0.7f, 1.f), 0.59f, 1.f});


        //resetGeometry();

        // Okay, neat -- now set up the buffers
        mModel = atlas::math::Matrix4(1.f);
        // Generate Vertex Arrays
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        // Generate Buffers
        glGenBuffers(1, &mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);

        // Load Geometry
        glBufferData(GL_ARRAY_BUFFER, sizeof(Star) * mStars.size(),
                        mStars.data(), GL_STATIC_DRAW);

        // 0: Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, _pos));
        glEnableVertexAttribArray(0);
        // 1: Color
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, _color));
        glEnableVertexAttribArray(1);
        // 2: mass/radius
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, _mass));
        glEnableVertexAttribArray(2);


        // Handle shaders
        const std::string ShaderDir = generated::ShaderPaths::getShaderDirectory();
        std::vector<ShaderInfo> shaders {
                { GL_VERTEX_SHADER, ShaderDir + "point.vs.glsl"},
                { GL_FRAGMENT_SHADER, ShaderDir + "point.fs.glsl"}
        };

        mShaders.push_back(ShaderPointer(new Shader));
        mShaders[0]->compileShaders(shaders);
        mShaders[0]->linkShaders();

        // Get Uniforms
        GLuint varID;
        varID = mShaders[0]->getUniformVariable("M");
        mUniforms.insert(UniformKey("M", varID));
        varID = mShaders[0]->getUniformVariable("V");
        mUniforms.insert(UniformKey("V", varID));
        varID = mShaders[0]->getUniformVariable("P");
        mUniforms.insert(UniformKey("P", varID));
        glBindVertexArray(0);
        mShaders[0]->disableShaders();
}

StarField::~StarField()
{
        glDeleteVertexArrays(1, &mVao);
        glDeleteBuffers(1, &mVbo);
}

void StarField::renderGeometry(atlas::math::Matrix4 p, atlas::math::Matrix4 v)
{
        mShaders[0]->enableShaders();
        glBindVertexArray(mVao);
        glUniformMatrix4fv(mUniforms["M"], 1, GL_FALSE, &mModel[0][0]);
        glUniformMatrix4fv(mUniforms["V"], 1, GL_FALSE, &v[0][0]);
        glUniformMatrix4fv(mUniforms["P"], 1, GL_FALSE, &p[0][0]);
        glDrawArrays(GL_POINTS, 0, mStars.size());
        glBindVertexArray(0);
        mShaders[0]->disableShaders();
}

void StarField::resetGeometry()
{
        /*
        mStars.push_back({glm::vec3(-10, 0, 0), glm::vec3(0, 0, 0), glm::vec4(1.f, 0.8f, 0.3f, 1.f), 0.5f, 0.5f});
        mStars.push_back({glm::vec3(0, 10, 0), glm::vec3(0.5, 0, 0), glm::vec4(1.f, 0.2f, 0.0f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(-15, 10, 0), glm::vec3(0, 0, 0), glm::vec4(1.f, 0.8f, 0.2f, 1.f), 2.f, 2.f});
        mStars.push_back({glm::vec3(15, 20, 0), glm::vec3(0, 0, 0), glm::vec4(0.8f, 0.6f, 0.4f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(15, 29, 0), glm::vec3(0, 0, 0), glm::vec4(0.8f, 0.9f, 0.4f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(15, 10, 5), glm::vec3(0, 0, 0), glm::vec4(0.1f, 0.9f, 0.4f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(-5, -10, 5), glm::vec3(0, 0, 0), glm::vec4(0.8f, 0.9f, 0.0f, 1.f), 1.f, 4.f});

        */

}

void StarField::stepGeometry(atlas::utils::Time const&t)
{
        physics::ODERK3 solver;
        glm::vec3 pos, vel;

        for (auto star = mStars.begin(); star != mStars.end(); star++)
        {
                if (star == mStars.begin()) continue;
                auto accelFunc = [=](auto t)
                {
                        glm::vec3 A;
                        for (auto other = mStars.begin(); other != mStars.end(); other++)
                        {
                                if (other == star) continue;
                                const float distance = glm::length(star->_pos - other->_pos);
                                A = -(other->_pos - star->_pos) * (-other->_mass / (distance * distance));
                        }
                        return A * 6.67E-11f / static_cast<float>(mStars.size());
                };

                solver.evaluate([=](auto t, auto y) { return accelFunc(t);},
                                t.currentTime, star->_pos, star->_vel,
                                t.deltaTime, pos, vel);
                std::cout << "Original Position: [" << star->_pos.x << ", " << star->_pos.y << ", " << star->_pos.z << "]\n";
                std::cout << "New Position: [" << pos.x << ", " << pos.y << ", " << pos.z << "]\n";
                std::cout << "Original Velocity : [" << star->_vel.x << ", " << star->_vel.y << ", " << star->_vel.z << "]\n";
                std::cout << "New Position: [" << vel.x << ", " << vel.y << ", " << vel.z << "]\n";
                star->_pos = pos;
                star->_vel = vel;
        }

        glBindVertexArray(mVao);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Star) * mStars.size(), mStars.data());
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StarField::updateGeometry(atlas::utils::Time const&t)
{
        physics::ODERK3 solver;
        glm::vec3 pos, vel;

        for (auto star = mStars.begin(); star != mStars.end(); star++)
        {
                //if (star == mStars.begin()) continue;
                auto accelFunc = [=](auto t)
                {
                        glm::vec3 A;
                        for (auto other = mStars.begin(); other != mStars.end(); other++)
                        {
                                if (other == star) continue;
                                const float distance = glm::abs(glm::length(star->_pos - other->_pos));
                                if (distance == 0) continue;
                                A += (star->_pos - other->_pos) * (-other->_mass / (distance * distance));
                        }
                        return A * 6.67f / static_cast<float>(mStars.size() - 1);
                };
                solver.evaluate([=](auto t, auto y) { return accelFunc(t) / star->_mass;},
                                t.currentTime, star->_pos, star->_vel,
                                t.deltaTime, pos, vel);
                star->_pos = pos;
                star->_vel = vel;
        }

        glBindVertexArray(mVao);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Star) * mStars.size(), mStars.data());
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}
