#include "CmsPlayerController.h"
#include "UI/Core/UIManager.h"
#include "UI/Presenters/Engine/GameSettingsUIP.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UUIManager::Get(this)->SetPlayerController(this);
	UUIManager::Get()->GetBlackScreen()->FadeIn(1, [this]() {
		StartupUIP = NewObject<UGameSettingsUIP>();
		StartupUIP->Show();
		UUIManager::Get()->GetBlackScreen()->FadeOut(1);
	});
}
