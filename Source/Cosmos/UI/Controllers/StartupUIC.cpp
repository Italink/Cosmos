#include "StartupUIC.h"

void UStartupUIC::OnCreateUI()
{
	View = CreateView<UView_Startup_C>();
}

void UStartupUIC::OnConnectUI()
{
	View->Button_30->OnClicked.AddDynamic(this, &UStartupUIC::OnButtonClicked);
}

void UStartupUIC::OnButtonClicked()
{
	View->TextBlock_36->SetText(FText::FromString("Hello"));
}
