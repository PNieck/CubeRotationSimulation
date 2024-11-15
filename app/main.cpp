#include <simulation/window.hpp>

int main() {
    Window window(600, 400, "Cube rotation simulation");

    window.RunMessageLoop();

    return 0;
}
