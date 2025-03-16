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
#include "AiAgentUIC.generated.h"

UCLASS()
class UAiAgentUIC : public UIController{
    GENERATED_BODY()
public:    
    void OnCreateUI() override;
    void OnConnectUI() override;
    void OnDestroy() override;
private:
    void Ask(FString Message);
    void OnHttpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
private:
    TObjectPtr<UView_AiAgent_C> View;
    FString ChatApiKey = "42119089-28c1-44c6-9915-b0506639a56d";


    TSharedPtr<IWebSocket> WebSocket;
    FString AppId = "4830288771";
    FString Token = "bfvYpkLuXUffWFEDfXL4CdMmThi9qbBQ";
    FString Cluster = "volcano_tts";
    FString VoiceType = "BV700_V2_streaming";
    FString Host = "openspeech.bytedance.com";
    FString ApiUrl = "wss://openspeech.bytedance.com/api/v1/tts/ws_binary";
    TArray<uint8> DefaultHeader = { 0x11, 0x10, 0x11, 0x00 };

    void Speak(FString Text);
    void OnConnected();
    void OnConnectionError(const FString& Error);
    void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
    void OnMessage(const FString& Message);
    void OnMessageRaw(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
    bool ParseResponse(const TArray<uint8>& Res, TArray<uint8>& File);
};