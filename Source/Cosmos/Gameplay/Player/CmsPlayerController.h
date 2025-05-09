#pragma once

#include "GameFramework/PlayerController.h"
#include "UI/Core/UIPresenter.h"
#include "Gameflow/LoginGFC.h"
#include "CmsPlayerController.generated.h"

UCLASS()
class ACmsPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	TSharedPtr<FLoginGFC> LoginGFC;
};
