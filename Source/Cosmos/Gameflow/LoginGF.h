#pragma once

#include "IGameflowNode.h"
#include "UI/Presenters/StartupUIP.h"

class FLoginGF : public IGameflowNode {
public:
	static FName GetNodeName() { return TEXT("Login"); };
	struct FSignals
	{
		IGameflowNode* Owner = nullptr;
		FGameflowSignal WorldReady{ "WorldReady", Owner };
		FGameflowSignal LoginSequencePlayFinisehd{ "LoginSequencePlayFinisehd", Owner };

	}Signals{ this };

	virtual void Activate() override;
private:
	TObjectPtr<UStartupUIP> StartupUIP;
};