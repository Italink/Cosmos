#include "CmsGameMode.h"
#include "CmsGameState.h"
#include "CmsWorldSetting.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Player/CmsPlayerController.h"
#include "Gameplay/Player/CmsCharacter.h"
#include "Gameplay/Player/CmsPlayerState.h"

ACmsGameMode::ACmsGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ACmsCharacter::StaticClass();
	GameStateClass = ACmsGameState::StaticClass();
	PlayerControllerClass = ACmsPlayerController::StaticClass();
	PlayerStateClass = ACmsPlayerState::StaticClass();
}