#pragma once

#include "UI/Core/UIController.h"
#include "UI/View/AiAgent/View_AiAgent.inl"
#include "Http.h"
#include "WebSocketsModule.h"
#include "Json.h"
#include "HttpModule.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/FileManager.h"
#include "Containers/Array.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "UObject/WeakObjectPtr.h"
#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"
#include "AiAgentUIC.generated.h"

UCLASS()
class UAiAgentUIC : public UIController{
    GENERATED_BODY()
public:    
    void OnCreateUI() override;
    void OnConnectUI() override;
    void OnDestroy() override;
    void OnTickEvent(float Delta) override;

    UFUNCTION()
    void OnAskClicked();
private:
    TObjectPtr<UView_AiAgent_C> View;
};