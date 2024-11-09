#pragma once

#include <imgui.h>


class DockingSpace {
public:
    DockingSpace();

    void Render() const;

private:
    ImGuiWindowFlags windowFlags;
    ImGuiDockNodeFlags dockNodeFlags;
};
