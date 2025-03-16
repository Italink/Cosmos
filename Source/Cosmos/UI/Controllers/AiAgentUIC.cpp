#include "AiAgentUIC.h"
#include "IWebSocket.h"

void UAiAgentUIC::OnCreateUI()
{
	View = CreateView<UView_AiAgent_C>();
}

void UAiAgentUIC::OnConnectUI()
{
    Ask("Hello, What is your name");

    TMap<FString, FString> Headers;
    Headers.Add("Authorization", FString::Printf(TEXT("Bearer; %s"), *Token));

    WebSocket = FWebSocketsModule::Get().CreateWebSocket(ApiUrl, "", Headers);
    WebSocket->OnConnected().AddUObject(this, &UAiAgentUIC::OnConnected);
    WebSocket->OnConnectionError().AddUObject(this, &UAiAgentUIC::OnConnectionError);
    WebSocket->OnClosed().AddUObject(this, &UAiAgentUIC::OnClosed);
    WebSocket->OnMessage().AddUObject(this, &UAiAgentUIC::OnMessage);
    WebSocket->OnRawMessage().AddUObject(this, &UAiAgentUIC::OnMessageRaw);
    WebSocket->Connect();
}

void UAiAgentUIC::OnDestroy()
{
    if (WebSocket.IsValid() && WebSocket->IsConnected())
    {
        WebSocket->Close();
    }
}

void UAiAgentUIC::Ask(FString Message)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetURL("https://ark.cn-beijing.volces.com/api/v3/chat/completions");

    HttpRequest->SetVerb("POST");

    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *ChatApiKey));

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

    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UAiAgentUIC::OnHttpResponseReceived);

    HttpRequest->ProcessRequest();
}

void UAiAgentUIC::OnHttpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
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
}

void UAiAgentUIC::Speak(FString Text)
{
    TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject());
    TSharedPtr<FJsonObject> AppJson = MakeShareable(new FJsonObject());
    AppJson->SetStringField("appid", AppId);
    AppJson->SetStringField("token", "access_token");
    AppJson->SetStringField("cluster", Cluster);
    RequestJson->SetObjectField("app", AppJson);

    TSharedPtr<FJsonObject> UserJson = MakeShareable(new FJsonObject());
    UserJson->SetStringField("uid", "388808087185088");
    RequestJson->SetObjectField("user", UserJson);

    TSharedPtr<FJsonObject> AudioJson = MakeShareable(new FJsonObject());
    AudioJson->SetStringField("voice_type", VoiceType);
    AudioJson->SetStringField("encoding", "mp3");
    AudioJson->SetNumberField("speed_ratio", 1.0);
    AudioJson->SetNumberField("volume_ratio", 1.0);
    AudioJson->SetNumberField("pitch_ratio", 1.0);
    RequestJson->SetObjectField("audio", AudioJson);

    TSharedPtr<FJsonObject> RequestInfoJson = MakeShareable(new FJsonObject());
    RequestInfoJson->SetStringField("reqid", FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens));
    RequestInfoJson->SetStringField("text", Text);
    RequestInfoJson->SetStringField("text_type", "plain");
    RequestInfoJson->SetStringField("operation", "submit");
    RequestJson->SetObjectField("request", RequestInfoJson);

    FString PayloadJsonString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&PayloadJsonString);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), JsonWriter);

    TArray<uint8> PayloadBytes;
    FTCHARToUTF8 Converter(*PayloadJsonString);
    PayloadBytes.Append((const uint8*)Converter.Get(), Converter.Length());

    // 这里需要实现gzip压缩，UE5没有直接的gzip库，可使用第三方库如zlib
    // PayloadBytes = GzipCompress(PayloadBytes); 

    TArray<uint8> FullClientRequest = DefaultHeader;
    int32 PayloadSize = PayloadBytes.Num();
    FullClientRequest.Append((const uint8*)&PayloadSize, sizeof(PayloadSize));
    FullClientRequest.Append(PayloadBytes);

    UE_LOG(LogTemp, Warning, TEXT("------------------------ test 'submit' -------------------------"));
    UE_LOG(LogTemp, Warning, TEXT("request json: %s"), *PayloadJsonString);
    UE_LOG(LogTemp, Warning, TEXT("request bytes size: %d"), FullClientRequest.Num());

    WebSocket->Send(FullClientRequest.GetData(), FullClientRequest.Num(), true);
}

void UAiAgentUIC::OnConnected()
{
    UE_LOG(LogTemp, Warning, TEXT("WebSocket connected"));
    Speak("Hello, Who are you.");
}

void UAiAgentUIC::OnConnectionError(const FString& Error)
{
    UE_LOG(LogTemp, Error, TEXT("WebSocket connection error: %s"), *Error);
}

void UAiAgentUIC::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
    UE_LOG(LogTemp, Warning, TEXT("WebSocket closed. StatusCode: %d, Reason: %s, Clean: %d"), StatusCode, *Reason, bWasClean);
}

void UAiAgentUIC::OnMessage(const FString& Message)
{
    UE_LOG(LogTemp, Warning, TEXT("Received text message: %s"), *Message);
}

void UAiAgentUIC::OnMessageRaw(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
    TArray<uint8> ResponseBytes;
    ResponseBytes.Append((const uint8*)Data, Size);
    TArray<uint8> File;
    bool Done = ParseResponse(ResponseBytes, File);
    if (Done)
    {
        FFileHelper::SaveArrayToFile(File, *(FPaths::ProjectContentDir() / TEXT("test_submit.mp3")));
        WebSocket->Close();
    }
}

bool UAiAgentUIC::ParseResponse(const TArray<uint8>& Res, TArray<uint8>& File)
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
    if (HeaderSize * 4 > 4)
    {
        int32 startIndex = 4;
        int32 length = HeaderSize * 4 - 4;
        for (int32 i = 0; i < length; ++i)
        {
            HeaderExtensions.Add(Res[startIndex + i]);
        }
    }

    // 获取Payload
    TArray<uint8> Payload;
    int32 payloadStartIndex = HeaderSize * 4;
    for (int32 i = payloadStartIndex; i < Res.Num(); ++i)
    {
        Payload.Add(Res[i]);
    }

    UE_LOG(LogTemp, Warning, TEXT("            Protocol version: 0x%x - version %d"), ProtocolVersion, ProtocolVersion);
    UE_LOG(LogTemp, Warning, TEXT("                 Header size: 0x%x - %d bytes "), HeaderSize, HeaderSize * 4);
    //UE_LOG(LogTemp, Warning, TEXT("                Message type: 0x%x - %s"), MessageType, *GetMessageTypeString(MessageType));
    //UE_LOG(LogTemp, Warning, TEXT(" Message type specific flags: 0x%x - %s"), MessageTypeSpecificFlags, *GetMessageTypeSpecificFlagsString(MessageTypeSpecificFlags));
    //UE_LOG(LogTemp, Warning, TEXT("Message serialization method: 0x%x - %s"), SerializationMethod, *GetSerializationMethodString(SerializationMethod));
    //UE_LOG(LogTemp, Warning, TEXT("         Message compression: 0x%x - %s"), MessageCompression, *GetMessageCompressionString(MessageCompression));
    UE_LOG(LogTemp, Warning, TEXT("                    Reserved: 0x%02x"), Reserved);

    if (HeaderSize != 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("           Header extensions: %s"), *BytesToString(HeaderExtensions.GetData(), HeaderExtensions.Num()));
    }

    if (MessageType == 0xb) // audio-only server response
    {
        int32 SequenceNumber;
        if (MessageTypeSpecificFlags == 0) // no sequence number as ACK
        {
            UE_LOG(LogTemp, Warning, TEXT("                Payload size: 0"));
            return false;
        }
        else
        {
            
            FMemory::Memcpy(&SequenceNumber, Payload.GetData(), sizeof(int32));
            int32 PayloadSize;
            FMemory::Memcpy(&PayloadSize, Payload.GetData() + sizeof(int32), sizeof(int32));
            Payload.RemoveAt(0, 2 * sizeof(int32));

            UE_LOG(LogTemp, Warning, TEXT("             Sequence number: %d"), SequenceNumber);
            UE_LOG(LogTemp, Warning, TEXT("                Payload size: %d bytes"), PayloadSize);
        }

        File.Append(Payload);

        if (SequenceNumber < 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (MessageType == 0xf)
    {
        int32 Code;
        FMemory::Memcpy(&Code, Payload.GetData(), sizeof(int32));
        int32 MsgSize;
        FMemory::Memcpy(&MsgSize, Payload.GetData() + sizeof(int32), sizeof(int32));
        TArray<uint8> ErrorMsg;
        int32 errorMsgStartIndex = 2 * sizeof(int32);
        for (int32 i = errorMsgStartIndex; i < Payload.Num(); ++i)
        {
            ErrorMsg.Add(Payload[i]);
        }

        // 这里需要实现gzip解压缩，UE5没有直接的gzip库，可使用第三方库如zlib
        // if (MessageCompression == 1)
        // {
        //     ErrorMsg = GzipDecompress(ErrorMsg);
        // }

        FString ErrorMsgString = FString(UTF8_TO_TCHAR(ErrorMsg.GetData()));
        UE_LOG(LogTemp, Warning, TEXT("          Error message code: %d"), Code);
        UE_LOG(LogTemp, Warning, TEXT("          Error message size: %d bytes"), MsgSize);
        UE_LOG(LogTemp, Warning, TEXT("               Error message: %s"), *ErrorMsgString);
        return true;
    }
    else if (MessageType == 0xc)
    {
        int32 MsgSize;
        FMemory::Memcpy(&MsgSize, Payload.GetData(), sizeof(int32));
        Payload.RemoveAt(0, sizeof(int32));

        // 这里需要实现gzip解压缩，UE5没有直接的gzip库，可使用第三方库如zlib
        // if (MessageCompression == 1)
        // {
        //     Payload = GzipDecompress(Payload);
        // }

        FString FrontendMessage = FString(UTF8_TO_TCHAR(Payload.GetData()));
        UE_LOG(LogTemp, Warning, TEXT("            Frontend message: %s"), *FrontendMessage);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("undefined message type!"));
        return true;
    }

    return false;
}