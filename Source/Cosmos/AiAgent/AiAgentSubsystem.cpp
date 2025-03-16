#include "AiAgentSubsystem.h"
#include "IWebSocket.h"
#include "Misc/Compression.h"
#include "Misc/ByteSwap.h"
#include "Sound/SoundWaveProcedural.h"
#include "Kismet/GameplayStatics.h"
#include "SampleBuffer.h"
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
#include "AiAgentSettings.h"

TArray<uint8> GZipCompress(TArray<uint8> PayloadBytes) {
    TArray<uint8> OutCompressedData;
    int32 CompressedSize = FCompression::CompressMemoryBound(NAME_Gzip, PayloadBytes.Num());
    OutCompressedData.SetNumUninitialized(CompressedSize);
    if (FCompression::CompressMemory(NAME_Gzip, OutCompressedData.GetData(), CompressedSize, PayloadBytes.GetData(), PayloadBytes.Num()))
    {
        OutCompressedData.SetNum(CompressedSize, EAllowShrinking::No);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to compress data "));
    }
    return OutCompressedData;
}

UAiAgentSubsystem* UAiAgentSubsystem::Get(UObject* WorldContext)
{
    if (WorldContext == nullptr)
        return nullptr;
    UWorld* World = WorldContext->GetWorld();
    return World->GetSubsystem<UAiAgentSubsystem>();
}

void UAiAgentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UAiAgentSettings* Settings = GetMutableDefault<UAiAgentSettings>();
	TMap<FString, FString> Headers;
	Headers.Add("Authorization", FString::Printf(TEXT("Bearer; %s"), *Settings->TtsToken));
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(Settings->TtsApiUrl, "", Headers);
	WebSocket->OnConnected().AddUObject(this, &UAiAgentSubsystem::OnConnected);
	WebSocket->OnConnectionError().AddUObject(this, &UAiAgentSubsystem::OnConnectionError);
	WebSocket->OnClosed().AddUObject(this, &UAiAgentSubsystem::OnClosed);
	WebSocket->OnMessage().AddUObject(this, &UAiAgentSubsystem::OnMessage);
	WebSocket->OnRawMessage().AddUObject(this, &UAiAgentSubsystem::OnMessageRaw);
	WebSocket->Connect();
}

void UAiAgentSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    WaveProcedural = NewObject<USoundWaveProcedural>();
    WaveProcedural->SetSampleRate(24000);
    WaveProcedural->NumChannels = 1;
    WaveProcedural->SampleByteSize = 1;
    WaveProcedural->Duration = INDEFINITELY_LOOPING_DURATION;
    WaveProcedural->SoundGroup = SOUNDGROUP_Voice;

    UWorld* World = GetWorld();
    WaveProceduralAudioComponent = UGameplayStatics::CreateSound2D(World, WaveProcedural);
    if (WaveProceduralAudioComponent) {
        WaveProceduralAudioComponent->bAutoDestroy = false;
        WaveProceduralAudioComponent->SetSound(WaveProcedural);
        WaveProceduralAudioComponent->Play();
    }
}

void UAiAgentSubsystem::Deinitialize()
{
    if (WebSocket.IsValid() && WebSocket->IsConnected()){
        WebSocket->Close();
    }
}

void UAiAgentSubsystem::Ask(FString Message, TFunction<void(FString Response)> Callback)
{
    UAiAgentSettings* Settings = GetMutableDefault<UAiAgentSettings>();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL("https://ark.cn-beijing.volces.com/api/v3/chat/completions");
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *Settings->ChatApiKey));

    FString RequestBody = FString::Printf(TEXT(R"({
        "model": "doubao-1.5-pro-32k-250115",
        "messages": [
            {
                "role": "user",
                "content": "%s"
            }
        ]
      })"), *Message);

    HttpRequest->SetContentAsString(RequestBody);

    HttpRequest->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
        if (bWasSuccessful && Response.IsValid())
        {
            int32 StatusCode = Response->GetResponseCode();
            UE_LOG(LogTemp, Warning, TEXT("Response Status Code: %d"), StatusCode);

            FString ResponseContent = Response->GetContentAsString();

            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseContent);
            if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
            {
                FString Id = JsonObject->GetStringField("id");
                FString ObjectType = JsonObject->GetStringField("object");
                int32 Created = JsonObject->GetIntegerField("created");
                FString Model = JsonObject->GetStringField("model");
                FString ServiceTier = JsonObject->GetStringField("service_tier");

                UE_LOG(LogTemp, Warning, TEXT("ID: %s"), *Id);
                UE_LOG(LogTemp, Warning, TEXT("Object Type: %s"), *ObjectType);
                UE_LOG(LogTemp, Warning, TEXT("Created: %d"), Created);
                UE_LOG(LogTemp, Warning, TEXT("Model: %s"), *Model);
                UE_LOG(LogTemp, Warning, TEXT("Service Tier: %s"), *ServiceTier);

                TArray<TSharedPtr<FJsonValue>> ChoicesArray = JsonObject->GetArrayField("choices");
                for (const auto& ChoiceValue : ChoicesArray)
                {
                    TSharedPtr<FJsonObject> ChoiceObject = ChoiceValue->AsObject();
                    int32 Index = ChoiceObject->GetIntegerField("index");
                    UE_LOG(LogTemp, Warning, TEXT("Choice Index: %d"), Index);

                    TSharedPtr<FJsonObject> MessageObject = ChoiceObject->GetObjectField("message");
                    FString Role = MessageObject->GetStringField("role");
                    FString Content = MessageObject->GetStringField("content");
                    UE_LOG(LogTemp, Warning, TEXT("Role: %s"), *Role);
                    UE_LOG(LogTemp, Warning, TEXT("Content: %s"), *Content);
                    Callback(Content);
                    FString FinishReason = ChoiceObject->GetStringField("finish_reason");
                    UE_LOG(LogTemp, Warning, TEXT("Finish Reason: %s"), *FinishReason);
                }

                TSharedPtr<FJsonObject> UsageObject = JsonObject->GetObjectField("usage");
                int32 PromptTokens = UsageObject->GetIntegerField("prompt_tokens");
                int32 CompletionTokens = UsageObject->GetIntegerField("completion_tokens");
                int32 TotalTokens = UsageObject->GetIntegerField("total_tokens");

                UE_LOG(LogTemp, Warning, TEXT("Prompt Tokens: %d"), PromptTokens);
                UE_LOG(LogTemp, Warning, TEXT("Completion Tokens: %d"), CompletionTokens);
                UE_LOG(LogTemp, Warning, TEXT("Total Tokens: %d"), TotalTokens);

                TSharedPtr<FJsonObject> PromptTokensDetailsObject = UsageObject->GetObjectField("prompt_tokens_details");
                int32 CachedTokens = PromptTokensDetailsObject->GetIntegerField("cached_tokens");
                UE_LOG(LogTemp, Warning, TEXT("Cached Tokens: %d"), CachedTokens);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("HTTP request failed!"));
        }
    });

    HttpRequest->ProcessRequest();
}

void UAiAgentSubsystem::Speak(FString Text)
{
    UAiAgentSettings* Settings = GetMutableDefault<UAiAgentSettings>();
    TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject());
    TSharedPtr<FJsonObject> AppJson = MakeShareable(new FJsonObject());
    AppJson->SetStringField(TEXT("appid"), Settings->AppId);
    AppJson->SetStringField(TEXT("token"), TEXT("access_token"));
    AppJson->SetStringField(TEXT("cluster"), Settings->TtsCluster);

    RequestJson->SetObjectField(TEXT("app"), AppJson);

    TSharedPtr<FJsonObject> UserJson = MakeShareable(new FJsonObject());
    UserJson->SetStringField(TEXT("uid"), TEXT("388808087185088"));
    RequestJson->SetObjectField(TEXT("user"), UserJson);

    TSharedPtr<FJsonObject> AudioJson = MakeShareable(new FJsonObject());
    AudioJson->SetStringField(TEXT("voice_type"), Settings->TtsVoiceType);
    AudioJson->SetStringField(TEXT("encoding"), TEXT("pcm"));
    AudioJson->SetNumberField(TEXT("speed_ratio"), 1.0);
    AudioJson->SetNumberField(TEXT("volume_ratio"), 1.0);
    AudioJson->SetNumberField(TEXT("pitch_ratio"), 1.0);
    RequestJson->SetObjectField(TEXT("audio"), AudioJson);

    TSharedPtr<FJsonObject> RequestInfoJson = MakeShareable(new FJsonObject());
    RequestInfoJson->SetStringField(TEXT("reqid"), FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens));
    RequestInfoJson->SetStringField(TEXT("text"), Text);
    RequestInfoJson->SetStringField(TEXT("text_type"), TEXT("plain"));
    RequestInfoJson->SetStringField(TEXT("operation"), TEXT("submit"));
    RequestJson->SetObjectField(TEXT("request"), RequestInfoJson);

    FString PayloadJsonString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&PayloadJsonString);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), JsonWriter);

    TArray<uint8> PayloadBytes;
    FTCHARToUTF8 Converter(*PayloadJsonString);
    PayloadBytes.Append((const uint8*)Converter.Get(), Converter.Length());

    PayloadBytes = GZipCompress(PayloadBytes);

    TArray<uint8> FullClientRequest = DefaultHeader;
    int32 PayloadSize = PayloadBytes.Num();
#if PLATFORM_LITTLE_ENDIAN
    PayloadSize = ByteSwap(PayloadSize);
#endif
    FullClientRequest.Append((const uint8*)&PayloadSize, sizeof(PayloadSize));
    FullClientRequest.Append(PayloadBytes);

    UE_LOG(LogTemp, Warning, TEXT("------------------------ test 'submit' -------------------------"));
    UE_LOG(LogTemp, Warning, TEXT("request json: %s"), *PayloadJsonString);
    UE_LOG(LogTemp, Warning, TEXT("request bytes size: %d"), FullClientRequest.Num());

    WebSocket->Send(FullClientRequest.GetData(), FullClientRequest.Num(), true);
}

void UAiAgentSubsystem::OnConnected()
{
    UE_LOG(LogTemp, Warning, TEXT("WebSocket connected"));
}

void UAiAgentSubsystem::OnConnectionError(const FString& Error)
{
    UE_LOG(LogTemp, Error, TEXT("WebSocket connection error: %s"), *Error);
}

void UAiAgentSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
    UE_LOG(LogTemp, Warning, TEXT("WebSocket closed. StatusCode: %d, Reason: %s, Clean: %d"), StatusCode, *Reason, bWasClean);
}

void UAiAgentSubsystem::OnMessage(const FString& Message)
{
    UE_LOG(LogTemp, Warning, TEXT("Received text message: %s"), *Message);
}

void UAiAgentSubsystem::OnMessageRaw(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
    NetBuffer.Append((const uint8*)Data, Size);
    if (BytesRemaining == 0) {
        bool Done = ParseResponse(NetBuffer);
        if (Done){
            //WebSocket->Close();
        }
        NetBuffer.Reset();
    }
}

bool UAiAgentSubsystem::ParseResponse(const TArray<uint8>& Res)
{
    UE_LOG(LogTemp, Warning, TEXT("--------------------------- response ---------------------------"));
    uint8 ProtocolVersion = Res[0] >> 4;
    uint8 HeaderSize = Res[0] & 0x0f;
    uint8 MessageType = Res[1] >> 4;
    uint8 MessageTypeSpecificFlags = Res[1] & 0x0f;
    uint8 SerializationMethod = Res[2] >> 4;
    uint8 MessageCompression = Res[2] & 0x0f;
    uint8 Reserved = Res[3];

    // 获取HeaderExtensions
    TArray<uint8> HeaderExtensions;
    if (HeaderSize * 4 > 4){
        int32 startIndex = 4;
        int32 length = HeaderSize * 4 - 4;
        for (int32 i = 0; i < length; ++i){
            HeaderExtensions.Add(Res[startIndex + i]);
        }
    }

    // 获取Payload
    TArray<uint8> Payload;
    int32 payloadStartIndex = HeaderSize * 4;
    for (int32 i = payloadStartIndex; i < Res.Num(); ++i){
        Payload.Add(Res[i]);
    }

    UE_LOG(LogTemp, Warning, TEXT("            Protocol version: 0x%x - version %d"), ProtocolVersion, ProtocolVersion);
    UE_LOG(LogTemp, Warning, TEXT("                 Header size: 0x%x - %d bytes "), HeaderSize, HeaderSize * 4);
	UE_LOG(LogTemp, Warning, TEXT("                Message type: 0x%x -"), MessageType);
	UE_LOG(LogTemp, Warning, TEXT(" Message type specific flags: 0x%x -"), MessageTypeSpecificFlags);
	UE_LOG(LogTemp, Warning, TEXT("Message serialization method: 0x%x -"), SerializationMethod);
	UE_LOG(LogTemp, Warning, TEXT("         Message compression: 0x%x -"), MessageCompression);
    UE_LOG(LogTemp, Warning, TEXT("                    Reserved: 0x%02x"), Reserved);

	if (HeaderSize != 1){
        UE_LOG(LogTemp, Warning, TEXT("           Header extensions: %s"), *BytesToString(HeaderExtensions.GetData(), HeaderExtensions.Num()));
    }

    if (MessageType == 0xb){
        int32 SequenceNumber;
        if (MessageTypeSpecificFlags == 0) {
            UE_LOG(LogTemp, Warning, TEXT("                Payload size: 0"));
            return false;
        }
		else{
            FMemory::Memcpy(&SequenceNumber, Payload.GetData(), sizeof(int32));
#if PLATFORM_LITTLE_ENDIAN
            SequenceNumber = ByteSwap(SequenceNumber);
#endif
            uint32 PayloadSize;
            FMemory::Memcpy(&PayloadSize, Payload.GetData() + sizeof(uint32), sizeof(uint32));
#if PLATFORM_LITTLE_ENDIAN
            PayloadSize = ByteSwap(PayloadSize);
#endif
            Payload.RemoveAt(0, 2 * sizeof(int32));

            UE_LOG(LogTemp, Warning, TEXT("             Sequence number: %d"), SequenceNumber);
            UE_LOG(LogTemp, Warning, TEXT("                Payload size: %d bytes"), PayloadSize);
        }
        if (!Payload.IsEmpty()) {
            WaveProcedural->QueueAudio((uint8*)Payload.GetData(), Payload.Num());
        }
        if (SequenceNumber < 0){
            return true;
        }
        else{
            return false;
        }
    }
    else if (MessageType == 0xf){
        int32 Code;
        FMemory::Memcpy(&Code, Payload.GetData(), sizeof(int32));
#if PLATFORM_LITTLE_ENDIAN
        Code = ByteSwap(Code);
#endif
        int32 MsgSize;
        FMemory::Memcpy(&MsgSize, Payload.GetData() + sizeof(int32), sizeof(int32));
#if PLATFORM_LITTLE_ENDIAN
        MsgSize = ByteSwap(MsgSize);
#endif
        TArray<uint8> ErrorMsg;
        int32 errorMsgStartIndex = 2 * sizeof(int32);
        for (int32 i = errorMsgStartIndex; i < Payload.Num(); ++i){
            ErrorMsg.Add(Payload[i]);
        }

		if (MessageCompression == 1){
			/*ErrorMsg = GzipDecompress(ErrorMsg);*/
		}

        FString ErrorMsgString = FString(UTF8_TO_TCHAR(ErrorMsg.GetData()));
        UE_LOG(LogTemp, Warning, TEXT("          Error message code: %d"), Code);
        UE_LOG(LogTemp, Warning, TEXT("          Error message size: %d bytes"), MsgSize);
        UE_LOG(LogTemp, Warning, TEXT("               Error message: %s"), *ErrorMsgString);
        return true;
    }
    else if (MessageType == 0xc){
        int32 MsgSize;
        FMemory::Memcpy(&MsgSize, Payload.GetData(), sizeof(int32));
#if PLATFORM_LITTLE_ENDIAN
        MsgSize = ByteSwap(MsgSize);
#endif
        Payload.RemoveAt(0, sizeof(int32));

        if (MessageCompression == 1){
            //Payload = GzipDecompress(Payload);
        }

        FString FrontendMessage = FString(UTF8_TO_TCHAR(Payload.GetData()));
        UE_LOG(LogTemp, Warning, TEXT("            Frontend message: %s"), *FrontendMessage);
    }
    else{
        UE_LOG(LogTemp, Warning, TEXT("undefined message type!"));
        return true;
    }

    return false;
}