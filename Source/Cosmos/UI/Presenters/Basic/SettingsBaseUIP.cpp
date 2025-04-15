#include "SettingsBaseUIP.h"

struct FButtonSettingItem : public USettingsBaseUIP::ISettingItemBase
{
    USettingsBaseUIP::ESettingItemType GetItemType() { return USettingsBaseUIP::ESettingItemType::Button; }
    TFunction<void()> OnClicked;
};

struct FGameSetting_CheckBox : public USettingsBaseUIP::ISettingItemBase
{
    USettingsBaseUIP::ESettingItemType GetItemType() { return USettingsBaseUIP::ESettingItemType::CheckBox; }
    TFunction<bool()> CheckStateGetter;
    TFunction<void(bool)> OnCheckStateChanged;
};

struct FGameSetting_NumberBox : public USettingsBaseUIP::ISettingItemBase
{
    USettingsBaseUIP::ESettingItemType GetItemType() { return USettingsBaseUIP::ESettingItemType::NumberBox; }
    float InitialValue;
    float MinimumValue;
    float MaximumValue;
    float Delta;
    TFunction<void(float)> OnNumberChanged;
};

struct FGameSetting_ComboBox : public USettingsBaseUIP::ISettingItemBase
{
    USettingsBaseUIP::ESettingItemType GetItemType() { return USettingsBaseUIP::ESettingItemType::ComboBox; }
    TFunction<TArray<FString> ()> OptionListGetter;
    TFunction<int()> CurrentIndexGetter;
    TFunction<void(int)> OnCurrentIndexChanged;
};

struct FGameSetting_Group : public USettingsBaseUIP::ISettingItemBase
{
    USettingsBaseUIP::ESettingItemType GetItemType() { return USettingsBaseUIP::ESettingItemType::Group; }
    USettingsBaseUIP::ESettingGroupType GroupType;
    TArray<TSharedPtr<USettingsBaseUIP::ISettingItemBase>> SubItemList;
    void Refresh() override
    {
        for (auto Item : SubItemList) {
            Item->Refresh();
        }
    }
};

void USettingsBaseUIP::OnCreateUI()
{

}

void USettingsBaseUIP::OnConnectUI()
{

}

void USettingsBaseUIP::OnDestroy()
{

}

