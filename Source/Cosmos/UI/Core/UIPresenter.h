#pragma once

#include "Blueprint/UserWidget.h"
#include "UICommonInclude.h"
#include "CmsLogChannels.h"
#include "UIPresenter.generated.h"

UCLASS()
class UIPresenter : public UObject{
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
    UUserWidget* CreateView(UClass* UMGClass);

    template<typename UMGClass>
    UMGClass* CreateView() { return Cast<UMGClass>(CreateView(UMGClass::StaticClass())); }

    void BeginDestroy() override;
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
    TArray<TObjectPtr<UUserWidget>> Views;
};