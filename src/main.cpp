#include <atlas/utils/Application.hpp>

#include "Scene.hpp"
#include "math.hpp"

#include <iostream>
#include <chrono>

int main()
{
        APPLICATION.createWindow(800, 800, "Stars");
        APPLICATION.addScene(new Scene);
        APPLICATION.runApplication();
        return 0;
}
