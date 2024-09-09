// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "../Character/CPlayerCharacter.h"
#include "CPlayerCameraActor.h"
#include "../Components/CPlayerAttributeComponent.h"
#include "../Global.h"
#include "../Components/CAimingComponent.h"
#include "../Components/CStateComponent.h"
#include "../Components/CActionComponent.h"



ACPlayerController::ACPlayerController()
{
	//Enable Tick
	PrimaryActorTick.bCanEverTick = true;

	//Create Actor Component
	CHelpers::CreateActorComponent(this, &PlayerAttributeComp, "PlayerAttributeComp");


	CurrentLocation = FVector(0, 0, 88.f);
	CameraRotation = GetControlRotation();
	MaxPlayerCharacterCount = 3;

	CHelpers::GetClass(&PlayerCameraActorClass, TEXT("/Game/Player/BP_CPlayerCameraActor"));

}


void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	if (PlayerCameraActor && IsLocalController())
	{
		FTimerHandle TH;
		FTimerDelegate TD;
		TD.BindUFunction(this, "OnSetViewTarget");
		GetWorld()->GetTimerManager().SetTimer(TH, TD, 0.1f, false);
		CLog::Print("SetViewTarget", -1, 10.f, FColor::Blue);
	}
	CLog::Print("BeginController", -1, 10.f, FColor::Red);
}


void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ACPlayerController::OnInputForward);
	InputComponent->BindAxis("MoveRight", this, &ACPlayerController::OnInputRight);
	InputComponent->BindAction("Key_1", IE_Pressed, this, &ACPlayerController::OnInputKey_1);
	InputComponent->BindAction("Key_2", IE_Pressed, this, &ACPlayerController::OnInputKey_2);
	InputComponent->BindAction("Key_3", IE_Pressed, this, &ACPlayerController::OnInputKey_3);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACPlayerController::OnJump);

	InputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ACPlayerController::OnPrimaryAction);
	InputComponent->BindAction("SecondaryAction", IE_Pressed, this, &ACPlayerController::OnSecondaryAction);

	InputComponent->BindAxis("Mouse_X", this, &ACPlayerController::OnMouseX);
	InputComponent->BindAxis("Mouse_Y", this, &ACPlayerController::OnMouseY);
}

void ACPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		FVector Location = PlayerCharacter->GetActorLocation();
		Location.Z += 95.f;
		PlayerCameraActor->SetActorLocation(Location);
		

		CLog::Print(GetControlRotation(), -1, GetWorld()->GetDeltaSeconds(), FColor::Red);
		CLog::Print(PlayerCameraActor->GetActorRotation(), -1, DeltaSeconds);
		CLog::Print(GetNameSafe(PlayerCharacter), -1, DeltaSeconds);
	}

}

void ACPlayerController::SetPlayerCharacterCurrentIndex(int32 InIndex)
{
	PlayerCharacterCurrentIndex = InIndex;
	
}

void ACPlayerController::AddControlledPlayerCharacter(ACPlayerCharacter* InPlayerCharacter)
{
	if (ensure(InPlayerCharacter))
	{
		PlayerCharacters.Add(InPlayerCharacter);
	}
	
}



void ACPlayerController::SpawnPlayerCharacter(FTransform StartTransform)
{
	
	for (int32 i = 0; i < GetMaxPlayerCharacterCount(); i++)
	{
		if (ensure(CharacterClasses[i]))
		{
			TSubclassOf<ACPlayerCharacter> CharacterClass = GetCharacterClasses()[i];
			PlayerCharacter = GetWorld()->SpawnActorDeferred<ACPlayerCharacter>(CharacterClass, StartTransform);
			PlayerCharacter->FinishSpawning(StartTransform);
			PlayerCharacter->SetOnField(false);
			PlayerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerCharacter->GetMesh()->SetVisibility(false);
			AddControlledPlayerCharacter(PlayerCharacter);
		}
	}
}

void ACPlayerController::SpawnCameraActor(FTransform StartTransform)
{
	if (PlayerCameraActorClass)
	{

		PlayerCameraActor = GetWorld()->SpawnActorDeferred<ACPlayerCameraActor>(PlayerCameraActorClass, StartTransform);
		PlayerCameraActor->FinishSpawning(StartTransform);

		SetViewTarget(PlayerCameraActor);
	}
}

void ACPlayerController::PossessCharacter(ACPlayerCharacter* InNewCharacter, EChangeMode InMode)
{
	FRotator Rotation;

	switch (InMode)
	{
	case EChangeMode::None:
		break;
	case EChangeMode::Action:
		break;
	case EChangeMode::Concerto:
		break;
	default:
		break;
	}

	Rotation = GetControlRotation();

	
	ShowCharacter(InNewCharacter);
	
	PlayerCharacter = InNewCharacter;

	if (!HasAuthority())
	{
		ServerPossessCharacter(PlayerCharacter, InMode);
	}
	else
	{
		Possess(PlayerCharacter);
	}

	if (PlayerCameraActor)
	{
		PlayerCameraActor->SetActorRotation(Rotation);
		ControlRotation = Rotation;
	}
	SetViewTarget(PlayerCameraActor);

	
}

void ACPlayerController::ServerPossessCharacter_Implementation(ACPlayerCharacter* InNewCharacter, EChangeMode InMode)
{
	PossessCharacter(InNewCharacter, InMode);
	
}

void ACPlayerController::UnPossessCharacter(EChangeMode InMode)
{
	if (PlayerCharacter)
	{
		switch (InMode)
		{
		case EChangeMode::None:
			PlayerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerCharacter->GetMesh()->SetVisibility(false);
			PlayerCharacter->GetMovementComponent()->StopMovementImmediately();
			break;
		case EChangeMode::Action:
			break;
		case EChangeMode::Concerto:
			break;
		default:
			break;
		}
		
		HideCharacter(PlayerCharacter);
		

		ACPlayerCharacter* PCI = PlayerCharacter;
		for (int32 i = 0; i < MaxPlayerCharacterCount; i++)
		{
			ACPlayerCharacter* PC = PlayerCharacters[i];
			if (PC == PCI) continue;
			if (PC) {
				PC->SetActorLocation(PlayerCharacter->GetActorLocation());
				PC->SetActorRotation(PlayerCharacter->GetActorRotation());
			}
		}

		if (IsLocalController())
		{
			PlayerCharacter->SetCanCharacterChange(false);
			PlayerCharacter->SetCharacterChangeCooldown();
		}

		if (!HasAuthority())
		{
			ServerUnPossessCharacter(InMode);
		}
		else
		{
			UnPossess();
		}
	}
}

void ACPlayerController::ServerUnPossessCharacter_Implementation(EChangeMode InMode)
{
	NetMulticastUnPossessCharacter(InMode);
	UnPossessCharacter(InMode);
}

void ACPlayerController::NetMulticastUnPossessCharacter_Implementation(EChangeMode InMode)
{
	/*for (int32 i = 0; i < MaxPlayerCharacterCount; i++)
	{

		if (i == PlayerCharacterCurrentIndex) continue; 
		ACPlayerCharacter* PC = PlayerCharacters[i];
		if (PC) {
			PC->SetActorLocation(PlayerCharacter->GetActorLocation());
			PC->SetActorRotation(PlayerCharacter->GetActorRotation());
		}
	}*/
}


void ACPlayerController::OnInputKey_R()
{
}

void ACPlayerController::OnInputKey_F()
{
}

void ACPlayerController::OnInputKey_Q()
{
}

void ACPlayerController::OnInputKey_E()
{
}

void ACPlayerController::OnInputForward(float Axis)
{
	if (!PlayerCharacter) return;
	PlayerCharacter->OnMoveForward(Axis);
	
}

void ACPlayerController::OnInputRight(float Axis)
{
	if (!PlayerCharacter) return;
	PlayerCharacter->OnMoveRight(Axis);
}

void ACPlayerController::OnJump()
{
	PlayerCharacter->OnJump();
}

void ACPlayerController::OnInputKey_1()
{
	if (PlayerCharacterCurrentIndex == 0) return;
	ChangePlayerCharacter(0);

}

void ACPlayerController::OnInputKey_2()
{
	if (PlayerCharacterCurrentIndex == 1) return;
	ChangePlayerCharacter(1);
}

void ACPlayerController::OnInputKey_3()
{
	if (PlayerCharacterCurrentIndex == 2) return;
	ChangePlayerCharacter(2);
}

void ACPlayerController::OnInputKey_4()
{
}

void ACPlayerController::OnInputKey_C()
{
}

void ACPlayerController::OnInputKey_Z()
{
}

void ACPlayerController::OnPrimaryAction()
{
}

void ACPlayerController::OnSecondaryAction()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->OnDash();
	}
}

void ACPlayerController::OnInputKey_Shift()
{
}

void ACPlayerController::OnInputKey_ESC()
{
}

void ACPlayerController::OnInputMouse_Left(float Axis)
{
}

void ACPlayerController::OnInputMouse_Right(float Axis)
{
}

void ACPlayerController::OnInputMouse_Wheel(float Axis)
{
}

void ACPlayerController::OnMouseX(float Axis)
{
	if (!PlayerCharacter || !PlayerCameraActor) return;
	PlayerCharacter->OnTurn(Axis);
	PlayerCameraActor->SetActorRotation(GetControlRotation());
}

void ACPlayerController::OnMouseY(float Axis)
{
	if (!PlayerCharacter || !PlayerCameraActor) return;
	PlayerCharacter->OnLookUp(Axis);
	PlayerCameraActor->SetActorRotation(GetControlRotation());

}

void ACPlayerController::ChangePlayerCharacter(uint32 InIndex)
{
	if (!ensure(PlayerCharacter)) return;

	/*if (PlayerCharacter->GetActionComponent()->IsUltimateMode() 
		|| PlayerCharacter->GetActionComponent()->IsStunMode() 
		|| PlayerCharacter->GetActionComponent()->IsDyingMode()) 
		return;*/

	ACPlayerCharacter* NextPlayerCharacter = Cast<ACPlayerCharacter>(PlayerCharacters[InIndex]);

	if (!ensure(NextPlayerCharacter)) return;

	if ((!NextPlayerCharacter->GetCanCharacterChange())) return;
	/*
		|| NextPlayerCharacter->GetActionComponent()->IsDeadMode()) 
		return;


	if (PlayerCharacter->GetActionComponent()->IsActionMode())
	{
		InMode = EChangeMode::Action;
	}*/
	EChangeMode InMode = EChangeMode::None;
	UnPossessCharacter(InMode);
	PlayerCharacterCurrentIndex = InIndex;
	PossessCharacter(NextPlayerCharacter, InMode);

}

void ACPlayerController::HideCharacter(ACPlayerCharacter* HideCharacter)
{
	HideCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HideCharacter->GetMesh()->SetVisibility(false);
	HideCharacter->SetOnField(false);
}

void ACPlayerController::ShowCharacter(ACPlayerCharacter* ShowCharacter)
{
	ShowCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShowCharacter->GetMesh()->SetVisibility(true);
	ShowCharacter->SetOnField(true);
}

void ACPlayerController::OnSetViewTarget()
{
	SetViewTarget(PlayerCameraActor);
}

void ACPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayerController, PlayerCharacter);
	DOREPLIFETIME(ACPlayerController, PlayerCharacters);
	DOREPLIFETIME(ACPlayerController, PlayerCameraActor);
	DOREPLIFETIME(ACPlayerController, PlayerCharacterCurrentIndex);
}
