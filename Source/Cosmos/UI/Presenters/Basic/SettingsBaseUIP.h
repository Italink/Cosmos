#pragma once

#include "UI/Core/UIPresenter.h"
#include "UI/View/Basic/Settings/UMG_SettingPanel.inl"
#include "SettingsBaseUIP.generated.h"

UCLASS()
class USettingsBaseUIP : public UIPresenter{
    GENERATED_BODY()
public:    
    enum class ESettingItemType {
        None,
        Button,
        CheckBox,
        TextBox,
        NumberBox,
        ComboBox,
        Group
    };
    enum class ESettingGroupType{
        None,
        HBox,
        VBox,
        WrapBox
    };
    struct ISettingItemBase{
        ESettingItemType Type = ESettingItemType::None;
		FText Name;
    };
    struct FButtonSettingItem : public ISettingItemBase{
        FButtonSettingItem() : ISettingItemBase({ ESettingItemType::Button }) {};
        TFunction<void()> OnClicked;
    };
    struct FCheckBoxSettingItem : public ISettingItemBase {
        FCheckBoxSettingItem() : ISettingItemBase({ ESettingItemType::CheckBox }) {};
		bool InitialState;
        TFunction<void(bool)> OnCheckStateChanged;
    };
    struct FNumberBoxSettingItem: public ISettingItemBase{
        FNumberBoxSettingItem() : ISettingItemBase({ ESettingItemType::NumberBox }) {};
        float InitialValue;
        float MinimumValue;
        float MaximumValue;
        float Delta;
        TFunction<void(float)> OnNumberChanged;
    };
    struct FTextBoxSettingItem : public ISettingItemBase {
        FTextBoxSettingItem() : ISettingItemBase({ ESettingItemType::TextBox }) {};
        FText InitialText;
        FText HintText;
        TFunction<void(FText)> OnTextChanged;
    };
    struct FComboBoxSettingItem : public ISettingItemBase{
        FComboBoxSettingItem() : ISettingItemBase({ ESettingItemType::ComboBox }) {};
        TArray<FString> OptionList;
        int CurrentIndex;
        TFunction<void(int)> OnCurrentIndexChanged;
    };
    struct FGroupSettingItem : public ISettingItemBase{
        FGroupSettingItem() : ISettingItemBase({ ESettingItemType::Group }) {};
        ESettingGroupType GroupType;
        TArray<TSharedPtr<ISettingItemBase>> SubItemList;
    };
protected:
    void OnCreateUI() override;
    void OnConnectUI() override;
    void OnDestroy() override;

    void BeginCategory(FText CategoryName);
    void EndCategory();

    void BeginGroup(FText GroupName, ESettingGroupType GroupType = ESettingGroupType::WrapBox);
    void EndGroup();

    void AddItem(TSharedPtr<ISettingItemBase> Item);
    void AddButtonItem(FText Name, TFunction<void()> OnClicked);
    void AddCheckBoxItem(FText Name, bool InitialState, TFunction<void(bool)> OnCheckStateChanged);
    void AddTextBoxItem(FText Name, FText InitialText, FText HintText, TFunction<void(FText)> OnTextChanged);
    void AddNumberBoxItem(FText Name, float InitialValue, float MinimumValue, float MaximumValue, float Delta, TFunction<void(float)> OnNumberChanged);
    void AddComboBoxItem(FText Name, TArray<FString> OptionList, int InitialIndex, TFunction<void(int)> OnCurrentIndexChanged);

    UFUNCTION()
    void SetCurrentCategory(FText Category);

    void RecreateSettingPanel();
    void GenerateWidgetForItem(ISettingItemBase* Item, TFunction<void(UWidget*)> WidgetSetupAction);
protected:
    TObjectPtr<UUMG_SettingPanel_C> View;
    TArray<FText> CategoryOrder;
    TMap<FString, TArray<TSharedPtr<ISettingItemBase>>> CategorySettingsMap;
    TPair<FText, TArray<TSharedPtr<ISettingItemBase>>*> CurrentCategory;
    bool bNeedRecreate = false;
};