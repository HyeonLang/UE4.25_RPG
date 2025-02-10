#include "CDBManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Global.h"


UCDBManager::UCDBManager()
{
}

void UCDBManager::SendData(const FText& InUserName, const FText& InPhone1, const FText& InPhone2, const FText& InPhone3, const FText& InAge, const FText& InAddress)
{
    //Http모듈. 멤버 변수로 획득 금지
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UCDBManager::OnResponseReceived);

    //JSON Elenemt Array(요소 전달)
    TSharedRef<FJsonObject> JsonFieldObject = MakeShared<FJsonObject>();

    const FText& nameText = InUserName;
    URL.Append("user_name=");
    URL.Append(FString::Printf(TEXT("%s"), *FGenericPlatformHttp::UrlEncode(nameText.ToString())));
    //URL.Append("%EA%B9%80%EA%B2%BD%EC%84%9D");

    const FText& phone1Text = InPhone1;
    URL.Append("&user_phone1=");
    URL.Append(FString::Printf(TEXT("%s"), *phone1Text.ToString()));

    const FText& phone2Text = InPhone2;
    URL.Append("&user_phone2=");
    URL.Append(FString::Printf(TEXT("%s"), *phone2Text.ToString()));

    const FText& phone3Text = InPhone3;
    URL.Append("&user_phone3=");
    URL.Append(FString::Printf(TEXT("%s"), *phone3Text.ToString()));

    //나이+주소 -> user_contents,
    if (InAge.IsEmpty() == false || InAddress.IsEmpty() == false)
    {
        const FText& ageText = InAge;
        const FText& addressText = InAddress;
        FString userContentsStr = FString::Printf(TEXT("%s/%s"), *ageText.ToString(), *FGenericPlatformHttp::UrlEncode(addressText.ToString()));

        URL.Append("&user_contents=");
        URL.Append(FString::Printf(TEXT("%s"), *userContentsStr));
    }
    UE_LOG(LogTemp, Error, TEXT("%s"), *URL);


    //Request->SetURL(FGenericPlatformHttp::UrlEncode(URL));

    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "text/html");
    bool bSuceess = Request->ProcessRequest();
    UE_LOG(LogTemp, Error, TEXT("Request result is %d"), bSuceess);

}

void UCDBManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}
