#include "CmsPlayerController.h"
#include "UI/Core/UIManager.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FUIManager::Get()->SetPlayerController(this);

	StartupUIC = NewObject<UAiAgentUIC>();
	StartupUIC->Show();
}
