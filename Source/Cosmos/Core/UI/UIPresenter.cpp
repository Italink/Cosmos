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
    for (auto View : TopLevelWidgets) {
        if (View) {
            View->RemoveFromParent();
        }
    }
    TopLevelWidgets.Empty();
    CachedVisibilities.Empty();

    for (auto Card : TopLevelWidgetCards) {
        Card->Destroy();
    }
    TopLevelWidgetCards.Empty();
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
        TopLevelWidgets.Add(UserWidget);
    }
    return UserWidget;
}

AWidgetCard* UIPresenter::CreateWidgetCard(UClass* UMGClass, FVector2D DrawSize /*= FVector2D(500, 500)*/)
{
    if (!UMGClass)
        return nullptr;
    UWorld* World = GetWorld();
    UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(UUIManager::Get()->GetGameInstance(), UMGClass, UUIManager::Get()->GetPlayerController());
    AWidgetCard* WidgetCard = World->SpawnActor<AWidgetCard>();
    WidgetCard->SetDrawSize(DrawSize);
    WidgetCard->SetWidget(UserWidget);
    WidgetCard->SetHidden(true);
    return WidgetCard;
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
            for(auto View : TopLevelWidgets){
                View->AddToViewport(ZOrder);
                CachedVisibilities.Add(View, View->GetVisibility());
            }
            for (auto Card : TopLevelWidgetCards) {
                Card->SetHidden(false);
            }
            OnConnectUI();
            bHasAddToViewport = true;
        }
        else{
            for (auto View : TopLevelWidgets){
                View->SetVisibility(CachedVisibilities[View]);
            }
            for (auto Card : TopLevelWidgetCards) {
                Card->SetHidden(false);
            }
        }
        OnShowEvent();
    }
    else{
        for (auto View : TopLevelWidgets) {
            View->SetVisibility(ESlateVisibility::Hidden);
        }
        for (auto Card : TopLevelWidgetCards) {
            Card->SetHidden(true);
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
