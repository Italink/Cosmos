#pragma once

#include "Blueprint/UserWidget.h"
#include "AiAgentSettings.generated.h"

UCLASS(EditInlineNew, CollapseCategories, config = AiAgentSettings)
class COSMOS_API UAiAgentSettings : public UObject {
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Config)
    FString ChatApiKey;

    UPROPERTY(EditAnywhere, Config)
    FString Host = "openspeech.bytedance.com";

    UPROPERTY(EditAnywhere, Config)
    FString AppId;

    UPROPERTY(EditAnywhere, Config)
    FString TtsToken ;

    UPROPERTY(EditAnywhere, Config)
    FString TtsCluster = "volcano_tts";

    UPROPERTY(EditAnywhere, Config)
    FString TtsVoiceType = "BV700_V2_streaming";

    UPROPERTY(EditAnywhere, Config)
    FString TtsApiUrl = "wss://openspeech.bytedance.com/api/v1/tts/ws_binary";
};