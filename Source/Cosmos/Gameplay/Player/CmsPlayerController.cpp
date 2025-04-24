#include "CmsPlayerController.h"
#include "UI/Core/UIManager.h"
#include "UI/Presenters/Engine/GameSettingsUIP.h"
#include "UI/Presenters/AiAgentUIP.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UUIManager::Get(this)->SetPlayerController(this);

	StartupUIP = NewObject<UAiAgentUIP>();
	StartupUIP->Show();

	//UUIManager::Get()->GetBlackScreen()->FadeIn(1, [this]() {
	//	UUIManager::Get()->GetBlackScreen()->FadeOut(1);
	//});
}
