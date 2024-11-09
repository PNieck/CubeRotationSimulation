#include <simulation/window.hpp>

int main() {
    Window window(600, 400, "Spring simulation");

    window.RunMessageLoop();

    return 0;
}
