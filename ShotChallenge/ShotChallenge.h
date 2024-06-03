#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "GuiBase.h"
#include "version.h"

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

struct ChatMessage {
    void* PRI;
    void* Team;
    wchar_t* PlayerName;
    uint8_t PlayerNamePadding[0x8];
    wchar_t* Message;
    uint8_t MessagePadding[0x8];
    uint8_t ChatChannel;
    unsigned long bPreset: 1;
};

class ShotChallenge: public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase {
	void onLoad() override;
	void onUnload() override;
    void renderShot(CanvasWrapper canvas);
    void shuffleShots();
    void nextShot();
    void prevShot();
    void onGameStart();
    void truncateShots();
    void loadShotFile();
    void onMessage(ActorWrapper, void*);
    std::vector<std::string> shots;         // Base list
    std::vector<std::string> shuffledShots; // Copy of base, to shuffle
    std::vector<std::string> selectedShots; // Sub-set of shuffled shots to show in-game
    int currentShotIndex;                // Index of the currently selected shot
    std::string nextKey = "F11";
    std::string backKey = "F9";
    std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
    std::string shotsFile = "shots.json";
    int seed;
    int shotCount = 10;
    int seedWindow = 120;
    bool sgEnabled = true;
    bool scoreTrackingEnabled = true;
    std::map<std::string, int> playerScores;

public:
	void RenderSettings() override;
};
