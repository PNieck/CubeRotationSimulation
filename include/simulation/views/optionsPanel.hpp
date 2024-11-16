#pragma once
#include "simulation/model/simulationProperties.hpp"

class MainController;


class OptionsPanel {
public:
    explicit OptionsPanel(MainController& controller);

    void Render();

private:
    MainController& controller;

    SimulationProperties properties;

    void RenderStartStopButton(bool simRuns);
    void RenderProperties(bool simRuns);
};
