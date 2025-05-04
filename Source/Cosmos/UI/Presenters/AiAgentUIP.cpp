#include "AiAgentUIP.h"
#include "AiAgent/AiAgentSubsystem.h"

void UAiAgentUIP::OnCreateUI()
{
	Card = CreateWidgetCard<UUMG_AiAgent_C>();
	View = Cast<UUMG_AiAgent_C>(Card->GetWidget());
}

void UAiAgentUIP::OnConnectUI()
{
	View->Button_Ask->OnClicked.AddDynamicLocalLambda([]() {
		UE_LOG(LogTemp, Warning, TEXT("Hello"));
	});
	View->Text_Content->OnTextChanged.AddDynamicLocalLambda([](FText Text) {
		UE_LOG(LogTemp, Warning, TEXT("Hello %s"), *Text.ToString());
	});
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
