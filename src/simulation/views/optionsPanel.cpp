#include <simulation/views/optionsPanel.hpp>

#include <imgui.h>

#include "imgui_internal.h"


void OptionsPanel::Render() const {
    ImGui::DockBuilderAddNode();

    ImGui::Begin("Options");

    ImGui::Button("Test");

    ImGui::End();
}
