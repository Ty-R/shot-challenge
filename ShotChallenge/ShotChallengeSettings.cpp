#include "pch.h"
#include "ShotChallenge.h"

void ShotChallenge::RenderSettings() {
    if (ImGui::Checkbox("Enable plugin", &sgEnabled)) {
        cvarManager->getCvar("sg_enabled").setValue(sgEnabled);
    }

    ImGui::TextUnformatted("File path for loading in custom shot lists; must be absolute and without quotes.");

    if (ImGui::InputText("Shots File Path", &shotsFile)) {
        cvarManager->getCvar("shots_file_path").setValue(shotsFile);
    };

    if (ImGui::SliderInt("Shot count", &shotCount, 1, shots.size())) {
        cvarManager->getCvar("sc_shot_count").setValue(shotCount);
    }
}
