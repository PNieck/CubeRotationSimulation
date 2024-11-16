#include <simulation/model/simulation.hpp>

#include <glm/gtx/string_cast.hpp>


Simulation::Simulation(const SimulationProperties &properties)
{
    SetProperties(properties);
}


void Simulation::Update()
{
    std::lock_guard guard(rotationMutex);

    auto N = InBodyTorque(Q);

    auto kW1 = CalculateDiffW(W, N);
    auto kQ1 = CalculateDiffQ(W, Q);

    auto W2 = W + kW1*properties.deltaT/2.f;
    auto Q2 = glm::normalize(Q + kQ1*properties.deltaT/2.f);
    N = InBodyTorque(Q2);
    auto kW2 = CalculateDiffW(W2, N);
    auto kQ2 = CalculateDiffQ(W2, Q2);

    auto W3 = W + kW2*properties.deltaT/2.f;
    auto Q3 = glm::normalize(Q + kQ2*properties.deltaT/2.f);
    N = InBodyTorque(Q3);
    auto kW3 = CalculateDiffW(W3, N);
    auto kQ3 = CalculateDiffQ(W3, Q3);

    auto W4 = W + kW3*properties.deltaT;
    auto Q4 = glm::normalize(Q + kQ3*properties.deltaT);
    N = InBodyTorque(Q4);
    auto kW4 = CalculateDiffW(W4, N);
    auto kQ4 = CalculateDiffQ(W4, Q4);

    auto Wt = properties.deltaT * (kW1 + 2.f*kW2 + 2.f*kW3 + kW4) / 6.f;
    W = W + Wt;

    auto Qt = properties.deltaT * (kQ1 + 2.f*kQ2 + 2.f*kQ3 + kQ4) / 6.f;
    Q = glm::normalize(Q + Qt);

    // Euler's method
    // const auto N = InBodyTorque(Q);
    // W = W + properties.deltaT*(CalculateDiffW(W, N));
    // Q = normalize(Q + properties.deltaT*(CalculateDiffQ(W, Q)));
}


glm::quat Simulation::GetRotation()
{
    std::lock_guard guard(rotationMutex);
    return Q;
}


void Simulation::SetProperties(const SimulationProperties &properties)
{
    std::lock_guard guard(rotationMutex);

    this->properties = properties;

    I = CubeInertiaTensor();
    invI = glm::inverse(I);
    W = glm::vec3(0.f, properties.angularVelocity, 0.f);
    Q = InitRotationQuaternion();
}


glm::vec3 Simulation::CalculateDiffW(const glm::vec3 &W, const glm::vec3 &N) const
{
    return invI * (N + cross(I*W, W));
}


glm::quat Simulation::CalculateDiffQ(const glm::vec3 &W, const glm::quat& q) const
{
    return q * glm::quat(0.f, W / 2.f);
}


glm::vec3 Simulation::InBodyTorque(const glm::quat &q) const
{
    if (!properties.gravity)
        return glm::vec3(0.f);

    const glm::vec3 globalGravityForce = CubeMass() * glm::vec3(0.f, -g, 0.f);
    const auto r = glm::vec3(0.f, CubeDiagonalLen()/2.f, 0.f);
    const auto rotatedGravity = glm::rotate(conjugate(q), globalGravityForce);

    return cross(r, rotatedGravity);
}


glm::mat3 Simulation::CubeInertiaTensor() const
{
    const float s = properties.cubeEdgeLen;
    const float s5 = s * s * s * s * s;

    const float diag = 2.f*s5/3.f;
    const float ndiag = -s5/4.f;

    glm::mat3 result(
         diag, ndiag, ndiag,
        ndiag,  diag, ndiag,
        ndiag, ndiag,  diag
    );

    result *= properties.cubeDensity;

    const auto rot = glm::rotation(glm::normalize(glm::vec3(1.f, 1.f, 1.f)), glm::vec3(0.f, 1.f, 0.f));
    const auto rotMat = glm::toMat3(rot);

    return rotMat * result * glm::transpose(rotMat);
}


glm::mat3 Simulation::CubeInvInertiaTensor() const
{
    const float s = properties.cubeEdgeLen;
    const float s5 = s * s * s * s * s;

    const float diag = 2.f*s5/3.f;
    const float ndiag = -s5/4.f;

    glm::mat3 result(
         diag, ndiag, ndiag,
        ndiag,  diag, ndiag,
        ndiag, ndiag,  diag
    );

    result *= properties.cubeDensity;

    result = glm::inverse(result);

    const auto rot = rotation(glm::normalize(glm::vec3(1.f, 1.f, 1.f)), glm::vec3(0.f, 1.f, 0.f));
    const auto rotMat = glm::toMat3(rot);

    return glm::transpose(rotMat) * result * rotMat;
}


float Simulation::CubeMass() const
{
    const float v = properties.cubeEdgeLen * properties.cubeEdgeLen * properties.cubeEdgeLen;
    return  v * properties.cubeDensity;
}


float Simulation::CubeDiagonalLen() const
{
    return properties.cubeEdgeLen * std::sqrt(3.f);
}


glm::quat Simulation::InitRotationQuaternion() const
{
    return glm::angleAxis(properties.angle, glm::vec3(1.f, 0.f, 0.f));
}
