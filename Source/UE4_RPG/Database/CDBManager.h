// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "CDBManager.generated.h"

/**
 * 
 */
UCLASS()
class UE4_RPG_API UCDBManager : public UObject
{
	GENERATED_BODY()
public:
	UCDBManager();


public:
	UFUNCTION(BlueprintCallable, Exec, Category = "HTTP")
	void SendData(const FText& InUserName, const FText& InPhone1, const FText& InPhone2, const FText& InPhone3, const FText& InAge, const FText& InAddress);


private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);


protected:
	UPROPERTY()
	FString URL;

};
