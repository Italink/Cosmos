#pragma once

#include "Blueprint/UserWidget.h"
#include "UICommonInclude.h"
#include "WidgetCardActor.h"
#include "Core/Basic/ObjectPlus.h"
#include "UIPresenter.generated.h"

UCLASS()
class UIPresenter : public UObjectPlus {
    GENERATED_UCLASS_BODY()
    friend class UUIManager;
public:    
    FSimpleMulticastDelegate OnVisiblityChangedDelegate;
public:
    void SetVisible(bool bVisible);
    void Show();
    void Hide();
    void ForceDestory();
public:
    bool IsVisible() const;
protected:
    UUserWidget* CreateView(UClass* UMGClass, bool bTopLevel = true);

    template<typename UMGClass>
    UMGClass* CreateView(bool bTopLevel = true) { 
        return Cast<UMGClass>(CreateView(UMGClass::StaticClass(), bTopLevel)); 
    }

    AWidgetCard* CreateWidgetCard(UClass* UMGClass, FVector2D DrawSize = FVector2D(500, 500));

    template<typename UMGClass>
    AWidgetCard* CreateWidgetCard(FVector2D DrawSize = FVector2D(500, 500)) {
        return (CreateWidgetCard(UMGClass::StaticClass(), DrawSize));
    }

    virtual void BeginDestroy() override;
    virtual void ReleaseResource();

    virtual void OnCreateUI(){}
    virtual void OnConnectUI() {}
    virtual void OnDestroy() {}
    virtual void OnShowEvent() {}
    virtual void OnCloseEvent() {}
    virtual void OnTickEvent(float Delta) {}
    virtual UWorld* GetWorld() const override;
public:
    int ZOrder = 0;
    TMap<UUserWidget*, ESlateVisibility> CachedVisibilities;
    bool bVisible = false;
    bool bHasDestroyed = false;
    bool bHasAddToViewport = false;
       
    UPROPERTY()
    TArray<TObjectPtr<UUserWidget>> TopLevelWidgets;

    UPROPERTY()
    TArray<TObjectPtr<AWidgetCard>> TopLevelWidgetCards;

};
