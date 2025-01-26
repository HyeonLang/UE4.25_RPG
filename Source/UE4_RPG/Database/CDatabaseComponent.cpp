// Fill out your copyright notice in the Description page of Project Settings.


#include "Database/CDatabaseComponent.h"
#include <xdevapi.h>
// Sets default values for this component's properties
UCDatabaseComponent::UCDatabaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	Session = nullptr;
	bIsConnected = false;
}


UCDatabaseComponent::~UCDatabaseComponent()
{
	DisconnectFromDatabase();
}

// Called when the game starts
void UCDatabaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCDatabaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	DisconnectFromDatabase();
}

// Called every frame
void UCDatabaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCDatabaseComponent::ConnectToDatabase(const FString& Host, const int32 Port, const FString& Username, const FString& Password, const FString& Schema)
{
	if (bIsConnected)
	{
		DisconnectFromDatabase();
	}

	const int32 MaxRetries = 3;
	int32 RetryCount = 0;

	while (RetryCount < MaxRetries)
	{
		UE_LOG(LogTemp, Log, TEXT("Attempt %d - Connecting to database..."), RetryCount + 1);

		if (Session != nullptr)
		{
			delete Session;
			Session = nullptr;
		}

#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
			FString::Printf(TEXT("Attempting to connect to: %s:%d"), *Host, Port));
#endif

		/*std::string ConnectionString = std::string("mysqlx://") + 
			TCHAR_TO_UTF8(*Username) + ":" + 
			TCHAR_TO_UTF8(*Password) + "@" + 
			TCHAR_TO_UTF8(*Host) + ":" + 
			std::to_string(Port) + "/" + 
			TCHAR_TO_UTF8(*Schema);

		Session = new mysqlx::Session(ConnectionString);*/
		std::string strHost = TCHAR_TO_UTF8(*Host);
		std::string strUserName = TCHAR_TO_UTF8(*Username);
		std::string strPassword = TCHAR_TO_UTF8(*Password);
		std::string strSchema = TCHAR_TO_UTF8(*Schema);

		Session = new mysqlx::Session(strHost, Port, strUserName, strPassword);
		if (Session != nullptr)
		{
			SchemaDB = new mysqlx::Schema(Session->getSchema(strSchema));
			bIsConnected = true;
			CurrentHost = Host;
			CurrentPort = Port;
			CurrentSchema = Schema;

			UE_LOG(LogTemp, Log, TEXT("Successfully connected to database: %s on attempt %d"), 
				*Host, RetryCount + 1);
			return true;
		}

		UE_LOG(LogTemp, Warning, TEXT("Failed to connect on attempt %d"), RetryCount + 1);
		FPlatformProcess::Sleep(1.0f);
		RetryCount++;
	}

	bIsConnected = false;
	UE_LOG(LogTemp, Error, TEXT("Failed to connect to database after %d attempts"), MaxRetries);
	return false;
}

void UCDatabaseComponent::DisconnectFromDatabase()
{
	if (Session)
	{
		delete Session;
		Session = nullptr;
		bIsConnected = false;
		UE_LOG(LogTemp, Log, TEXT("Disconnected from database"));
	}
}

bool UCDatabaseComponent::InsertUserData(const FString& TableName, const FString& Username, const FString& Password)
{
	if (nullptr == Session)
	{
		return false;
	}

	try
	{
		mysqlx::Table Table = SchemaDB->getTable(TCHAR_TO_UTF8(*TableName));
		Table.insert("username", "password").values(TCHAR_TO_UTF8(*Username), TCHAR_TO_UTF8(*Password)).execute();


		return true;
	}
	catch (const mysqlx::Error& err)
	{
		UE_LOG(LogTemp, Error, TEXT("Insert Error : %s"), *FString(err.what()));
		return false;
	}
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("Unkonow error : "));
		return false;
	}

	return false;
}

