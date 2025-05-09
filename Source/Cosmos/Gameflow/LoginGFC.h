#pragma once

#include "IGameflowContext.h"
#include "UI/Presenters/StartupUIP.h"

class FLoginGFC : public IGameflowContext {
public:
	GAMEFLOW_CONTEXT_BEGIN(FLoginGFC)
		GAMEFLOW_CONTEXT_SIGNAL(WorldReady)
		GAMEFLOW_CONTEXT_SIGNAL(LoginSequencePlayFinisehd)
	GAMEFLOW_CONTEXT_END()
protected:
	virtual void Activate() override;
private:
	TObjectPtr<UStartupUIP> StartupUIP;
};