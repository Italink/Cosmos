#include "AiAgentUIC.h"
#include "AiAgent/AiAgentSubsystem.h"

void UAiAgentUIC::OnCreateUI()
{
	View = CreateView<UView_AiAgent_C>();
}

void UAiAgentUIC::OnConnectUI()
{
	View->Button_Ask->OnClicked.AddDynamic(this, &UAiAgentUIC::OnAskClicked);
}

void UAiAgentUIC::OnDestroy()
{
}

void UAiAgentUIC::OnTickEvent(float Delta)
{
}

void UAiAgentUIC::OnAskClicked()
{
	UAiAgentSubsystem* AiAgent = UAiAgentSubsystem::Get(this);
	AiAgent->Ask(View->Text_Content->GetText().ToString(), [AiAgent](FString Response) {
		AiAgent->Speak(Response);
	});
}
