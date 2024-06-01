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
            shuffleShots();
            truncateShots();
        });

    cvarManager->registerCvar("enable_scoring", "0", "Enable score tracking")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
            scoreTrackingEnabled = cvar.getBoolValue();
        });

    loadShotFile();

    gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) { renderShot(canvas); });
    cvarManager->registerNotifier("next_shot", [this](std::vector<std::string> params) { nextShot(); }, "", 0);
    cvarManager->registerNotifier("prev_shot", [this](std::vector<std::string> params) { prevShot(); }, "", 0);
    cvarManager->executeCommand("bind " + nextKey + " next_shot");
    cvarManager->executeCommand("bind " + backKey + " prev_shot");

    gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.InitGame", std::bind(&ShotChallenge::onGameStart, this));

    gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.HUDBase_TA.OnChatMessage",
        std::bind(&ShotChallenge::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

// https://github.com/0xleft/trnslt/blob/master/trnslt.cpp#L69
void ShotChallenge::onMessage(ActorWrapper Caller, void* params) {
    if (!scoreTrackingEnabled) { return; }

    // Track player score through the use of chat events.
    // This is because chat events (at least, non-team ones)
    // are visible to all clients, and can be acted upon.

    if (params) {
        ChatMessage* message = (ChatMessage*)params;
        cvarManager->log(message->Message);
        if (message->PlayerName == nullptr) return;

        std::wstring wPlayerName(message->PlayerName);
        std::string playerName(wPlayerName.begin(), wPlayerName.end());
        std::wstring wMessage(message->Message);
        std::string msg(wMessage.begin(), wMessage.end());

        if (msg == "--") {
            playerScores[playerName]--;
        }
        else if (msg == "++") {
            playerScores[playerName]++;
        }
        else if (msg == "sc reset") {
            playerScores.clear();
        }
    }
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
    gameWrapper->UnhookEvent("Function TAGame.HUDBase_TA.OnChatMessagee");
}

void ShotChallenge::onGameStart() {
    shuffleShots();
    truncateShots();
    playerScores.clear();
    currentShotIndex = 0;
    cvarManager->log("Game started, shots shuffled..");
}

void ShotChallenge::shuffleShots() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Round down by one minute to try and account for slight differences across clients.
    // There's likely a better approach than using a timestamp (e.g. hooking into a game's ID, if such a thing exists)
    seed = currentTime - (currentTime % 60);

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
    if (!gameWrapper->IsInOnlineGame() && !gameWrapper->IsInFreeplay()) {
        return;
    }

    LinearColor colors;
    colors.R = 255;
    colors.G = 255;
    colors.B = 0;
    colors.A = 255;
    canvas.SetColor(colors);

    canvas.SetPosition(Vector2F{ 0, 400.0 });
    canvas.DrawString("Previous shot: " + backKey + ", Next shot: " + nextKey, 2.0f, 2.0f);

    if (!selectedShots.empty() && currentShotIndex < selectedShots.size()) {
        std::string count = std::to_string(currentShotIndex + 1) + "/" + std::to_string(selectedShots.size());
        std::string currentShot = "Shot " + count + ": " + selectedShots[currentShotIndex];

        canvas.SetPosition(Vector2F{ 0, 430.0 });
        canvas.DrawString(currentShot, 2.0f, 2.0f);
    }
    else {
        canvas.SetPosition(Vector2F{ 0, 430.0 });
        canvas.DrawString("Shots vector is empty or currentShotIndex out of bounds.", 2.0f, 2.0f);
    }

    if (!playerScores.empty() && scoreTrackingEnabled) {
        canvas.SetPosition(Vector2F{ 0, 490.0 });
        canvas.DrawString("Player Scores:", 2.0f, 2.0f);

        float yOffset = 520.0f;

        for (const auto& pair : playerScores) {
            std::string playerScoreStr = pair.first + ": " + std::to_string(pair.second);
            canvas.SetPosition(Vector2F{ 0, yOffset });
            canvas.DrawString(playerScoreStr, 2.0f, 2.0f);
            yOffset += 30.0f;
        }
    }
}
