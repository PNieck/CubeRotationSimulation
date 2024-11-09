#include <simulation/controllers/mainController.hpp>


MainController::MainController(GLFWwindow *window):
    guiController(window)
{

}


void MainController::Render() const
{
    guiController.Render();
}
