// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <mysqlx/xdevapi.h>
#include "CDatabaseComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = "Custom", meta = (BlueprintSpawnableComponent))
class UE4_RPG_API UCDatabaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCDatabaseComponent();
	virtual ~UCDatabaseComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ConnectToDatabase(const FString& Host, const int32 Port, const FString& Username, const FString& Password, const FString& Schema);

	UFUNCTION(BlueprintCallable, Category = "Database")
	void DisconnectFromDatabase();

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool InsertUserData(const FString& TableName, const FString& Username, const FString& Password);

	UFUNCTION(BlueprintPure, Category = "Database")
	bool IsConnected() const { return bIsConnected; }

private:
	mysqlx::Session* CreateSession(const FString& ConnectionString, bool& OutSuccess);

private:
	mysqlx::Session* Session;
	mysqlx::Schema* SchemaDB;
	bool bIsConnected;

	// 연결 정보 저장
	FString CurrentHost;
	int32 CurrentPort;
	FString CurrentSchema;
};
