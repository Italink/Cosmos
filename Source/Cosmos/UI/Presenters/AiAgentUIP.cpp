#include "AiAgentUIP.h"
#include "AiAgent/AiAgentSubsystem.h"

void UAiAgentUIP::OnCreateUI()
{
	View = CreateView<UView_AiAgent_C>();
}

void UAiAgentUIP::OnConnectUI()
{
	View->Button_Ask->OnClicked.AddDynamic(this, &UAiAgentUIP::OnAskClicked);
}

void UAiAgentUIP::OnDestroy()
{
}

void UAiAgentUIP::OnTickEvent(float Delta)
{
}

void UAiAgentUIP::OnAskClicked()
{
	UAiAgentSubsystem* AiAgent = UAiAgentSubsystem::Get(this);
	AiAgent->Ask(View->Text_Content->GetText().ToString(), [AiAgent](FString Response) {
		AiAgent->Speak(Response);
	});
}
