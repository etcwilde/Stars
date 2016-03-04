#include "Scene.hpp"

#include <atlas/core/Log.hpp>
#include <atlas/core/GLFW.hpp>

Scene::Scene() :
        mDragging(false),
        mPaused(true),
        mPrevTime(0.0),
        mMovementType(Scene::MoveType::MOVE_FIXED),
        mCameraType(Scene::CameraType::CAMERA_ARC),
        mStars(10)
{
        // Initialize openGL stuff
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

Scene::~Scene() {}

void Scene::mousePressEvent(int b, int a, int m, double x, double y)
{
        if (b == GLFW_MOUSE_BUTTON_MIDDLE && a == GLFW_PRESS)
        {
                mDragging = true;
                ArcCamera::CameraMovements movements;
                switch(m)
                {
                        case GLFW_MOD_CONTROL:
                                movements = ArcCamera::CameraMovements::DOLLY;
                                break;
                        case GLFW_MOD_SHIFT:
                                movements = ArcCamera::CameraMovements::TRACK;
                                break;
                        default:
                                movements = ArcCamera::CameraMovements::TUMBLE;
                                break;
                }
                mArcCam.mouseDown(atlas::math::Point2(x, y), movements);
        }
        else
        {
                mDragging = false;
                mArcCam.mouseUp();
        }
}

void Scene::mouseMoveEvent(double x, double y)
{
        if (mDragging) mArcCam.mouseDrag(atlas::math::Point2(x, y));
}

void Scene::scrollEvent(double x, double y)
{
        mArcCam.mouseScroll(atlas::math::Point2(x, y));
}


#include <iostream>
void Scene::keyPressEvent(int key, int scancode, int action, int mods)
{
        if (action == GLFW_PRESS)
        {
                switch(key)
                {
                        case GLFW_KEY_G: mDrawGrid = !mDrawGrid; break;
                        case GLFW_KEY_SPACE: mPaused = !mPaused; break;
                        case GLFW_KEY_S: stepScene(0.05); break;
                        case GLFW_KEY_W: mStars.resetGeometry(); break;
                }
        }
}

void Scene::screenResizeEvent(int width, int height)
{
        mWidth = width;
        mHeight = height;
        atlas::utils::Scene::screenResizeEvent(width, height);
        mArcCam.setDims(width, height);
}

void Scene::updateScene(double time)
{

        mTime.deltaTime = static_cast<float>(time) - mTime.currentTime;
        mTime.totalTime += static_cast<float>(time);
        mTime.currentTime = static_cast<float>(time);
        if (!mPaused)
        {
                mStars.updateGeometry(mTime);
        }
}

void Scene::stepScene(double time)
{
        mTime.deltaTime = static_cast<float>(time) - mTime.currentTime;
        mTime.totalTime += static_cast<float>(time);
        mTime.currentTime = static_cast<float>(time);
        mTime.deltaTime = static_cast<float>(time);
        mStars.stepGeometry(mTime);
}

void Scene::renderScene()
{
        glClearColor(0.f, 0.16f, 0.21f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        switch (mCameraType)
        {
                case CAMERA_ARC: mView = mArcCam.getCameraMatrix(); break;
                default: mView = atlas::math::Matrix4(1.f); break;
        }

        //mGrid.renderGeometry(mProjection, mView);
        if (mDrawGrid) mGrid.renderGeometry(mProjection, mView);
        mStars.renderGeometry(mProjection, mView);
}
