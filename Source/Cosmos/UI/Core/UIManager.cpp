#include "UIManager.h"
#include "Modules/ModuleManager.h"
#include "UIController.h"
#include "Kismet/GameplayStatics.h"

FUIManager* FUIManager::Get()
{
	static FUIManager Instance;
	return &Instance;
}

void FUIManager::SetPlayerController(APlayerController* InPlayerController)
{
	CurrentPlayerController = InPlayerController;
	CurrentGameInstance = UGameplayStatics::GetGameInstance(InPlayerController);
} 

UGameInstance* FUIManager::GetGameInstance()
{
	return CurrentGameInstance;
}

APlayerController* FUIManager::GetPlayerController()
{
	return CurrentPlayerController;
}

void FUIManager::RegisterController(UIController* InController)
{
	AllControllers.Add(InController);
}

void FUIManager::UnRegisterController(UIController* InController)
{
	AllControllers.Remove(InController);
}

TStatId FUIManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FUIManager, STATGROUP_Tickables);
}

void FUIManager::Tick(float InDeltaSeconds)
{
	for (auto Controller : AllControllers) {
		if (Controller) {
			Controller->OnTickEvent(InDeltaSeconds);
		}
	}
}
