#include "CmsPlayerController.h"
#include "UI/Core/UIManager.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogCms, Warning, TEXT("ACmsPlayerController::BeginPlay"));

	FUIManager::Get()->SetPlayerController(this);

	StartupUIC = NewObject<UStartupUIC>();
	StartupUIC->Show();
}
