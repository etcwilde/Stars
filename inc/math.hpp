#ifndef __STARS_MATH_HPP
#define __STARS_MATH_HPP

#include <iostream>
/**
 * The math ODE Solvers are useful for single-level ODE solvers.
 * If you are working with acceleration and trying to get to position, use
 * the solvers from the physics namespace. They will work a little faster
 * for that.
 */
namespace math
{
        template <typename Derived>
                class ODESolver {
                        public:
                                virtual ~ODESolver() {}

                                // Note: If Vector is a scalar type and the function
                                // takes a scalar, it will work fine too
                                //
                                // function is of the form:
                                //      f(x_i, y_i) :
                                //              x is the independent scalar
                                //                      -- Might work with vectors?
                                //                         Not sure what this would
                                //                         imply though. Would be
                                //                         neat!
                                //              y is the dependent vector
                                template <typename Vector, typename Scalar, typename Function>
                                        void evaluate(
                                                        Function f,
                                                        Scalar x,
                                                        const Vector& y,
                                                        Scalar h,
                                                        Vector& out)
                                        {
                                                static_cast<Derived *>(this)->evaluate(f, x, y, h, out);
                                        }
                };

        class ODEEuler: ODESolver<ODEEuler>
        {
                public:
                        template <typename Vector, typename Scalar, typename Function>
                                void evaluate(
                                                Function f,
                                                Scalar x,
                                                const Vector& y,
                                                Scalar h,
                                                Vector& out)
                                {
                                        out = y + f(x, y) * h;
                                }
        };

        // Hune's method with single corrector
        class ODEHune: ODESolver<ODEHune>
        {
                public:
                        template <typename Vector, typename Scalar, typename Function>
                                void evaluate(
                                                Function f,
                                                Scalar x,
                                                const Vector& y,
                                                Scalar h,
                                                Vector& out)
                                {
                                        const Vector k1 = f(x, y);
                                        const Scalar halfH = Scalar(0.5) * h;
                                        const Vector k2 = f(x + h, y + k1 * h);
                                        out = y + (k1 + k2) * halfH;
                                }
        };


        class ODEMidPoint: ODESolver<ODEMidPoint>
        {
                public:
                        template <typename Vector, typename Scalar, typename Function>
                                void evaluate(
                                                Function f,
                                                Scalar x,
                                                const Vector& y,
                                                Scalar h,
                                                Vector& out)
                                {
                                        const Vector k1 = f(x, y);
                                        const Scalar halfH = Scalar(0.5) * h;
                                        const Vector k2 = f(x + halfH, y + k1 * halfH);
                                        out = y + k2 * h;
                                }
        };

        class ODERalston: ODESolver<ODERalston>
        {
                public:
                        template <typename Vector, typename Scalar, typename Function>
                                void evaluate(
                                                Function f,
                                                Scalar x,
                                                const Vector& y,
                                                Scalar h,
                                                Vector& out)
                                {
                                        const Vector k1 = f(x, y);
                                        const Scalar threeQuarterH = Scalar(0.75) * h;
                                        const Vector k2 = f(x + threeQuarterH,
                                                        y + threeQuarterH * k1);
                                        out = y + (Scalar(1/3.) * k1 + Scalar(2/3.) * k2) * h;
                                }
        };

        class ODERK3: ODESolver<ODERK3>
        {
                public:
                        template <typename Vector, typename Scalar, typename Function>
                                void evaluate(
                                                Function f,
                                                Scalar x,
                                                const Vector& y,
                                                Scalar h,
                                                Vector& out)
                                {
                                        const Vector k1 = f(x, y);
                                        const Scalar halfH = Scalar(0.5) * h;
                                        const Vector k2 = f(x + halfH, y + halfH * k1);
                                        const Vector k1h = k1 * h;
                                        const Vector k3 = f(x + h, y - k1h + 2. * k2 * h);
                                        out = y + Scalar(1/6.) * (k1 + 4. * k2 + k3) * h;
                                }
        };

        class ODERK4: ODESolver<ODERK4>
        {
                public:
                        template <typename Vector, typename Scalar, typename Function>
                                void evaluate(
                                                Function f,
                                                Scalar x,
                                                const Vector& y,
                                                Scalar h,
                                                Vector& out)
                                {
                                        const Scalar halfH = Scalar(0.5) * h;
                                        const Vector k1 = f(x, y);
                                        const Vector k2 = f(x + halfH, y + halfH * k1);
                                        const Vector k3 = f(x + halfH, y + halfH * k2);
                                        const Vector k4 = f(x + h, y + k3 * h);
                                        out = y + Scalar(1/6.) * h * (k1 + 2. * k2 + 2. * k3 + k4);
                                }
        };


}

namespace physics
{
        template <typename Derived>
                class ODESolver {
                        public:
                                virtual ~ODESolver() {}

                                template <typename Vector, typename Scalar, typename Function>
                                        void evaluate(
                                                        Function accelFunc,
                                                        Scalar x,
                                                        const Vector& initialPos,
                                                        const Vector& initialVel,
                                                        Scalar h,
                                                        Vector& pos,
                                                        Vector& vel)
                                        {
                                                static_cast<Derived*>(this)->evaluate(accelFunc, x, initialPos,
                                                                initialVel, h, pos, vel);
                                        }
                };

        class ODEHune: ODESolver<ODEHune>
        {
                        public:
                                template <typename Vector, typename Scalar, typename Function>
                                        void evaluate(
                                                        Function accelFunc,
                                                        Scalar x,
                                                        const Vector& initialPos,
                                                        const Vector& initialVel,
                                                        Scalar h,
                                                        Vector& pos,
                                                        Vector& vel)
                                        {
                                                Vector velk2;
                                                math::ODEHune solver;
                                                solver.evaluate(accelFunc, x, initialVel, h, vel);
                                                solver.evaluate(accelFunc, x + h, vel * h, h, velk2);
                                                pos = initialPos + (vel + velk2) * Scalar(0.5) * h;
                                        }
        };

        class ODEMidPoint: ODESolver<ODEHune>
        {
                        public:
                                template <typename Vector, typename Scalar, typename Function>
                                        void evaluate(
                                                        Function accelFunc,
                                                        Scalar x,
                                                        const Vector& initialPos,
                                                        const Vector& initialVel,
                                                        Scalar h,
                                                        Vector& pos,
                                                        Vector& vel)
                                        {
                                                Vector velk2;
                                                const Scalar halfH = Scalar(0.5) * h;
                                                math::ODEMidPoint solver;
                                                solver.evaluate(accelFunc, x, initialVel, h, vel);
                                                solver.evaluate(accelFunc, x + halfH, vel * halfH , h, velk2);
                                                pos = initialPos + velk2 * h;
                                        }
        };

        class ODERalston: ODESolver<ODEHune>
        {
                        public:
                                template <typename Vector, typename Scalar, typename Function>
                                        void evaluate(
                                                        Function accelFunc,
                                                        Scalar x,
                                                        const Vector& initialPos,
                                                        const Vector& initialVel,
                                                        Scalar h,
                                                        Vector& pos,
                                                        Vector& vel)
                                        {
                                                Vector velk2;
                                                const Scalar threeQuarterH = Scalar(0.75) * h;
                                                math::ODERalston solver;
                                                solver.evaluate(accelFunc, x, initialVel, h, vel);
                                                solver.evaluate(accelFunc, x + threeQuarterH, vel * threeQuarterH, h, velk2);
                                                pos = initialPos + (Scalar(1/3.) * vel + Scalar(2/3.) * velk2) * h;
                                        }

        };

        class ODERK3: ODESolver<ODERK3>
        {
                public: // x is time, initialVel is velocity at time x
                        template <typename Vector, typename Scalar, typename Function>
                                void evaluate(
                                                Function accelFunc,
                                                Scalar x,
                                                const Vector& initialPos,
                                                const Vector& initialVel,
                                                Scalar h,
                                                Vector& pos,
                                                Vector& vel)
                                {
                                        /*
                                        const Scalar halfH = Scalar(0.5) * h;
                                        const Vector vk11 = accelFunc(x, initialVel);
                                        const Vector vk12 = accelFunc(x + halfH, initialVel + halfH * vk11);
                                        const Vector vk13 = accelFunc(x + h, initialVel - vk11 * h + 2. * vk12 * h);
                                        const Vector pk1 = Scalar(1/6.) * (vk11 + 4. * vk12 + vk13) * h;
                                        const Vector vk21 = accelFunc(x + halfH, initialVel + halfH * vk11);
                                        const Vector vk22 = accelFunc(x + 2 * halfH, initialVel + halfH * vk11 + halfH  * vk21);
                                        const Vector vk23 = accelFunc(x + halfH + h, initialVel + halfH * vk11 - vk21 * h + 2 * vk22 *h);
                                        const Vector pk2 = Scalar(1/6.) * (vk21 + 4. * vk22 + vk23) * h;
                                        const Vector vk31 = accelFunc(x + h, initialVel - vk11 * h + 2 * vk22 * h);
                                        const Vector vk32 = accelFunc(x + h + halfH, initialVel - vk11 * h + 2 * vk22 * h + halfH * vk31);
                                        const Vector vk33 = accelFunc(x + 2 * h, initialVel - vk11 * h + 2 * vk22 * h - vk31 + 2 * vk32 * h);
                                        const Vector pk3 = Scalar(1/6.) * (vk31 + 4. * vk32 + vk33) * h;
                                        */

                                        const Scalar h6 = Scalar(1/6.) * h;
                                        const Scalar halfH = Scalar(0.5) * h;
                                        const Vector vk11 = accelFunc(x, initialVel);
                                        const Vector vk12 = accelFunc(x + halfH, initialVel + halfH * vk11);
                                        const Vector vk13 = accelFunc(x + h, initialVel - vk11 * h + Scalar(2) * vk12 * h);
                                        const Vector pk1 = initialVel + h6 * (vk11 + Scalar(4) * vk12 + vk13);
                                        //const Vector vk21 = vk12;
                                        const Vector vk21 = accelFunc(x + halfH, initialVel + halfH * pk1);
                                        const Vector vk22 = accelFunc(x + h, initialVel + halfH * pk1 + halfH  * vk21);
                                        const Vector vk23 = accelFunc(x + halfH + h, initialVel + halfH * pk1 - vk21 * h + Scalar(2) * vk22 *h);
                                        const Vector pk2 = pk1 + h6 * (vk21 + Scalar(4) * vk22 + vk23);
                                        const Vector vk31 = accelFunc(x + h, initialVel - pk1 * h + Scalar(2) * pk2 * h);
                                        const Vector vk32 = accelFunc(x + h + halfH, initialVel - pk1 * h + Scalar(2) * pk2 * h + halfH * vk31);
                                        const Vector vk33 = accelFunc(x + Scalar(2) * h, initialVel - pk1 * h + Scalar(2) * pk2 * h - vk31 + Scalar(2) * vk32 * h);
                                        const Vector pk3 = pk2 + h6 * (vk31 + Scalar(4) * vk32 + vk33);

                                        vel = pk1;
                                        pos =  initialPos + h6 * (pk1 + Scalar(4) * pk2 + pk3) * Scalar(0.5);
                                }

        };

        class ODERK4: ODESolver<ODERK4>
        {
                public:
                        template <typename Vector, typename Scalar, typename Function>
                                void evaluate(
                                                Function accelFunc,
                                                Scalar x,
                                                const Vector& initialPos,
                                                const Vector& initialVel,
                                                Scalar h,
                                                Vector& pos,
                                                Vector& vel)
                                {
                                        const Scalar halfH = Scalar(0.5) * h;
                                        const Scalar h6 = Scalar(1./6.) * h;

                                        const Vector vk11 = accelFunc(x, initialVel);
                                        const Vector vk12 = accelFunc(x + halfH, initialVel + halfH * vk11);
                                        const Vector vk13 = accelFunc(x + halfH, initialVel + halfH * vk12);
                                        const Vector vk14 = accelFunc(x + h, initialPos + h * vk13);
                                        const Vector pk1 = initialVel + h6 * (vk11 + Scalar(2.) * vk12 + Scalar(2.) * vk13 + vk14);
                                        vel = pk1;
                                        const Vector halfPk1 = halfH * vk11;
                                        const Vector vk21 = accelFunc(x + halfH, initialVel + halfPk1);
                                        const Vector vk22 = accelFunc(x + h, initialVel + halfPk1 + halfH * vk21);
                                        const Vector vk23 = accelFunc(x + h, initialVel + halfPk1 + halfH * vk22);
                                        const Vector vk24 = accelFunc(x + halfH + h, initialVel + halfPk1 + halfH * vk23);
                                        const Vector pk2 = pk1 + h6 * (vk21 + Scalar(2.) * vk22 + Scalar(2.) * vk23 + vk24);

                                        const Vector halfPk2 = halfH * pk2;
                                        const Vector vk31 = accelFunc(x + halfH, initialVel + halfPk2);
                                        const Vector vk32 = accelFunc(x + h, initialVel + halfPk2 + halfH * vk31);
                                        const Vector vk33 = accelFunc(x + h, initialVel + halfPk2 + halfH * vk32);
                                        const Vector vk34 = accelFunc(x + halfH + h, initialVel + halfPk2 + h * vk33);
                                        const Vector pk3 = pk2 + h6 * (vk31 + Scalar(2.)* (vk32 + vk33) + vk34);

                                        const Vector hPk3 = h * pk3;
                                        const Vector vk41 = accelFunc(x + h, initialVel + hPk3);
                                        const Vector vk42 = accelFunc(x + h + halfH, initialVel + hPk3 + halfH * vk41);
                                        const Vector vk43 = accelFunc(x + h + halfH, initialVel + hPk3 + halfH * vk42);
                                        const Vector vk44 = accelFunc(x + Scalar(2) * h, initialVel + hPk3 + h * vk43);
                                        const Vector pk4 = pk3 + h6 * (vk41 + Scalar(2.) * (vk42 + vk43) + vk44);
                                        pos = initialPos + h6 * (pk1 + Scalar(2)*(pk2 +  pk3) + pk4) * Scalar(0.5);
                                }


        };


}
#endif//__STARS_MATH_HPP
