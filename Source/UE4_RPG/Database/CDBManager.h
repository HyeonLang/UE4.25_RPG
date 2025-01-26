// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <xdevapi.h>
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
	~UCDBManager();

public:
	UFUNCTION(BlueprintCallable, Category = "DB")
	bool Connect(const FString& Host, int32 Port, const FString& UserName, const FString& Password, const FString& Schema);
	UFUNCTION(BlueprintCallable, Category = "DB")
	bool InsertUserTable(const FString& InTableName, const FString& InUserName, const FString& InPassword);
	UFUNCTION(BlueprintCallable, Category = "DB")
	bool Disconnect();

public:
	//UPROPERTY()
	mysqlx::Session* m_Session;

	//UPROPERTY()
	mysqlx::Schema* m_SchemaDB;

};
