#include "CmsPlayerController.h"
#include "Core/UI/UIManager.h"
#include "Core/Gameflow/GameflowContextManager.h"
#include "UI/Presenters/Engine/GameSettingsUIP.h"
#include "UI/Presenters/AiAgentUIP.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	USignalEventManager::Get(this);
	UUIManager::Get(this)->SetPlayerController(this);
	LoginGFC = UGameflowContextManager::Get(this)->Push<ULoginGFC>();
	LoginGFC->Signals.WorldReady.Invoke(GetWorld());
}
