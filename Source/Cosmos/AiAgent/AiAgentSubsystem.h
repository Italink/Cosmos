#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "AiAgentSubsystem.generated.h"

class IWebSocket;
class USoundWaveProcedural;
class UAudioComponent;

UCLASS()
class UAiAgentSubsystem : public UWorldSubsystem {
    GENERATED_BODY()
public:
    static UAiAgentSubsystem* Get(UObject* WorldContext);

    void Ask(FString Message, TFunction<void(FString Response)> Callback);
    void Speak(FString Text);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;
    virtual void Deinitialize() override;

    void OnConnected();
    void OnConnectionError(const FString& Error);
    void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
    void OnMessage(const FString& Message);
    void OnMessageRaw(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
    bool ParseResponse(const TArray<uint8>& Res);
private:
    TSharedPtr<IWebSocket> WebSocket;
    TArray<uint8> DefaultHeader = { 0x11, 0x10, 0x11, 0x00 };
    TArray<uint8> NetBuffer;
    FString PendingSpeakText;

    UPROPERTY()
    TObjectPtr<USoundWaveProcedural> WaveProcedural;

    UPROPERTY()
    TObjectPtr<UAudioComponent> WaveProceduralAudioComponent;
};