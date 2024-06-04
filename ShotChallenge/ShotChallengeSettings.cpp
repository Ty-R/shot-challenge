#include "pch.h"
#include "ShotChallenge.h"

void ShotChallenge::RenderSettings() {
    if (ImGui::Checkbox("Enable plugin", &sgEnabled)) {
        cvarManager->getCvar("sg_enabled").setValue(sgEnabled);
    }

    ImGui::Separator();

    if (ImGui::InputText("Shots File Path (absolute, no quotes)", &shotsFile)) {
        cvarManager->getCvar("shots_file_path").setValue(shotsFile);
    };

    if (ImGui::SliderInt("Shot count per game", &shotCount, 1, shots.size())) {
        cvarManager->getCvar("sc_shot_count").setValue(shotCount);
    }

    ImGui::Separator();

    std::string timestampStr = std::to_string(seed);
    ImGui::TextUnformatted(timestampStr.c_str(), nullptr);
}
