#pragma once

#include "GameFramework/PlayerController.h"
#include "Gameflow/LoginGFC.h"
#include "CmsPlayerController.generated.h"

UCLASS()
class ACmsPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	TObjectPtr<ULoginGFC> LoginGFC;
};
