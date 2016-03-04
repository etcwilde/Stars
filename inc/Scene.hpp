#ifndef __STAR_SCENE_HPP
#define __STAR_SCENE_HPP

#include <atlas/utils/Scene.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/gl/Shader.hpp>

#include "ArcCamera.hpp"
#include "Grid.hpp"
#include "Star.hpp"

#include <vector>

class Scene: public atlas::utils::Scene
{
        public:
                Scene();
                ~Scene();

                // Event Handlers
                void mousePressEvent(int button, int action, int mods, double x, double y) override;
                void mouseMoveEvent(double x, double y) override;
                void scrollEvent(double x, double y) override;
                void keyPressEvent(int key, int scancode, int action, int mods) override;

                void screenResizeEvent(int width, int height) override;

                void updateScene(double time) override;
                void stepScene(double time);
                void renderScene() override;

        private:
                // Free, camera is affected by gravity of stars
                // Fixed, camera is moved by user-input only
                enum MoveType { MOVE_FIXED, MOVE_FREE };
                enum CameraType { CAMERA_ARC, CAMERA_SHIP };

                bool mDragging;
                bool mPaused;
                bool mPrevTime;
                int mWidth;
                int mHeight;
                MoveType mMovementType;
                CameraType mCameraType;
                ArcCamera mArcCam;

                bool mDrawGrid;
                Grid mGrid;

                StarField mStars;
};


#endif//__STAR_SCENE_HPP
