#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../views/dockingSpace.hpp"
#include "../views/optionsPanel.hpp"


class GuiController {
public:
    explicit GuiController(GLFWwindow* window);
    ~GuiController();

    void Render() const;

private:
    DockingSpace dockingSpace;
    OptionsPanel optionsPanel;
};
