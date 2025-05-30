/* This file is generated by Blueprint_C */ 

class UUMG_ComboBoxSettingItem_C : public UUserWidget 
{
public:
    static UClass* StaticClass()
    {
        return LoadObject<UClass>(nullptr, TEXT("/Game/UI/View/Basic/Settings/Item/UMG_ComboBoxSettingItem.UMG_ComboBoxSettingItem_C")); 
    }

    UPROPERTY(BlueprintReadOnly, Instanced, NotReplicated, Meta = (EditInline="true", DisplayName="ComboBox", Category="UMG_ComboBoxSettingItem"))
    TObjectPtr<UComboBoxString> ComboBox;

    UPROPERTY(BlueprintReadOnly, Instanced, NotReplicated, Meta = (EditInline="true", DisplayName="Name", Category="UMG_ComboBoxSettingItem"))
    TObjectPtr<UTextBlock> Name;


};