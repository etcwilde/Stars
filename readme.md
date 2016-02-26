# Stars

## Initial Phase

The initial phase is to build a basic version that has a limited number of stars. We
will begin with three stars to determine how the base system will work. The initial
phase will not be using any acceleration structures or fancy algorithm, simply the
raw mathematics and physics to calculate the accelerations for each star. From that,
we will look at various ODE solvers including Romberg and RK4 to determine which is
more suitable for our application and provides better visual results in less time.

## Secondary Phase

The Secondary phase is to expand the system for much larger systems with many stars.
We will have to determine a more generalized method of calculating the derivatives
as we won't know how many stars to calculate for so implementing a fixed function
will no longer work.

As there are many stars, and the acceleration calculations are a O(n^2) operation,
acceleration structures may prove useful for limiting the amount of calculations to
perform. First, we can look into limiting the number of stars that we are involving
in the Lagrangian by using a radius search for all stars that are within a certain
distance of the others. Beyond that distance, the stars are far enough away from
each other that their gravitational pulls are negligible.
