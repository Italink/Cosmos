#include "UIPresenter.h"
#include "UIManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UIPresenter::UIPresenter(const FObjectInitializer& ObjectInitializer)
{
    if (!HasAnyFlags(RF_ClassDefaultObject)) {
        UUIManager::Get()->RegisterPresenter(this);
    }
}

void UIPresenter::BeginDestroy()
{
    Super::BeginDestroy();
    if (!HasAnyFlags(RF_ClassDefaultObject)) {
        ForceDestory();
    }
}

void UIPresenter::ForceDestory()
{
	if (!bHasDestroyed) {
		bHasDestroyed = true;
		if (bVisible) {
			OnCloseEvent();
		}
		for (auto View : Views) {
			if (View) {
				View->RemoveFromParent();
			}
		}
		Views.Reset();
		CachedVisibilities.Reset();
		OnDestroy();
		UUIManager::Get()->UnRegisterPresenter(this);
	}
}

UUserWidget* UIPresenter::CreateView(UClass* UMGClass)
{
    if(!UMGClass)
        return nullptr;
    UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(UUIManager::Get()->GetGameInstance(), UMGClass, UUIManager::Get()->GetPlayerController());
    Views.Add(UserWidget);
    return UserWidget;
}

void UIPresenter::SetVisible(bool InVisible)
{
    if (bHasDestroyed){
        UE_LOG(LogCmsUI, Warning, TEXT("UI %s has destroyed"), *GetClass()->GetName());
        return;
    }

    if (bVisible == InVisible) {
        return;
    }

    bVisible = InVisible;

    OnVisiblityChangedDelegate.Broadcast();

    if (bVisible){
        if (!bHasAddToViewport){
            OnCreateUI();
            for(auto View : Views){
                View->AddToViewport(ZOrder);
                CachedVisibilities.Add(View, View->GetVisibility());
            }
            OnConnectUI();
            bHasAddToViewport = true;
        }
        else{
            for (auto View : Views){
                View->SetVisibility(CachedVisibilities[View]);
            }
        }
        OnShowEvent();
    }
    else{
        for (auto View : Views) {
            View->SetVisibility(ESlateVisibility::Hidden);
        }
        OnCloseEvent();
    }
}

void UIPresenter::Show()
{
    SetVisible(true);
}

void UIPresenter::Hide()
{
    SetVisible(false);
}


bool UIPresenter::IsVisible() const
{
    return bVisible;
}

UWorld* UIPresenter::GetWorld() const
{
    return UUIManager::Get()->GetPlayerController()->GetWorld();
}

