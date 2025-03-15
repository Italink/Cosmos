#include "UIController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UIController::UIController(const FObjectInitializer& ObjectInitializer)
{
    if (!HasAnyFlags(RF_ClassDefaultObject)) {
        FUIManager::Get()->RegisterController(this);
    }
}

void UIController::BeginDestroy()
{
    Super::BeginDestroy();
    if (!HasAnyFlags(RF_ClassDefaultObject)) {
        FUIManager::Get()->UnRegisterController(this);
    }
}

UUserWidget* UIController::CreateView(UClass* UMGClass)
{
    if(!UMGClass)
        return nullptr;
    UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(FUIManager::Get()->GetGameInstance(), UMGClass, FUIManager::Get()->GetPlayerController());
    Views.Add(UserWidget);
    return UserWidget;
}

void UIController::SetVisible(bool InVisible)
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

void UIController::Show()
{
    SetVisible(true);
}

void UIController::Hide()
{
    SetVisible(false);
}

void UIController::ForceDestory()
{
    if (!bHasDestroyed){
        bHasDestroyed = true;
        if (bVisible){
            OnCloseEvent();
        }
        for (auto View : Views) {
            if (View){
                View->RemoveFromParent();
            }
        }
        Views.Reset();
        CachedVisibilities.Reset();
        OnDestroy();
        FUIManager::Get()->UnRegisterController(this);
    }
}

