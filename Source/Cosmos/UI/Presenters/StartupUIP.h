#pragma once

#include "UI/Core/UIPresenter.h"
#include "UI/View/UMG_Startup.inl"
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
    TObjectPtr<UUMG_Startup_C> View;
};