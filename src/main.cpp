#include <atlas/utils/Application.hpp>

#include "Scene.hpp"
#include "math.hpp"

#include <iostream>
#include <chrono>

// Scalar: float
// Vector: float
float function(float x, float y)
{
        return ((((-2 * x) + 12) * x) - 20) * x + 8.5;
}

float accelFunc(float x, float y)
{
        //return 10;
        return 2*x + 3;
}

float velFunc(float x, float y)
{
        //return 10 * x;
        return x * x + 3.f * x;
}


int main()
{

        float actual = 3.21875;
        float x = 0, y = 1;
        float yout;

        std::cout << "Testing ODE Solvers:\n";
        math::ODEEuler euler;
        math::ODEHune hune;
        math::ODEMidPoint midPoint;
        math::ODERalston ralston;
        math::ODERK3 rk3;
        math::ODERK4 rk4;
        euler.evaluate(function, x, y, 0.5f, yout);
        std::cout << "Euler:\t\t" << yout << "\tRelative Error: " << ((actual - yout) / actual) * 100.<< "%\n";
        hune.evaluate(function, x, y, 0.5f, yout);
        std::cout << "Hune:\t\t" << yout << "\tRelative Error: " << ((actual - yout) / actual) * 100.<< "%\n";
        midPoint.evaluate(function, x, y, 0.5f, yout);
        std::cout << "Midpoint:\t" << yout << "\tRelative Error: " << ((actual - yout) / actual) * 100.<< "%\n";
        ralston.evaluate(function, x, y, 0.5f, yout);
        std::cout << "Ralston:\t" << yout << "\tRelative Error: " << ((actual - yout) / actual) * 100.<< "%\n";
        rk3.evaluate(function, x, y, 0.5f, yout);
        std::cout << "RK3:\t\t" << yout << "\tRelative Error: " << ((actual - yout) / actual) * 100.<< "%\n";
        rk4.evaluate(function, x, y, 0.5f, yout);
        std::cout << "RK4:\t\t" << yout << "\tRelative Error: " << ((actual - yout) / actual) * 100.<< "%\n";
        std::cout << "Actual Answer:\t" << actual << '\n';

        float v0 = 0, p0 = 0;
        float vel, pos;
        float actualVel;
        float h = 0.5f;

        //std::chrono::high_resolution_clock<
        rk4.evaluate(velFunc, x, p0, h, actual);
        rk4.evaluate(accelFunc, x, v0, h, actualVel);
        physics::ODEHune huneSolver;
        physics::ODEMidPoint midPointSolver;
        physics::ODERalston ralstonSolver;
        physics::ODERK3 rk3Solver;
        physics::ODERK4 rk4Solver;
        std::cout << "Testing Position Solver:\n";
        auto start = std::chrono::high_resolution_clock::now();
        huneSolver.evaluate(accelFunc, x, p0, v0, h, pos, vel);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Hune:\t\t" "Velocity: " << vel << " Position: " << pos << " \tError: " << ((actual - pos) / actual) * 100. << "%\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns\n";

        start = std::chrono::high_resolution_clock::now();
        midPointSolver.evaluate(accelFunc, x, p0, v0, h, pos, vel);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Midpoint:\t" "Velocity: " << vel << " Position: " << pos << " \tError: " << ((actual - pos) / actual) * 100. << "%\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns\n";

        start = std::chrono::high_resolution_clock::now();
        ralstonSolver.evaluate(accelFunc, x, p0, v0, h, pos, vel);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Ralston:\t" "Velocity: " << vel << " Position: " << pos << " \tError: " << ((actual - pos) / actual) * 100. << "%\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns\n";

        start = std::chrono::high_resolution_clock::now();
        rk3Solver.evaluate(accelFunc, x, p0, v0, h, pos, vel);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "RK3:\t\t" "Velocity: " << vel << " Position: " << pos << " \t\tError: " << ((actual - pos) / actual) * 100. << "%\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns\n";

        start = std::chrono::high_resolution_clock::now();
        rk4Solver.evaluate(accelFunc, x, p0, v0, h, pos, vel);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "RK4:\t\t" "Velocity: " << vel << " Position: " << pos << " \t\tError: " << ((actual - pos) / actual) * 100. << "%\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns\n";


        // Star Base
        // M1x'' = -G M1 (m2(x2 - x1)/r12^2 + m3(x3 - x1)/r13^2)

        std::cout << "Actual Velocity: " << actualVel << "\tActual Position: " << actual << '\n';

        APPLICATION.createWindow(800, 800, "Stars");
        APPLICATION.addScene(new Scene);
        APPLICATION.runApplication();
        return 0;
}
