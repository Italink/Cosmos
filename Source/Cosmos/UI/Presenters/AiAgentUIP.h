#pragma once

#include "UI/Core/UIPresenter.h"
#include "UI/View/AiAgent/UMG_AiAgent.inl"
#include "AiAgentUIP.generated.h"

UCLASS()
class UAiAgentUIP : public UIPresenter {
	GENERATED_BODY()
public:
	void OnCreateUI() override;
	void OnConnectUI() override;
	void OnDestroy() override;
	void OnTickEvent(float Delta) override;

	UFUNCTION()
	void OnAskClicked();
private:
	TObjectPtr<UUMG_AiAgent_C> View;
};