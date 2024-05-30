#include "pch.h"
#include <random>
#include "ShotChallenge.h"

BAKKESMOD_PLUGIN(ShotChallenge, "Shot Generator Plugin", "0.1", PLUGINTYPE_FREEPLAY)

void ShotChallenge::onLoad() {
    cvarManager->registerCvar("sg_enabled", "1", "Enable SG")
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		    sgEnabled = cvar.getBoolValue();
		});

    cvarManager->registerCvar("sc_shot_count", "10", "Number of shots")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
            shotCount = cvar.getIntValue();
            truncateShots();
        });

    cvarManager->registerCvar("shots_file_path", "shots.json", "Shots filepath")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
            shotsFile = cvar.getStringValue();
            loadShotFile();
            truncateShots();
        });

    loadShotFile();
    shuffleShots();
    truncateShots();

    gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) { renderShot(canvas); });
    cvarManager->registerNotifier("next_shot", [this](std::vector<std::string> params) { nextShot(); }, "", 0);
    cvarManager->registerNotifier("prev_shot", [this](std::vector<std::string> params) { prevShot(); }, "", 0);
    cvarManager->executeCommand("bind " + nextKey + " next_shot");
    cvarManager->executeCommand("bind " + backKey + " prev_shot");

    gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.InitGame", std::bind(&ShotChallenge::onGameStart, this));
}

void ShotChallenge::loadShotFile() {
    std::ifstream file(shotsFile);
    if (file.is_open()) {
        json shotsJson;
        file >> shotsJson;
        shots = shotsJson.get<std::vector<std::string>>();
        cvarManager->log("Loaded shots from: " + shotsFile);
        file.close();
    }
    else {
        cvarManager->log("Failed to open file: " + shotsFile);
    }
}

void ShotChallenge::truncateShots() {
    // OOB territory.
    // Attemnpt to preserve shotCount but only if it's within bounds
    // Reset currentShotIndex as that's probably what we want anyway

    if (shotCount > shots.size()) {
        shotCount = shots.size();
    }

    selectedShots.assign(shots.begin(), shots.begin() + shotCount);
    currentShotIndex = 0;
}

void ShotChallenge::onUnload() {
    gameWrapper->UnhookEvent("Function TAGame.GameEvent_Soccar_TA.InitGame");
}

void ShotChallenge::onGameStart() {
    shuffleShots();
    currentShotIndex = 0;
    cvarManager->log("Game started, shots shuffled..");
}

void ShotChallenge::shuffleShots() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Round down to the nearest 30 minutes
    seed = currentTime - (currentTime % 1800);

    std::shuffle(shots.begin(), shots.end(), std::default_random_engine(static_cast<unsigned>(seed)));
}

void ShotChallenge::nextShot() {
    currentShotIndex = (currentShotIndex + 1) % selectedShots.size();
}

void ShotChallenge::prevShot() {
    if (currentShotIndex == 0) {
        currentShotIndex = selectedShots.size() - 1;
    }
    else {
        --currentShotIndex;
    }
}

void ShotChallenge::renderShot(CanvasWrapper canvas) {
    if (!sgEnabled) { return; }

    LinearColor colors;
    colors.R = 255;
    colors.G = 255;
    colors.B = 0;
    colors.A = 255;
    canvas.SetColor(colors);

    std::string seedString = "Seed: " + std::to_string(seed);
    canvas.DrawString(seedString, 2.0f, 2.0f);

    canvas.SetPosition(Vector2F{ 0, 30.0 });
    canvas.DrawString("Previous shot: " + backKey + ", Next shot: " + nextKey, 2.0f, 2.0f);

    if (!selectedShots.empty() && currentShotIndex < selectedShots.size()) {
        std::string count = std::to_string(currentShotIndex + 1) + "/" + std::to_string(selectedShots.size());
        std::string currentShot = "Shot " + count + ": " + selectedShots[currentShotIndex];

        canvas.SetPosition(Vector2F{ 0, 80.0 });
        canvas.DrawString(currentShot, 2.0f, 2.0f);
    }
    else {
        canvas.DrawString("Shots vector is empty or currentShotIndex out of bounds.", 2.0f, 2.0f);
    }    
}
