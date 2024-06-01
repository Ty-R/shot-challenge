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

    ImGui::TextUnformatted("Enable plugin to hook into chat events as a method of tracking score.");

    if (ImGui::Checkbox("Enable score tracking (requires chat)", &scoreTrackingEnabled)) {
        cvarManager->getCvar("enable_scoring").setValue(scoreTrackingEnabled);
    }

    ImGui::TextUnformatted("Controls");
    ImGui::TextUnformatted("F9: Previous shot");
    ImGui::TextUnformatted("F11: Next shot");
    ImGui::TextUnformatted("[chat] ++: Increment player score");
    ImGui::TextUnformatted("[chat] --: Decrement player score");
    ImGui::TextUnformatted("[chat] sc reset: Reset all scores");
}
