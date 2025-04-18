/* This file is generated by Blueprint_C */ 

class UUMG_StringSettingItem_C : public UUserWidget 
{
public:
    static UClass* StaticClass()
    {
        return LoadObject<UClass>(nullptr, TEXT("/Game/UI/View/Basic/Settings/Item/UMG_StringSettingItem.UMG_StringSettingItem_C")); 
    }

    UPROPERTY(BlueprintReadOnly, Instanced, NotReplicated, Meta = (EditInline="true", DisplayName="LineEditor", Category="UMG_StringSettingItem"))
    TObjectPtr<UMultiLineEditableText> LineEditor;

    UPROPERTY(BlueprintReadOnly, Instanced, NotReplicated, Meta = (EditInline="true", DisplayName="Name", Category="UMG_StringSettingItem"))
    TObjectPtr<UTextBlock> Name;


};