#include "CmsPlayerController.h"
#include "UI/Core/UIManager.h"
#include "UI/Presenters/Engine/GameSettingsUIP.h"
#include "UI/Presenters/AiAgentUIP.h"
#include "Gameflow/LoginGF.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UUIManager::Get(this)->SetPlayerController(this);

	ULoginFlow::Get(this)->Setup();
	ULoginFlow::Get()->GetWorldReady().Broadcast();
}
