#include "simulation/views/visualization.hpp"

#include <imgui.h>


void Visualization::Render() const
{
    framebuffer.Use();
    glClear(GL_COLOR_BUFFER_BIT);
    Framebuffer::UseDefault();

    ImGui::Begin(WindowName());

    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(framebuffer.GetColorTextureId(), viewportPanelSize);

    ImGui::End();
}
