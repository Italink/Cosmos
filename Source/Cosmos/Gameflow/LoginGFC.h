#pragma once

#include "Core/Gameflow/GameflowContext.h"
#include "UI/Presenters/StartupUIP.h"
#include "LoginGFC.generated.h"

UCLASS()
class ULoginGFC : public UGameflowContext {
	GENERATED_BODY()
	GAMEFLOW_CONTEXT_BEGIN(ULoginGFC)
		GAMEFLOW_CONTEXT_SIGNAL(WorldReady)
		GAMEFLOW_CONTEXT_SIGNAL(OpeningSequencePlayFinisehd)
		GAMEFLOW_CONTEXT_SIGNAL(ShowMenuSequencePlayFinisehd)
	GAMEFLOW_CONTEXT_END()

protected:
	virtual void Activate() override;

private:
	UPROPERTY()
	TObjectPtr<UStartupUIP> StartupUIP;

	UPROPERTY()
	TObjectPtr<ACameraActor> SequenceCameraActor;
};