#include <simulation/views/optionsPanel.hpp>

#include <imgui.h>

#include "imgui_internal.h"
#include "simulation/controllers/mainController.hpp"


void OptionsPanel::Render() const
{
    ImGui::DockBuilderAddNode();

    ImGui::Begin("Options");

    const bool simRuns = controller.SimulationIsRunning();

    ImGui::BeginDisabled(simRuns);
    if (ImGui::Button("Start")) {
        controller.StartSimulation();
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    ImGui::BeginDisabled(!simRuns);
    if (ImGui::Button("Stop")) {
        controller.StopSimulation();
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(simRuns);
    if (ImGui::Button("Update")) {
        controller.UpdateSimulation();
    }
    ImGui::EndDisabled();

    ImGui::End();
}
