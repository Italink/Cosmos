#include "CmsPlayerController.h"
#include "UI/Core/UIManager.h"
#include "UI/Presenters/Engine/GameSettingsUIP.h"
#include "UI/Presenters/AiAgentUIP.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	USignalEventManager::Get(this);
	UUIManager::Get(this)->SetPlayerController(this);

	LoginGFC = IGameflowContext::Push<FLoginGFC>();
	LoginGFC->Signals.WorldReady.Invoke(GetWorld());
}
