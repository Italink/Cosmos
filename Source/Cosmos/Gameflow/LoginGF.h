#pragma once

#include "IGameflowNode.h"
#include "UI/Presenters/StartupUIP.h"

class FLoginGF : public IGameflowNode {
public:
	GAMEFLOW_NODE_BEGIN(FLoginGF)
		GAMEFLOW_NODE_SIGNAL(WorldReady)
		GAMEFLOW_NODE_SIGNAL(LoginSequencePlayFinisehd)
	GAMEFLOW_NODE_END()

	virtual void Activate() override;
private:
	TObjectPtr<UStartupUIP> StartupUIP;
};