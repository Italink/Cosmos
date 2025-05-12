#pragma once

#include "CosmosGlobals.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/EngineSubsystem.h"
#include "UI/Presenters/Basic/BlackScreenUIP.h"
#include "UIManager.generated.h"

UCLASS(MinimalAPI)
class UUIManager : public UGameInstanceSubsystem, public FTickableGameObject{
	GENERATED_BODY()
public:
	static UUIManager* Get(UObject* InWorldContext = nullptr);

	void SetPlayerController(APlayerController* InPlayerController);

	APlayerController* GetPlayerController();

	void RegisterPresenter(UIPresenter* InPresenter);
	void UnRegisterPresenter(UIPresenter* InPresenter);

	UBlackScreenUIP* GetBlackScreen();
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//~ Begin FTickableGameObject
	virtual TStatId GetStatId() const override;
	virtual void Tick(float InDeltaSeconds) override;
	virtual bool IsTickable() const override { return true;  }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }
	//~ End FTickableGameObject

private:
	TArray<TObjectPtr<UIPresenter>> AllPresenters;
	TObjectPtr<APlayerController> CurrentPlayerController;

	UPROPERTY()
	TObjectPtr<class UBlackScreenUIP> BlackScreenUIP;
};