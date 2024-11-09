#pragma once

#include "guiController.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class MainController {
public:
    MainController(GLFWwindow* window);

    void Render() const;

private:
    GuiController guiController;
};
