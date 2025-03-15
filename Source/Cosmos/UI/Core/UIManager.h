#pragma once

#include "Blueprint/UserWidget.h"
#include "Subsystems/EngineSubsystem.h"
#include "CmsLogChannels.h"

class UIController;

class FUIManager: public FTickableGameObject {
public:
	static FUIManager* Get();

	void SetPlayerController(APlayerController* InPlayerController);

	UGameInstance* GetGameInstance();
	APlayerController* GetPlayerController();

	void RegisterController(UIController* InController);
	void UnRegisterController(UIController* InController);

protected:
	//~ Begin FTickableGameObject
	virtual TStatId GetStatId() const override;
	virtual void Tick(float InDeltaSeconds) override;
	virtual bool IsTickable() const override { return true;  }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }
	//~ End FTickableGameObject

private:
	TArray<TObjectPtr<UIController>> AllControllers;
	TObjectPtr<UGameInstance> CurrentGameInstance;
	TObjectPtr<APlayerController> CurrentPlayerController;
};