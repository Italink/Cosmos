#pragma once

#include "Blueprint/UserWidget.h"
#include "UICommonInclude.h"
#include "CmsLogChannels.h"
#include "DynamicLambda.h"
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
    UUserWidget* CreateView(UClass* UMGClass, bool bTopLevel = true);

    template<typename UMGClass>
    UMGClass* CreateView(bool bTopLevel = true) { return Cast<UMGClass>(CreateView(UMGClass::StaticClass(), bTopLevel)); }

    void BeginDestroy() override;
    virtual void ReleaseResource();

    virtual void OnCreateUI(){}
    virtual void OnConnectUI() {}
    virtual void OnDestroy() {}
    virtual void OnShowEvent() {}
    virtual void OnCloseEvent() {}
    virtual void OnTickEvent(float Delta) {}
    virtual UWorld* GetWorld() const override;

    virtual void ProcessEvent(UFunction* Function, void* Parms) override;

    template <typename FuncType>
    FScriptDelegate CreateLambdaDynamic(FuncType Lambda) {
        UClass* Class = GetClass();
        UDynamicLambda* NewDynamicLambda = NewObject<UDynamicLambda>(Class);
        NewDynamicLambda->SetFlags(RF_Transient);
        NewDynamicLambda->SetupLambda(Lambda);
        DynamicLambdas.Add(NewDynamicLambda);
        Class->AddFunctionToFunctionMap(NewDynamicLambda, NewDynamicLambda->GetFName());
        FScriptDelegate ScriptDelegate;
        ScriptDelegate.BindUFunction(this, NewDynamicLambda->GetFName());
        return ScriptDelegate;
    }
public:
    int ZOrder = 0;
    TMap<UUserWidget*, ESlateVisibility> CachedVisibilities;
    bool bVisible = false;
    bool bHasDestroyed = false;
    bool bHasAddToViewport = false;

    UPROPERTY()
    TArray<TObjectPtr<UUserWidget>> TopLevelViews;

    UPROPERTY()
    TArray<TObjectPtr<UDynamicLambda>> DynamicLambdas;
};