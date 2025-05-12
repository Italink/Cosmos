#pragma once

#include "Core/UI/UIPresenter.h"
#include "UI/Presenters/Basic/SettingsBaseUIP.h"
#include "GameSettingsUIP.generated.h"

UCLASS()
class UGameSettingsUIP : public USettingsBaseUIP {
    GENERATED_BODY()
public:    
protected:
    void OnCreateUI() override;
};