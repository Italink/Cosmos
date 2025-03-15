#pragma once

#include "GameFramework/PlayerController.h"
#include "UI/Controllers/StartupUIC.h"
#include "CmsPlayerController.generated.h"


UCLASS()
class ACmsPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UStartupUIC> StartupUIC;
};
