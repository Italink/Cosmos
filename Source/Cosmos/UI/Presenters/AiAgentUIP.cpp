#include "AiAgentUIP.h"
#include "AiAgent/AiAgentSubsystem.h"

void UAiAgentUIP::OnCreateUI()
{
	View = CreateView<UUMG_AiAgent_C>();
}

void UAiAgentUIP::OnConnectUI()
{
	View->Button_Ask->OnClicked.Add(CreateLambdaDynamic([]() {
		UE_LOG(LogTemp, Warning, TEXT("Hello"));
	}));

	View->Text_Content->OnTextChanged.Add(CreateLambdaDynamic([](FText Text) {
		UE_LOG(LogTemp, Warning, TEXT("Hello %s"), *Text.ToString());
	}));
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
