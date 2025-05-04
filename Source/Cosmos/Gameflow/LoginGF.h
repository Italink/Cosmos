#pragma once

#include "IGameflow.h"
#include "UI/Presenters/StartupUIP.h"
#include "LoginGF.generated.h"

UCLASS(MinimalAPI)
class ULoginFlow : public UGameInstanceSubsystem {
	GENERATED_BODY()
public:
	DEFINE_GAMEFLOW_DELEGATE_TYPED(WorldReady, FSimpleMulticastDelegate);
	DEFINE_GAMEFLOW_DELEGATE_TYPED(LoginSequencePlayFinisehd, FSimpleMulticastDelegate);
public:
	static ULoginFlow* Get(UObject* InWorldContext = nullptr);
	void Setup();

	UPROPERTY()
	TObjectPtr<UStartupUIP> StartupUIP;
};
