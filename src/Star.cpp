#include "Star.hpp"


// TODO: Implement with count -- Get working for now
StarField::StarField(size_t count)
{
        USING_ATLAS_GL_NS;

        // Generate Geometry

        // Start working with one star

        mStars.push_back({glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec4(1.f, 0.8f, 0.3f, 1.f), 0.5f, 3.f});
        mStars.push_back({glm::vec3(0, 10, 0), glm::vec3(0.5, 0, 0), glm::vec4(1.f, 0.2f, 0.0f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(15, 10, 0), glm::vec3(0, 0, 0), glm::vec4(1.f, 0.8f, 0.2f, 1.f), 0.25f, 2.f});
        mStars.push_back({glm::vec3(15, 20, 0), glm::vec3(0, 0, 0), glm::vec4(0.8f, 0.6f, 0.4f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(15, 29, 0), glm::vec3(0, 0, 0), glm::vec4(0.8f, 0.9f, 0.4f, 1.f), 5.f, 4.f});
        mStars.push_back({glm::vec3(15, -10, 5), glm::vec3(0, 0, 0), glm::vec4(0.1f, 0.9f, 0.4f, 1.f), 0.1f, 4.f});
        mStars.push_back({glm::vec3(15, -14, 5), glm::vec3(0, 0, 0), glm::vec4(0.1f, 0.9f, 0.4f, 1.f), 0.2f, 2.f});
        mStars.push_back({glm::vec3(15, -14, -5), glm::vec3(0, 0, 0), glm::vec4(0.1f, 0.9f, 0.4f, 1.f), 0.2f, 2.f});
        mStars.push_back({glm::vec3(15, 14, -5), glm::vec3(0, 0, 0), glm::vec4(0.1f, 0.9f, 0.4f, 1.f), 0.2f, 2.f});


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

void StarField::stepGeometry(atlas::utils::Time const& t)
{
        physics::ODERK4 solver;
        glm::vec3 pos, vel;
        // Okay, we need to update all of the things
        for (auto sIt = mStars.begin(); sIt != mStars.end(); sIt++)
        {
                if (sIt == mStars.begin()) continue;
                auto accelFunc = [=](auto t)
                {
                        glm::vec3 F;
                        for (auto other = mStars.begin(); other != mStars.end(); other++)
                        {
                                if (other == sIt) continue;
                                const glm::vec3 disp = other->_pos - sIt->_pos;
                                const float x = glm::length(disp);
                                F += glm::vec3(-25.f * (15.f - x) * glm::normalize(disp));
                        }
                        return F / sIt->_mass;
                };
                solver.evaluate([=](auto t, auto y){return accelFunc(t);}, t.currentTime, sIt->_pos, sIt->_vel, t.deltaTime, pos, vel);
                std::cout << "Star: " << sIt - mStars.begin() << '\n';
                std::cout << "Position: [" << sIt->_pos.x << ", " <<
                        sIt->_pos.y << ", " <<
                        sIt->_pos.z << "] -> [" <<
                        pos.x << ", " <<
                        pos.y << ", " <<
                        pos.z << "]\n";


                sIt->_pos = pos;
                sIt->_vel = vel;
        }
        // Update the positions
        glBindVertexArray(mVao);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Star) * mStars.size(), mStars.data());
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void StarField::resetGeometry()
{
        mStars.push_back({glm::vec3(-10, 0, 0), glm::vec3(0, 0, 0), glm::vec4(1.f, 0.8f, 0.3f, 1.f), 0.5f, 0.5f});
        mStars.push_back({glm::vec3(0, 10, 0), glm::vec3(0.5, 0, 0), glm::vec4(1.f, 0.2f, 0.0f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(-15, 10, 0), glm::vec3(0, 0, 0), glm::vec4(1.f, 0.8f, 0.2f, 1.f), 2.f, 2.f});
        mStars.push_back({glm::vec3(15, 20, 0), glm::vec3(0, 0, 0), glm::vec4(0.8f, 0.6f, 0.4f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(15, 29, 0), glm::vec3(0, 0, 0), glm::vec4(0.8f, 0.9f, 0.4f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(15, 10, 5), glm::vec3(0, 0, 0), glm::vec4(0.1f, 0.9f, 0.4f, 1.f), 1.f, 4.f});
        mStars.push_back({glm::vec3(-5, -10, 5), glm::vec3(0, 0, 0), glm::vec4(0.8f, 0.9f, 0.0f, 1.f), 1.f, 4.f});

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
        // Okay, we need to update all of the things
        for (auto sIt = mStars.begin(); sIt != mStars.end(); sIt++)
        {
                if (sIt == mStars.begin()) continue;
                auto accelFunc = [=](auto t)
                {
                        glm::vec3 F;
                        for (auto other = mStars.begin(); other != mStars.end(); other++)\
                        {
                                if (other == sIt) continue;
                                const glm::vec3 disp = other->_pos - sIt->_pos;
                                const float x = glm::length(disp);
                                F += glm::vec3(-25.f * (15.f - x) * glm::normalize(disp));
                        }
                        return F / sIt->_mass;
                };
                solver.evaluate([=](auto t, auto y){return accelFunc(t);}, t.currentTime, sIt->_pos, sIt->_vel, t.deltaTime, pos, vel);
                sIt->_pos = pos;
                sIt->_vel = vel;
        }
        // Update the positions
        glBindVertexArray(mVao);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Star) * mStars.size(), mStars.data());
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

}
