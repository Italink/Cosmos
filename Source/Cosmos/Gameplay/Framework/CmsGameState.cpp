#include "CmsGameState.h"

ACmsGameState::ACmsGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ACmsGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}