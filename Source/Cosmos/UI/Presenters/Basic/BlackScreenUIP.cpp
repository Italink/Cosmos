#include "BlackScreenUIP.h"

void UBlackScreenUIP::OnCreateUI()
{
	ZOrder = 9999;
	View = CreateView<UUMG_BlackScreen_C>();
}

void UBlackScreenUIP::OnConnectUI()
{

}

void UBlackScreenUIP::OnDestroy()
{
    if (FadeInTickerHandle.IsValid()) {
        FTSTicker::GetCoreTicker().RemoveTicker(FadeInTickerHandle);
    }
    if (FadeOutTickerHandle.IsValid()) {
        FTSTicker::GetCoreTicker().RemoveTicker(FadeOutTickerHandle);
    }
}

void UBlackScreenUIP::FadeIn(float InFadeInTimeSec, TFunction<void()> InFinishedCallback /*= {}*/, FSlateColor InColor /*= FSlateColor(FLinearColor::Black)*/, float InBlurStrength /*= 0*/)
{
    if (IsVisible()) {
        UE_LOG(LogCmsUI, Warning, TEXT("Can`t add black screen again"));
    }
    Show();

    TimerProgress = 0.0f;
    FadeInTimeSec = InFadeInTimeSec;
    FadeInFinishedCallback = InFinishedCallback;

    View->Blur->SetBlurStrength(InBlurStrength);
    View->Color->SetBrushTintColor(InColor);
    View->SetRenderOpacity(FadeInTimeSec > 0.0f ? 0.0f : 1.0f);

    if (FadeOutTickerHandle.IsValid()) {
        FTSTicker::GetCoreTicker().RemoveTicker(FadeOutTickerHandle);
        FadeOutTickerHandle.Reset();
    }

    if (!FadeInTickerHandle.IsValid()) {
        FadeInTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float Delta) {
            TimerProgress += Delta;
            if (TimerProgress < FadeInTimeSec) {
                float FadeInProgress = TimerProgress / FadeInTimeSec;
                View->SetRenderOpacity((float)FadeInProgress);
                return true;
            }
            else {
                FadeInTickerHandle.Reset();
                if (FadeInFinishedCallback) {
                    FadeInFinishedCallback();
                    FadeInFinishedCallback = {};
                }
                View->SetRenderOpacity(1.0f);
                return false;
            }
        }));
    }
}

void UBlackScreenUIP::FadeOut(float InFadeOutTimeSec, TFunction<void()> InFinishedCallback /*= {}*/)
{
    TimerProgress = 0.0f;
    FadeOutTimeSec = InFadeOutTimeSec;
    FadeOutFinishedCallback = InFinishedCallback;
    if (FadeInTickerHandle.IsValid()) {
        FTSTicker::GetCoreTicker().RemoveTicker(FadeInTickerHandle);
        FadeInTickerHandle.Reset();
    }

    if (!FadeOutTickerHandle.IsValid()) {
        FadeOutTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float Delta) {
            TimerProgress += Delta;
            if (TimerProgress < FadeOutTimeSec){
                float FadeOutProgress = TimerProgress / FadeOutTimeSec;
                View->SetRenderOpacity(1 - FadeOutProgress);
                return true;
            }
            else{
                FadeOutTickerHandle.Reset();
                Hide();
                if (FadeOutFinishedCallback){
                    FadeOutFinishedCallback();
                    FadeOutFinishedCallback = {};
                }
                return false;
            }
        }));
    }
}
