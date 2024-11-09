#include <simulation/views/dockingSpace.hpp>

#include "imgui_internal.h"

#include "simulation/views/visualization.hpp"


DockingSpace::DockingSpace()
{
    windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    dockNodeFlags = ImGuiDockNodeFlags_PassthruCentralNode;
}


void DockingSpace::Render() const
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", nullptr, windowFlags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID DockSpaceId = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(DockSpaceId, ImVec2(0.0f, 0.0f), dockNodeFlags);

        static auto first_time = true;
        if (first_time)
        {
            first_time = false;

            ImGui::DockBuilderRemoveNode(DockSpaceId); // clear any previous layout
            ImGui::DockBuilderAddNode(DockSpaceId, dockNodeFlags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(DockSpaceId, viewport->Size);

            const auto rightDockId = ImGui::DockBuilderSplitNode(DockSpaceId, ImGuiDir_Right, 0.25f, nullptr, &DockSpaceId);
            ImGui::DockBuilderDockWindow("Options", rightDockId);
            ImGui::DockBuilderDockWindow(Visualization::WindowName(), DockSpaceId);
            ImGui::DockBuilderFinish(DockSpaceId);
        }
    }

    ImGui::End();
}
