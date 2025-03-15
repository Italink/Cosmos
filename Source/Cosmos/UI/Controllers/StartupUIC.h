#pragma once

#include "UI/Core/UIController.h"
#include "UI/View/View_Startup.inl"
#include "StartupUIC.generated.h"

UCLASS()
class UStartupUIC : public UIController{
    GENERATED_BODY()
public:    
    void OnCreateUI() override;
    void OnConnectUI() override;

    UFUNCTION()
    void OnButtonClicked();

private:
    TObjectPtr<UView_Startup_C> View;
};