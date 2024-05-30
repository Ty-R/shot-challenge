#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "GuiBase.h"
#include "version.h"

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

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
    std::vector<std::string> shots;
    std::string nextKey = "F11";
    std::string backKey = "F9";
    std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
    std::vector<std::string> selectedShots;
    std::string currentShot;
    std::string shotsFile = "shots.json";
    size_t currentShotIndex;
    time_t seed;
    int shotCount = 10;
    bool sgEnabled = true;

public:
	void RenderSettings() override;
};
