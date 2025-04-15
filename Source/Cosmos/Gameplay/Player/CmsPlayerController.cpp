#include "CmsPlayerController.h"
#include "UI/Core/UIManager.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UUIManager::Get(this)->SetPlayerController(this);
	UUIManager::Get()->GetBlackScreen()->FadeIn(5, [this]() {
		StartupUIP = NewObject<UAiAgentUIP>();
		StartupUIP->Show();
		UUIManager::Get()->GetBlackScreen()->FadeOut(5);
	});
}
