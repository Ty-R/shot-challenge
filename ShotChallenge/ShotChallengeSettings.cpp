#include "pch.h"
#include "ShotChallenge.h"

void ShotChallenge::RenderSettings() {
    if (ImGui::Checkbox("Enable plugin", &sgEnabled)) {
        cvarManager->getCvar("sg_enabled").setValue(sgEnabled);
    }

    if (ImGui::Checkbox("Enable score tracking (requires chat)", &scoreTrackingEnabled)) {
        cvarManager->getCvar("enable_scoring").setValue(scoreTrackingEnabled);
    }

    ImGui::Separator();

    if (ImGui::InputText("Shots File Path (absolute, no quotes)", &shotsFile)) {
        cvarManager->getCvar("shots_file_path").setValue(shotsFile);
    };

    if (ImGui::SliderInt("Shot count per game", &shotCount, 1, shots.size())) {
        cvarManager->getCvar("sc_shot_count").setValue(shotCount);
    }

    ImGui::Separator();

    ImGui::TextUnformatted("Seconds between shot re-shuffles. Lower values are quicker to re-shuffle but may be more inaccurate for multiplayer.");

    if (ImGui::SliderInt("Seed window", &seedWindow, 30, 1800)) {
        cvarManager->getCvar("seed_window").setValue(seedWindow);
    }

    if (ImGui::Button("Shuffle shots (per seed window)")) {
        gameWrapper->Execute([this](GameWrapper* gw) {
            cvarManager->executeCommand("shuffle");
        });
    }

    ImGui::Separator();

    std::string timestampStr = std::to_string(seed);
    ImGui::TextUnformatted(timestampStr.c_str(), nullptr);
}
