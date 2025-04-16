#include "SettingsBaseUIP.h"
#include "UI/View/Basic/Settings/Item/UMG_CategorySettingItem.inl"
#include "UI/View/Basic/Settings/Item/UMG_ButtonSettingItem.inl"
#include "UI/View/Basic/Settings/Item/UMG_GroupHeader.inl"
#include "UI/View/Basic/Settings/Item/UMG_ComboBoxSettingItem.inl"
#include "UI/View/Basic/Settings/Item/UMG_CheckBoxSettingItem.inl"
#include "UI/View/Basic/Settings/Item/UMG_NumberBoxSettingItem.inl"
#include "UI/View/Basic/Settings/Item/UMG_StringSettingItem.inl"

void USettingsBaseUIP::OnCreateUI()
{
	View = CreateView<UUMG_SettingPanel_C>();
	if (bNeedRecreate) {
		bNeedRecreate = false;
		RecreateSettingPanel();
	}
}

void USettingsBaseUIP::OnConnectUI()
{

}

void USettingsBaseUIP::OnDestroy()
{

}

void USettingsBaseUIP::BeginCategory(FText CategoryName)
{
	CategoryOrder.Add(CategoryName);
	CurrentCategory = { CategoryName, &CategorySettingsMap.FindOrAdd(CategoryName.ToString()) };
}

void USettingsBaseUIP::EndCategory()
{
	CurrentCategory = { FText::FromString("Common"), nullptr };
}

void USettingsBaseUIP::BeginGroup(FText GroupName, ESettingGroupType GroupType /*= ESettingGroupType::WrapBox*/)
{
	TSharedPtr<FGroupSettingItem> Group = MakeShared<FGroupSettingItem>();
	Group->Name = GroupName;
	Group->GroupType = GroupType;
	CurrentCategory.Value = &Group->SubItemList;
	CategorySettingsMap.FindOrAdd(CurrentCategory.Key.ToString()).Add(Group);
}

void USettingsBaseUIP::EndGroup()
{
	CurrentCategory.Value = &CategorySettingsMap.FindOrAdd(CurrentCategory.Key.ToString());
}

void USettingsBaseUIP::AddItem(TSharedPtr<ISettingItemBase> Item)
{
	if (Item){
		TArray<TSharedPtr<ISettingItemBase>>* ItemList = &CategorySettingsMap.FindOrAdd(CurrentCategory.Key.ToString());
		CurrentCategory.Value->Add(Item);
		if (ItemList == CurrentCategory.Value) {
			ItemList->Add(Item);
		}
		bNeedRecreate = true;
	}
	else
	{
		//Console.WriteLine("Item is Invalid");
	}
}

void USettingsBaseUIP::AddButtonItem(FText Name, TFunction<void()> OnClicked)
{
	TSharedPtr<FButtonSettingItem> Item = MakeShared<FButtonSettingItem>();
	Item->Name = Name;
	Item->OnClicked = OnClicked;
	AddItem(Item);
}

void USettingsBaseUIP::AddCheckBoxItem(FText Name, bool InitialState, TFunction<void(bool)> OnCheckStateChanged)
{
	TSharedPtr<FCheckBoxSettingItem> Item = MakeShared<FCheckBoxSettingItem>();
	Item->Name = Name;
	Item->InitialState = InitialState;
	Item->OnCheckStateChanged = OnCheckStateChanged;
	AddItem(Item);
}

void USettingsBaseUIP::AddTextBoxItem(FText Name, FText InitialText, FText HintText, TFunction<void(FText)> OnTextChanged)
{
	TSharedPtr<FTextBoxSettingItem> Item = MakeShared<FTextBoxSettingItem>();
	Item->Name = Name;
	Item->InitialText = InitialText;
	Item->HintText = HintText;
	Item->OnTextChanged = OnTextChanged;
	AddItem(Item);
}

void USettingsBaseUIP::AddNumberBoxItem(FText Name, float InitialValue, float MinimumValue, float MaximumValue, float Delta, TFunction<void(float)> OnNumberChanged)
{
	TSharedPtr<FNumberBoxSettingItem> Item = MakeShared<FNumberBoxSettingItem>();
	Item->Name = Name;
	Item->InitialValue = InitialValue;
	Item->MinimumValue = MinimumValue;
	Item->MaximumValue = MaximumValue;
	Item->Delta = Delta;
	Item->OnNumberChanged = OnNumberChanged;
	AddItem(Item);
}

void USettingsBaseUIP::AddComboBoxItem(FText Name, TArray<FString> OptionList, int InitialIndex, TFunction<void(int)> OnCurrentIndexChanged)
{
	TSharedPtr<FComboBoxSettingItem> Item = MakeShared<FComboBoxSettingItem>();
	Item->Name = Name;
	Item->OptionList = OptionList;
	Item->CurrentIndex = InitialIndex;
	Item->OnCurrentIndexChanged = OnCurrentIndexChanged;
	AddItem(Item);
}

void USettingsBaseUIP::RecreateSettingPanel()
{
	if (View) {
		View->CategoryVBox->ClearChildren();
		for (auto Category : CategoryOrder) {
			UUMG_CategorySettingItem_C* CategoryItem = CreateView<UUMG_CategorySettingItem_C>(false);
			CategoryItem->Name->SetText(Category);
			CategoryItem->Button->OnClicked.Add(CreateLambdaDynamic([this, Category]() {
				SetCurrentCategory(Category);
			}));
			View->CategoryVBox->AddChildToVerticalBox(CategoryItem);
		}
	}
	if (!CategoryOrder.IsEmpty()) {
		SetCurrentCategory(CategoryOrder[0]);
	}
}

void USettingsBaseUIP::SetCurrentCategory(FText Category)
{
	View->ItemVBox->ClearChildren();
	if (CategorySettingsMap.Contains(Category.ToString())){
		for(auto Item : CategorySettingsMap[Category.ToString()]){
			GenerateWidgetForItem(Item.Get(), [this](UWidget* Widget){
				View->ItemVBox->AddChildToVerticalBox(Widget);
			});
		}
	}
}

void USettingsBaseUIP::GenerateWidgetForItem(ISettingItemBase* Item, TFunction<void(UWidget*)> WidgetSetupAction)
{
	switch (Item->Type)
	{
	case ESettingItemType::None:
		break;
	case ESettingItemType::Button:
	{
		FButtonSettingItem* Cmd = static_cast<FButtonSettingItem*>(Item);
		UUMG_ButtonSettingItem_C* Widget = CreateView<UUMG_ButtonSettingItem_C>(false);
		WidgetSetupAction(Widget);
		Widget->Name->SetText(Cmd->Name);
		Widget->Button->OnClicked.Add(CreateLambdaDynamic(Cmd->OnClicked));
		break;
	}

	case ESettingItemType::CheckBox:
	{
		FCheckBoxSettingItem* Cmd = static_cast<FCheckBoxSettingItem*>(Item);
		UUMG_CheckBoxSettingItem_C* Widget = CreateView<UUMG_CheckBoxSettingItem_C>(false);
		WidgetSetupAction(Widget);
		Widget->Name->SetText(Cmd->Name);
		Widget->CheckBox->SetCheckedState(Cmd->InitialState ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		Widget->CheckBox->OnCheckStateChanged.Add(CreateLambdaDynamic(Cmd->OnCheckStateChanged));
		break;
	}
	case ESettingItemType::NumberBox:
	{
		FNumberBoxSettingItem* Cmd = static_cast<FNumberBoxSettingItem*>(Item);
		UUMG_NumberBoxSettingItem_C* Widget = CreateView<UUMG_NumberBoxSettingItem_C>(false);
		WidgetSetupAction(Widget);
		Widget->Name->SetText(Cmd->Name);
		Widget->SpinBox->SetValue(Cmd->InitialValue);
		Widget->SpinBox->SetMaxSliderValue(Cmd->MaximumValue);
		Widget->SpinBox->SetMinValue(Cmd->MinimumValue);
		Widget->SpinBox->SetMinSliderValue(Cmd->MinimumValue);
		Widget->SpinBox->SetDelta(Cmd->Delta);
		Widget->SpinBox->OnValueChanged.Add(CreateLambdaDynamic(Cmd->OnNumberChanged));
		break;
	}
	case ESettingItemType::TextBox:
	{
		FTextBoxSettingItem* Cmd = static_cast<FTextBoxSettingItem*>(Item);
		UUMG_StringSettingItem_C* Widget = CreateView<UUMG_StringSettingItem_C>(false);
		WidgetSetupAction(Widget);
		Widget->Name->SetText(Cmd->Name);
		Widget->LineEditor->SetText(Cmd->InitialText);
		Widget->LineEditor->SetHintText(Cmd->HintText);
		Widget->LineEditor->OnTextChanged.Add(CreateLambdaDynamic([Cmd](FText Text) {
			Cmd->OnTextChanged(Text);
		}));
		break;
	}
	case ESettingItemType::ComboBox:
	{
		FComboBoxSettingItem* Cmd = static_cast<FComboBoxSettingItem*>(Item);
		UUMG_ComboBoxSettingItem_C* Widget = CreateView<UUMG_ComboBoxSettingItem_C>(false);
		WidgetSetupAction(Widget);
		Widget->Name->SetText(Cmd->Name);
		for(auto Opt : Cmd->OptionList){
			Widget->ComboBox->AddOption(Opt);
		}
		Widget->ComboBox->SetSelectedIndex(Cmd->CurrentIndex);
		Widget->ComboBox->OnSelectionChanged.Add(CreateLambdaDynamic([Cmd](FString Opt, ESelectInfo::Type SelectInfo){
			int Index = Cmd->OptionList.IndexOfByKey(Opt);
			if (Index != -1) {
				Cmd->CurrentIndex = Index;
				Cmd->OnCurrentIndexChanged(Index);
			}
		}));
		break;
	}
	case ESettingItemType::Group:
	{
		FGroupSettingItem* Cmd = static_cast<FGroupSettingItem*>(Item);
		UUMG_GroupHeader_C* Widget = CreateView<UUMG_GroupHeader_C>(false);
		WidgetSetupAction(Widget);
		Widget->Name->SetText(Cmd->Name);
		if (Cmd->GroupType == ESettingGroupType::WrapBox){
			UWrapBox* GroupLayout = NewObject<UWrapBox>();
			WidgetSetupAction(GroupLayout);
			for (auto SubItem : Cmd->SubItemList) {
				GenerateWidgetForItem(SubItem.Get(), [this, GroupLayout](UWidget* Widget) {
					GroupLayout->AddChildToWrapBox(Widget);
				});
			}
		}
		else if (Cmd->GroupType == ESettingGroupType::VBox){
			UVerticalBox* GroupLayout = NewObject<UVerticalBox>();
			WidgetSetupAction(GroupLayout);
			for (auto SubItem : Cmd->SubItemList) {
				GenerateWidgetForItem(SubItem.Get(), [this, GroupLayout](UWidget* Widget) {
					GroupLayout->AddChildToVerticalBox(Widget);
				});
			}
		}
		else if (Cmd->GroupType == ESettingGroupType::HBox){
			UHorizontalBox* GroupLayout = NewObject<UHorizontalBox>();
			WidgetSetupAction(GroupLayout);
			for (auto SubItem : Cmd->SubItemList) {
				GenerateWidgetForItem(SubItem.Get(), [this, GroupLayout](UWidget* Widget) {
					GroupLayout->AddChildToHorizontalBox(Widget);
				});
			}
		}
		break;
	}
	}
}