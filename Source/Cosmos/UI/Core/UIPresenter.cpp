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
    if (!HasAnyFlags(RF_ClassDefaultObject) && !bHasDestroyed) {
        bHasDestroyed = true;
        ReleaseResource();
    }
}

void UIPresenter::ReleaseResource()
{
    if (bVisible) {
        OnCloseEvent();
    }
    for (auto View : TopLevelViews) {
        if (View) {
            View->RemoveFromParent();
        }
    }
    TopLevelViews.Empty();
    CachedVisibilities.Empty();

    UClass* Class = GetClass();
    for (auto DynamicLambda : DynamicLambdas) {
        Class->RemoveFunctionFromFunctionMap(DynamicLambda);
    }
    DynamicLambdas.Empty();

    OnDestroy();
    UUIManager::Get()->UnRegisterPresenter(this);
}

void UIPresenter::ForceDestory()
{
    if (!bHasDestroyed) {
        bHasDestroyed = true;
        ReleaseResource();
    }
}

UUserWidget* UIPresenter::CreateView(UClass* UMGClass, bool bTopLevel)
{
    if(!UMGClass)
        return nullptr;
    UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(UUIManager::Get()->GetGameInstance(), UMGClass, UUIManager::Get()->GetPlayerController());
    if (bTopLevel) {
        TopLevelViews.Add(UserWidget);
    }
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
            for(auto View : TopLevelViews){
                View->AddToViewport(ZOrder);
                CachedVisibilities.Add(View, View->GetVisibility());
            }
            OnConnectUI();
            bHasAddToViewport = true;
        }
        else{
            for (auto View : TopLevelViews){
                View->SetVisibility(CachedVisibilities[View]);
            }
        }
        OnShowEvent();
    }
    else{
        for (auto View : TopLevelViews) {
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

void UIPresenter::ProcessEvent(UFunction* Function, void* Parms)
{
    if (UDynamicLambda* DynamicLambda = Cast<UDynamicLambda>(Function)) {
        DynamicLambda->Invoke(Parms);
    }
    else {
        UObject::ProcessEvent(Function, Parms);
    }
}

