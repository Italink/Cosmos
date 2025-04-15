#pragma once

#include "GameFramework/PlayerController.h"
#include "UI/Presenters/StartupUIP.h"
#include "UI/Presenters/AiAgentUIP.h"
#include "CmsPlayerController.generated.h"


UCLASS()
class ACmsPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAiAgentUIP> StartupUIP;
};
