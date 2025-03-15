#pragma once

#include "Blueprint/UserWidget.h"
#include "UICommonInclude.h"
#include "UIManager.h"
#include "UIController.generated.h"

UCLASS()
class UIController : public UObject{
    GENERATED_UCLASS_BODY()
public:    
    FSimpleMulticastDelegate OnVisiblityChangedDelegate;
public:
    void BeginDestroy() override;

    UUserWidget* CreateView(UClass* UMGClass);

    template<typename UMGClass>
    UMGClass* CreateView() {
        return Cast<UMGClass>(CreateView(UMGClass::StaticClass()));
    }

    void SetVisible(bool bVisible);
    void Show();
    void Hide();

    void ForceDestory();

protected:

    virtual void OnCreateUI(){}

    virtual void OnConnectUI() {}

    virtual void OnDestroy() {}

    virtual void OnShowEvent() {}

    virtual void OnCloseEvent() {}

    virtual void OnTickEvent(float Delta) {}
public:
    int ZOrder = 0;
    TMap<UUserWidget*, ESlateVisibility> CachedVisibilities;
    bool bVisible = false;
    bool bHasDestroyed = false;
    bool bHasAddToViewport = false;
    TArray<TObjectPtr<UUserWidget>> Views;
};