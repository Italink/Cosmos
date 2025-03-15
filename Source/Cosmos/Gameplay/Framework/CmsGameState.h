#pragma once

#include "GameFramework/GameState.h"
#include "CmsGameState.generated.h"

UCLASS()
class ACmsGameState : public AGameState
{
	GENERATED_BODY()
public:
	ACmsGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void Tick(float DeltaSeconds) override;
	//~End of AActor interface
protected:

};
