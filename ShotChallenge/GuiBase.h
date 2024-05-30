#pragma once
#include "bakkesmod/plugin/PluginSettingsWindow.h"

class SettingsWindowBase : public BakkesMod::Plugin::PluginSettingsWindow {
	public:
		std::string GetPluginName() override;
		void SetImGuiContext(uintptr_t ctx) override;
};
