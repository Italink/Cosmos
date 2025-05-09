#include "CmsPlayerController.h"
#include "UI/Core/UIManager.h"
#include "UI/Presenters/Engine/GameSettingsUIP.h"
#include "UI/Presenters/AiAgentUIP.h"
#include "Gameflow/LoginGF.h"

void ACmsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	USignalEventManager::Get(this);
	UUIManager::Get(this)->SetPlayerController(this);
	LoginGF = IGameflowNode::Make<FLoginGF>();
	LoginGF->Signals.WorldReady.Invoke(GetWorld());
}
