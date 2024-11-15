#pragma once

#include "simulation.hpp"
#include "timedLoop.hpp"


class Model {
public:
    explicit Model(const SimulationProperties& properties = SimulationProperties());

    void StartSimulation()
        { timedLoop.Start(); }

    void StopSimulation()
        { timedLoop.End(); }

    [[nodiscard]]
    bool SimulationRunning() const
        { return timedLoop.IsRunning(); }

    void UpdateSimulation()
        { simulation.Update(); }

    glm::quat GetCubeRotation()
        { return simulation.GetRotation(); }

private:
    Simulation simulation;
    TimedLoop timedLoop;
};
