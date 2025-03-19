#include "CLog.h"

DEFINE_LOG_CATEGORY_STATIC(GameProject, Display, All)

void CLog::Print(int32 InValue, int32 InKey, float InDuration, FColor InColor)
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::FromInt(InValue));
#endif
}

void CLog::Print(float InValue, int32 InKey, float InDuration, FColor InColor)
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::SanitizeFloat(InValue));
#endif
}

void CLog::Print(const FString& InValue, int32 InKey, float InDuration, FColor InColor)
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue);
#endif
}

void CLog::Print(const FVector& InValue, int32 InKey, float InDuration, FColor InColor)
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
#endif
}

void CLog::Print(const FRotator& InValue, int32 InKey, float InDuration, FColor InColor)
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
#endif
}

void CLog::Log(int32 InValue)
{
	UE_LOG(GameProject, Display, TEXT("%d"), InValue);
}

void CLog::Log(float InValue)
{
	UE_LOG(GameProject, Display, TEXT("%f"), InValue);
}

void CLog::Log(const FString& InValue)
{
	UE_LOG(GameProject, Display, TEXT("%s"), *InValue);
}

void CLog::Log(const FVector& InValue)
{
	UE_LOG(GameProject, Display, TEXT("%s"), *InValue.ToString());
}

void CLog::Log(const FRotator& InValue)
{
	UE_LOG(GameProject, Display, TEXT("%s"), *InValue.ToString());
}

void CLog::Log(const UObject* InObject)
{
	FString str;
	if (InObject)
	{
		str.Append(InObject->GetName());
	}

	str.Append(InObject ? " is not null" : "NULL");

	UE_LOG(GameProject, Display, TEXT("%s"), *str);
}

void CLog::Log(const FString& InFuncName, int32 InLineNumber)
{
	FString str;
	str.Append(InFuncName);
	str.Append(", ");
	str.Append(FString::FromInt(InLineNumber));

	UE_LOG(GameProject, Display, TEXT("%s"), *str);
}

void CLog::LogOnScreen(UObject* WorldContext, FString Msg, FColor Color, float Duration)
{
#if WITH_EDITOR
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString Prefix = World->IsNetMode(NM_Client) ? "[Client] " : "[Server] ";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Prefix + Msg);
	}
#endif
}