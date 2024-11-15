#include <simulation/model/model.hpp>


Model::Model(const SimulationProperties &properties):
    simulation(properties),
    timedLoop(static_cast<int>(properties.deltaT * 1000.f), [this] { simulation.Update(); })
{
}
