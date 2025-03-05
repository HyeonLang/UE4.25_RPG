// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "CDBManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHttpResponseReceived, const FString&, ResponseContent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHttpRequestFailed, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginSuccess, const FString&, UserData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginFailed, const FString&, ErrorMessage);

/**
 * HTTP를 통한 웹 서버 통신을 관리하는 매니저 클래스
 */
UCLASS()
class UE4_RPG_API UCDBManager : public UObject
{
	GENERATED_BODY()
public:
	UCDBManager();


public:
	// 사용자 데이터 전송
	UFUNCTION(BlueprintCallable, Exec, Category = "HTTP")
	void SendData(const FText& InUserName, const FText& InPhone1, const FText& InPhone2, const FText& InPhone3, const FText& InAge, const FText& InAddress);

	// GET 요청 전송
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SendGetRequest(const FString& Endpoint);

	// POST 요청 전송
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SendPostRequest(const FString& Endpoint, const FString& JsonString);

	// 로그인 요청
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void RequestLogin(const FString& Username, const FString& Password);

	// 로그인 요청 상태 확인
	UFUNCTION(BlueprintPure, Category = "HTTP")
	bool IsLoginInProgress() const { return bLoginInProgress; }

	// 기본 URL 설정
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SetBaseUrl(const FString& NewBaseUrl) { BaseURL = NewBaseUrl; }

private:
	// HTTP 응답 처리
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// 로그인 응답 처리
	void OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// 쿼리 파라미터 문자열 생성
	FString CreateQueryString(const TMap<FString, FString>& Params);

protected:
	// 기본 URL
	UPROPERTY()
	FString BaseURL;

	// 현재 URL
	UPROPERTY()
	FString URL;

	// 로그인 요청 진행 중 여부
	UPROPERTY()
	bool bLoginInProgress;

public:
	// 응답 수신 이벤트
	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FOnHttpResponseReceived OnResponseReceivedEvent;

	// 요청 실패 이벤트
	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FOnHttpRequestFailed OnRequestFailedEvent;

	// 로그인 성공 이벤트
	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FOnLoginSuccess OnLoginSuccessEvent;

	// 로그인 실패 이벤트
	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FOnLoginFailed OnLoginFailedEvent;
};
