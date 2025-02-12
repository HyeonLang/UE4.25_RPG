#include "CDBManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Global.h"

UCDBManager::UCDBManager()
{
}

void UCDBManager::SendData(const FText& InUserName, const FText& InPhone1, const FText& InPhone2, const FText& InPhone3, const FText& InAge, const FText& InAddress)
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UCDBManager::OnResponseReceived);

    URL.Empty();
    URL.Append("user_name=");
    URL.Append(FString::Printf(TEXT("%s"), *FGenericPlatformHttp::UrlEncode(InUserName.ToString())));

    URL.Append("&user_phone1=");
    URL.Append(FString::Printf(TEXT("%s"), *InPhone1.ToString()));

    URL.Append("&user_phone2=");
    URL.Append(FString::Printf(TEXT("%s"), *InPhone2.ToString()));

    URL.Append("&user_phone3=");
    URL.Append(FString::Printf(TEXT("%s"), *InPhone3.ToString()));

    if (!InAge.IsEmpty() || !InAddress.IsEmpty())
    {
        FString userContents = FString::Printf(TEXT("%s/%s"), 
            *InAge.ToString(), 
            *FGenericPlatformHttp::UrlEncode(InAddress.ToString()));

        URL.Append("&user_contents=");
        URL.Append(FString::Printf(TEXT("%s"), *userContents));
    }

    Request->SetURL(URL);
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("text/html"));

    if (!Request->ProcessRequest())
    {
        OnRequestFailedEvent.Broadcast(TEXT("Failed to send request"));
    }
}

void UCDBManager::SendGetRequest(const FString& Endpoint)
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UCDBManager::OnResponseReceived);

    Request->SetURL(Endpoint);
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    if (!Request->ProcessRequest())
    {
        OnRequestFailedEvent.Broadcast(TEXT("Failed to send GET request"));
    }
}

void UCDBManager::SendPostRequest(const FString& Endpoint, const FString& JsonString)
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UCDBManager::OnResponseReceived);

    Request->SetURL(Endpoint);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(JsonString);

    if (!Request->ProcessRequest())
    {
        OnRequestFailedEvent.Broadcast(TEXT("Failed to send POST request"));
    }
}

void UCDBManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!bConnectedSuccessfully || !Response.IsValid())
    {
        OnRequestFailedEvent.Broadcast(TEXT("Connection failed or invalid response"));
        return;
    }

    if (Response->GetResponseCode() >= 400)
    {
        OnRequestFailedEvent.Broadcast(FString::Printf(TEXT("Server error: %d"), Response->GetResponseCode()));
        return;
    }

    FString ResponseString = Response->GetContentAsString();
    OnResponseReceivedEvent.Broadcast(ResponseString);

    // JSON 응답 파싱 예시
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        // JSON 파싱 성공
        UE_LOG(LogTemp, Log, TEXT("Response parsed successfully: %s"), *ResponseString);
    }
}

FString UCDBManager::CreateQueryString(const TMap<FString, FString>& Params)
{
    FString QueryString;
    bool bFirst = true;

    for (const auto& Param : Params)
    {
        if (!bFirst)
        {
            QueryString += TEXT("&");
        }
        QueryString += FString::Printf(TEXT("%s=%s"), 
            *FGenericPlatformHttp::UrlEncode(Param.Key), 
            *FGenericPlatformHttp::UrlEncode(Param.Value));
        bFirst = false;
    }

    return QueryString;
}

void UCDBManager::RequestLogin(const FString& Username, const FString& Password)
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UCDBManager::OnLoginResponse);

    // JSON 데이터 생성
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField(TEXT("username"), Username);
    JsonObject->SetStringField(TEXT("password"), Password);

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    // POST 요청 설정
    Request->SetURL(TEXT("http://your-server.com/api/login"));  // 서버의 로그인 엔드포인트로 변경 필요
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(JsonString);

    if (!Request->ProcessRequest())
    {
        OnLoginFailedEvent.Broadcast(TEXT("Failed to send login request"));
    }
}

void UCDBManager::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!bConnectedSuccessfully || !Response.IsValid())
    {
        OnLoginFailedEvent.Broadcast(TEXT("Connection failed or invalid response"));
        return;
    }

    if (Response->GetResponseCode() >= 400)
    {
        OnLoginFailedEvent.Broadcast(FString::Printf(TEXT("Login failed: %d"), Response->GetResponseCode()));
        return;
    }

    FString ResponseString = Response->GetContentAsString();
    
    // JSON 응답 파싱
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
    
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // 로그인 성공 시 사용자 데이터 전달
        OnLoginSuccessEvent.Broadcast(ResponseString);
        UE_LOG(LogTemp, Log, TEXT("Login successful: %s"), *ResponseString);
    }
    else
    {
        OnLoginFailedEvent.Broadcast(TEXT("Failed to parse login response"));
    }
}
