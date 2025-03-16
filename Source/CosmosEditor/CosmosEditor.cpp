#include "CosmosEditor.h"
#include "AiAgent/AiAgentSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "CosmosEditor"

void FCosmosEditorModule::StartupModule()
{
	if (!FSlateApplication::IsInitialized()) {
		return;
	}
	UAiAgentSettings* Settings = GetMutableDefault<UAiAgentSettings>();
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule) {
		SettingsModule->RegisterSettings("Project", TEXT("Project"), TEXT("AiAgent"),
			LOCTEXT("AiAgent", "AiAgent"),
			LOCTEXT("AiAgent", "AiAgent"),
			Settings);
	}
}

void FCosmosEditorModule::ShutdownModule()
{
	if (!FSlateApplication::IsInitialized()) {
		return;
	}
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule) {
		SettingsModule->UnregisterSettings("Project", TEXT("Project"), TEXT("AiAgent"));
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCosmosEditorModule, CosmosEditor); 

