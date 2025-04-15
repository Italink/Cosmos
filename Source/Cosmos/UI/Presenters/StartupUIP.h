#pragma once

#include "UI/Core/UIPresenter.h"
#include "UI/View/View_Startup.inl"
#include "StartupUIP.generated.h"

UCLASS()
class UStartupUIP : public UIPresenter{
    GENERATED_BODY()
public:    
    void OnCreateUI() override;
    void OnConnectUI() override;

    UFUNCTION()
    void OnButtonClicked();

private:
    TObjectPtr<UView_Startup_C> View;
};