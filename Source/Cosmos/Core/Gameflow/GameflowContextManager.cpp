#include "GameflowContextManager.h"
#include "Kismet/GameplayStatics.h"

UGameflowContextManager* UGameflowContextManager::Get(UObject* InWorldContext /*= nullptr*/)
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
	return GameInstacne->GetSubsystem<UGameflowContextManager>();
}