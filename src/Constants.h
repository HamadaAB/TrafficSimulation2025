//
// Created by Hamad on 15/03/2025.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H




namespace Constants {
    // From Appendix B.6
    constexpr double VehicleLength = 4.0;          // l
    constexpr double MaxAbsoluteSpeed = 16.6;      // Vmax
    constexpr double MaxAcceleration = 1.44;       // amax
    constexpr double MaxDeceleration = 4.61;       // bmax
    constexpr double MinFollowingDistance = 4.0;   // fmin
    constexpr double SimulationTimeStep = 0.0166;  // Δt
    constexpr double DecelerationDistance = 50.0;  // Δxs
    constexpr double StopDistance = 15.0;          // Δxs0
    constexpr double SlowdownFactor = 0.4;         // s
}

#endif //CONSTANTS_H
