#pragma once

#include "Core/UI/UIPresenter.h"
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

public:
    TObjectPtr<UUMG_Startup_C> View;
};