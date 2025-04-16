#pragma once

#include "GameFramework/PlayerController.h"
#include "UI/Core/UIPresenter.h"
#include "CmsPlayerController.generated.h"


UCLASS()
class ACmsPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UIPresenter> StartupUIP;
};
