#pragma once

#include <glm/glm.hpp>


class SimulationProperties {
public:
    float cubeEdgeLen = 1.f;
    float cubeDensity = 1.f;
    float deltaT = 0.001;
    bool gravity = true;
    float angle = glm::radians(5.f);
    float angularVelocity = 32.f;
};
