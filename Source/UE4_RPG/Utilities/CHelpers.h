#pragma once

#include "CoreMinimal.h"

#define CheckNull(p) { if (p == nullptr) return; }
#define CheckNullResult(p, result) { if (p == nullptr) return result; }

#define CheckTrue(p) { if (p == true) return; }
#define CheckTrueResult(p, result) { if (p == true) return result; }

#define CheckFalse(p) { if (p == false) return; }
#define CheckFalseResult(p, result) { if (p == false) return result; }


class UE4_RPG_API CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutAsset, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> Asset(*InPath);
		ensureMsgf(Asset.Succeeded(), TEXT("Asset Not Found"));

		*OutAsset = Asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutAsset, FString InPath)
	{
		T* Object = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
		ensureMsgf(Object, TEXT("Asset Not Found(Dynamic)"));

		*OutAsset = Object;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> Class(*InPath);
		ensureMsgf(Class.Succeeded(), TEXT("Class Not Found"));

		*OutClass = Class.Class;
	}

	template<typename T>
	static void GetClassDynamic(TSubclassOf<T>* OutClass, FString InPath)
	{
		
		UClass* LoadedClass = StaticLoadClass(T::StaticClass(), nullptr, *InPath);

		if (!LoadedClass)
		{
			ensureMsgf(false, TEXT("Failed to load class from path: %s"), *InPath);
			return;
		}

		*OutClass = Cast<UClass>(LoadedClass);

		if (!*OutClass)
		{
			ensureMsgf(false, TEXT("Failed to cast the loaded class to TSubclassOf<T>"));
		}
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static void CreateSceneComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
		
		if (InParent)
		{
			(*OutComponent)->SetupAttachment(InParent);
			return;
		}
		
		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static T* ConvertSoftToHardObject(TSoftObjectPtr<T> SoftTexturePtr)
	{
		T* Texture = nullptr;
		// UTexture2D 로드
		if (SoftTexturePtr.IsValid()) // 이미 로드된 경우
		{
			Texture = SoftTexturePtr.Get();
			
		}
		else // 아직 로드되지 않은 경우
		{
			Texture = SoftTexturePtr.LoadSynchronous();
		}

		return Texture;
	}
};