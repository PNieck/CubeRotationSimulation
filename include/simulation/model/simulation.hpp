#pragma once

#include "simulationProperties.hpp"

#include <glm/mat3x3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <mutex>


class Simulation {
public:
    explicit Simulation(const SimulationProperties& properties);

    void Update();

    glm::quat GetRotation();

    void SetProperties(const SimulationProperties& properties);

private:
    constexpr static float g = 9.81f;

    SimulationProperties properties;
    glm::mat3 I;
    glm::mat3 invI;

    glm::vec3 W;

    glm::quat Q;

    std::mutex rotationMutex;

    [[nodiscard]]
    glm::vec3 CalculateDiffW(const glm::vec3& W, const glm::vec3& N) const;

    [[nodiscard]]
    glm::quat CalculateDiffQ(const glm::vec3& W, const glm::quat& q) const;

    [[nodiscard]]
    glm::vec3 InBodyTorque(const glm::quat& q) const;;

    [[nodiscard]]
    glm::mat3 CubeInertiaTensor() const;

    [[nodiscard]]
    glm::mat3 CubeInvInertiaTensor() const;

    [[nodiscard]]
    float CubeMass() const;

    [[nodiscard]]
    float CubeDiagonalLen() const;

    [[nodiscard]]
    glm::quat InitRotationQuaternion() const;
};
