#include "pch.h"
#include "GuiBase.h"

std::string SettingsWindowBase::GetPluginName() {
	return "ShotChallenge";
}

void SettingsWindowBase::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}
