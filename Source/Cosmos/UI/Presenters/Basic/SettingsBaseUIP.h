#pragma once

#include "UI/Core/UIPresenter.h"
#include "SettingsBaseUIP.generated.h"


UCLASS()
class USettingsBaseUIP : public UIPresenter{
    GENERATED_BODY()
public:    
    enum ESettingItemType{
        None,
        Button,
        CheckBox,
        NumberBox,
        ComboBox,
        Group
    };

    enum ESettingGroupType{
        None,
        HBox,
        VBox,
        WrapBox
    };

    struct ISettingItemBase{
		FString Name;
        TFunction<void()> RefreshCallback;
		virtual ESettingItemType GetItemType() {
			return ESettingItemType::None;
		}
		virtual void Refresh() {
			if (RefreshCallback)
				RefreshCallback();
		}
		void SetupRefreshCallback(TFunction<void()> Callback){
			RefreshCallback = Callback;
			RefreshCallback();
		}
    };

protected:
    void OnCreateUI() override;
    void OnConnectUI() override;
    void OnDestroy() override;
private:

};