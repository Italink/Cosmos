#include "StartupUIP.h"

void UStartupUIP::OnCreateUI()
{
	View = CreateView<UUMG_Startup_C>();
}

void UStartupUIP::OnConnectUI()
{
	View->Button_Start->OnClicked.AddDynamic(this, &UStartupUIP::OnButtonClicked);
}

void UStartupUIP::OnButtonClicked()
{
}
