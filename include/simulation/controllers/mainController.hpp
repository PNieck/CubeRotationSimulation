#pragma once

#include "mouseState.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../views/dockingSpace.hpp"
#include "../views/optionsPanel.hpp"
#include "../views/visualization.hpp"

#include "../model/model.hpp"


class MainController {
public:
    explicit MainController(GLFWwindow* window);
    ~MainController();

    void Update();

    void StartSimulation()
        { model.StartSimulation(); }

    void StopSimulation()
        { model.StopSimulation(); }

    void UpdateSimulation()
        { model.UpdateSimulation(); }

    void SetProperties(const SimulationProperties& properties);

    [[nodiscard]]
    bool SimulationIsRunning() const
        { return model.SimulationRunning(); }

    void Render();

    void SizeChanged(const int width, const int height)
        { visualization.ResizeWindow(width, height); }

    void MouseClicked(const MouseButton button)
        { mouseState.ButtonClicked(button); }

    void MouseReleased(const MouseButton button)
        { mouseState.ButtonReleased(button); }

    void MouseMoved(int x, int y);
    void ScrollMoved(int offset);

private:
    MouseState mouseState;

    DockingSpace dockingSpace;
    OptionsPanel optionsPanel;
    Visualization visualization;

    Model model;
};
