#include "UIManager.h"
#include "Modules/ModuleManager.h"
#include "UIPresenter.h"
#include "Kismet/GameplayStatics.h"

UUIManager* UUIManager::Get(UObject* InWorldContext)
{
	static TObjectPtr<UGameInstance> LastUseGameInstance;
	UGameInstance* GameInstacne = nullptr;
	if (InWorldContext) {
		GameInstacne = UGameplayStatics::GetGameInstance(InWorldContext);
		LastUseGameInstance = GameInstacne;
	}
	else {
		GameInstacne = LastUseGameInstance;
	}
	return GameInstacne->GetSubsystem<UUIManager>();
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UUIManager::Deinitialize()
{
	TArray<TObjectPtr<UIPresenter>> PresentersIter = AllPresenters;
	for (auto Presenter : PresentersIter) {
		if (Presenter) {
			Presenter->ForceDestory();
		}
	}
	AllPresenters.Reset();
}

void UUIManager::SetPlayerController(APlayerController* InPlayerController)
{
	CurrentPlayerController = InPlayerController;
} 

APlayerController* UUIManager::GetPlayerController()
{
	return CurrentPlayerController;
}

void UUIManager::RegisterPresenter(UIPresenter* InController)
{
	AllPresenters.Add(InController);
}

void UUIManager::UnRegisterPresenter(UIPresenter* InController)
{
	AllPresenters.Remove(InController);
}

UBlackScreenUIP* UUIManager::GetBlackScreen()
{
	if (!BlackScreenUIP) {
		BlackScreenUIP = NewObject<UBlackScreenUIP>();
	}
	return BlackScreenUIP;
}

TStatId UUIManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UUIManager, STATGROUP_Tickables);
}

void UUIManager::Tick(float InDeltaSeconds)
{
	for (auto Presenter : AllPresenters) {
		if (Presenter) {
			Presenter->OnTickEvent(InDeltaSeconds);
		}
	}
}

