#include <simulation/model/model.hpp>


Model::Model(const SimulationProperties &properties):
    simulation(properties),
    timedLoop(static_cast<int>(properties.deltaT * 1000.f), [this] { simulation.Update(); })
{
}


void Model::SetProperties(const SimulationProperties &properties)
{
    const bool simulationRuns = SimulationRunning();

    if (simulationRuns)
        StopSimulation();

    simulation.SetProperties(properties);

    if (simulationRuns)
        StartSimulation();
}
