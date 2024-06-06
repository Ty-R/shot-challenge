#include "pch.h"
#include <random>
#include "ShotChallenge.h"

BAKKESMOD_PLUGIN(ShotChallenge, "Shot Generator Plugin", "0.1", PERMISSION_ALL)

void ShotChallenge::onLoad() {
    defaultShots = {
        "Double touch",
        "Flip reset",
        "45 degree flick",
        "Ground pinch",
        "Wall pinch",
        "Ground to air dribble",
        "Off the wall air dribble",
        "Musty flick",
        "Ceiling pinch",
        "180 backflip flick"
    };

    cvarManager->registerCvar("sg_enabled", "1", "Enable SG")
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		    sgEnabled = cvar.getBoolValue();
		});

    cvarManager->registerCvar("sc_shot_count", "10", "Number of shots")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
            shotCount = cvar.getIntValue();
            truncateShots();
        });

    cvarManager->registerCvar("shots_file_path", shotsFile, "Shots filepath")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
            shotsFile = cvar.getStringValue();
            loadShotFile();
            shuffleShots();
            truncateShots();
        });
    
    gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) { renderShot(canvas); });
    cvarManager->registerNotifier("next_shot", [this](std::vector<std::string> params) { nextShot(); }, "", 0);
    cvarManager->registerNotifier("prev_shot", [this](std::vector<std::string> params) { prevShot(); }, "", 0);
    cvarManager->executeCommand("bind " + nextKey + " next_shot");
    cvarManager->executeCommand("bind " + backKey + " prev_shot");

    gameWrapper->HookEvent("Function GameEvent_Soccar_TA.WaitingForPlayers.EndState", [this](std::string eventName) {
        resetPlayerScores();
        shuffleShots();
        truncateShots();
        playerScores.clear();
        currentShotIndex = 0;
    });

    gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.HUDBase_TA.OnChatMessage",
        std::bind(&ShotChallenge::onMessage, this, std::placeholders::_1, std::placeholders::_2));

    if (shotsFile.empty()) {
        cvarManager->log("Loading default shots");
        shots = defaultShots;
    }
}

// https://github.com/0xleft/trnslt/blob/master/trnslt.cpp#L69
void ShotChallenge::onMessage(ActorWrapper Caller, void* params) {
    // Track player score through the use of chat events.
    // This is because chat events (at least, non-team ones)
    // are visible to all clients, and can be acted upon.

    if (params) {
        ChatMessage* message = (ChatMessage*)params;
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
            resetPlayerScores();
        }
    }
}

void ShotChallenge::loadShotFile() {
    std::ifstream file(shotsFile);
    if (file.is_open()) {
        json shotsJson;
        file >> shotsJson;
        shots = shotsJson.get<std::vector<std::string>>();
        cvarManager->log("Loaded shot file successfully");
        file.close();
    }
    else {
        cvarManager->log("Failed to open file or no shot file specified; loading default shot list");
        shots = defaultShots;
    }
}

int ShotChallenge::trimmedGUID() {
    ServerWrapper game = gameWrapper->GetCurrentGameState();

    if (game.IsNull()) { return 0; }

    matchGUID = game.GetMatchGUID();
    intifiedGUID = std::regex_replace(matchGUID, std::regex(R"([\D])"), "");

    return std::stoi(intifiedGUID.substr(0, 5));
}

void ShotChallenge::shuffleShots() {
    shuffledShots = shots;
    seed = trimmedGUID();
    std::shuffle(shuffledShots.begin(), shuffledShots.end(), std::default_random_engine(seed));
}

void ShotChallenge::truncateShots() {
    if (shuffledShots.empty()) { return; }

    if (shotCount > shuffledShots.size()) {
        shotCount = shuffledShots.size();
    }

    selectedShots.assign(shuffledShots.begin(), shuffledShots.begin() + shotCount);
    currentShotIndex = 0;
}

void ShotChallenge::resetPlayerScores() {
    playerScores.clear();
    ServerWrapper game = gameWrapper->GetCurrentGameState();

    auto players = game.GetPRIs();

    for (auto player : players) {
        std::string playerName = player.GetPlayerName().ToString();
        playerScores[playerName] = 0;
    }
}

void ShotChallenge::nextShot() {
    if (selectedShots.empty()) { return; }

    currentShotIndex = (currentShotIndex + 1) % selectedShots.size();
}

void ShotChallenge::prevShot() {
    if (selectedShots.empty()) { return; }

    if (currentShotIndex == 0) {
        currentShotIndex = selectedShots.size() - 1;
    }
    else {
        --currentShotIndex;
    }
}

void ShotChallenge::renderShot(CanvasWrapper canvas) {
    if (!sgEnabled) { return; }
    if (!gameWrapper->IsInOnlineGame()) { return; }

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
        canvas.DrawString("Shots not loaded.", 2.0f, 2.0f);
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

void ShotChallenge::onUnload() {
    gameWrapper->UnhookEvent("Function TAGame.HUDBase_TA.OnChatMessage");
    gameWrapper->UnhookEvent("Function GameEvent_Soccar_TA.WaitingForPlayers.BeginState");
    // Control binds?
}
