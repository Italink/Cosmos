#pragma once

#include "UI/Core/UIPresenter.h"
#include "UI/View/Basic/UMG_BlackScreen.inl"
#include "BlackScreenUIP.generated.h"

UCLASS()
class UBlackScreenUIP : public UIPresenter{
    GENERATED_BODY()
public:    
	void FadeIn(float InFadeInTimeSec, TFunction<void()> InFinishedCallback = {}, FSlateColor InColor = FSlateColor(FLinearColor::Black), float InBlurStrength = 0);
	void FadeOut(float InFadeOutTimeSec, TFunction<void()> InFinishedCallback = {});
protected:
	void OnCreateUI() override;
	void OnConnectUI() override;
	void OnDestroy() override;
private:
    TObjectPtr<UUMG_BlackScreen_C> View;

    float TimerProgress = 0.0f;

	float FadeInTimeSec = 0.0f;
	FTSTicker::FDelegateHandle FadeInTickerHandle;
    TFunction<void()> FadeInFinishedCallback;

	float FadeOutTimeSec = 0.0f;
	FTSTicker::FDelegateHandle  FadeOutTickerHandle;
	TFunction<void()> FadeOutFinishedCallback;
};