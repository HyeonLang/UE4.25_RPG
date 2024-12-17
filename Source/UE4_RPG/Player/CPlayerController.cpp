// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Character/CPlayerCharacter.h"
#include "CPlayerCameraActor.h"
#include "CMinimapCameraActor.h"
#include "../Components/CPlayerAttributeComponent.h"
#include "../Global.h"
#include "../Components/CAimingComponent.h"
#include "../Components/CStateComponent.h"
#include "../Components/CActionComponent.h"
#include "Actions/CAction.h"



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
	}

	/*if (ensure(MinimapCameraActorClass) && IsLocalController())
	{
		MinimapCameraActor = GetWorld()->SpawnActorDeferred<ACMinimapCameraActor>(MinimapCameraActorClass, FTransform(), this);
		MinimapCameraActor->FinishSpawning(FTransform());
	}*/
}


void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ACPlayerController::OnInputForward);
	InputComponent->BindAxis("MoveRight", this, &ACPlayerController::OnInputRight);

	InputComponent->BindAction("Key_1", IE_Pressed, this, &ACPlayerController::OnInputKey_1);
	InputComponent->BindAction("Key_2", IE_Pressed, this, &ACPlayerController::OnInputKey_2);
	InputComponent->BindAction("Key_3", IE_Pressed, this, &ACPlayerController::OnInputKey_3);

	InputComponent->BindAction("SpaceBar", IE_Pressed, this, &ACPlayerController::OnSpaceBar);

	InputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ACPlayerController::OnPrimaryAction);
	InputComponent->BindAction("SecondaryAction", IE_Pressed, this, &ACPlayerController::OnSecondaryAction);

	InputComponent->BindAction("Key_E", IE_Pressed, this, &ACPlayerController::OnInputKey_E);
	InputComponent->BindAction("Key_R", IE_Pressed, this, &ACPlayerController::OnInputKey_R);


	InputComponent->BindAxis("Mouse_X", this, &ACPlayerController::OnMouseX);
	InputComponent->BindAxis("Mouse_Y", this, &ACPlayerController::OnMouseY);

	InputComponent->BindAction("SecondaryAction", IE_Pressed, this, &ACPlayerController::OnStartSprint);
	InputComponent->BindAction("SecondaryAction", IE_Released, this, &ACPlayerController::OnStopSprint);
}

void ACPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayerCharacter)
	{
		FVector Location;

		Location = PlayerCharacter->GetActorLocation();
		Location.Z += 95.f;
		
		if (PlayerCharacter->HasAuthority() && PlayerCameraActor)
		{
			PlayerCameraActor->SetActorLocation(Location);
		}

		/*if (MinimapCameraActor)
		{
			MinimapCameraActor->SetActorLocation(Location);
			CLog::Print(MinimapCameraActor->GetActorLocation(), -1, DeltaSeconds);
		}*/
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
			//PlayerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName("BehindPlayerCharacter");
			PlayerCharacter->GetMesh()->SetVisibility(false);
			PlayerCharacter->SetOwnerController(this);
			AddControlledPlayerCharacter(PlayerCharacter);
		}
	}
}

void ACPlayerController::SpawnCameraActor(FTransform StartTransform)
{
	if (ensure(PlayerCameraActorClass))
	{

		PlayerCameraActor = GetWorld()->SpawnActorDeferred<ACPlayerCameraActor>(PlayerCameraActorClass, StartTransform);
		PlayerCameraActor->FinishSpawning(StartTransform);
		SetViewTarget(PlayerCameraActor);
	}
	
}

void ACPlayerController::PossessCharacter(ACPlayerCharacter* InNewCharacter, FVector CurrentVelocity, EChangeMode InMode)
{
	FVector Location;
	FRotator Rotation = GetControlRotation();

	if (IsLocalController())
	{
		UCAction* RunningAction = InNewCharacter->GetActionComponent()->ActiveMontageAction;
		if (RunningAction && RunningAction->IsRunning())
		{
			InNewCharacter->GetActionComponent()->StopActionByName(InNewCharacter, RunningAction->ActionName);
			InNewCharacter->ServerStopAnimMontage();
		}
	}
	
	if (GetChangeCharacterLocation(Location, PlayerCharacter, PlayerCharacter->GetAimingComponent()->TargetActor, InMode))
	{
		InNewCharacter->SetActorLocation(Location);
	}

	switch (InMode)
	{
	case EChangeMode::None:
		InNewCharacter->GetRootComponent()->ComponentVelocity = CurrentVelocity;
		//InNewCharacter->GetCharacterMovement()->Velocity = CurrentVelocity;
		break;
	case EChangeMode::Action:
		break;
	case EChangeMode::Concerto:
		break;
	default:
		break;
	}


	ShowCharacter(InNewCharacter);
	PlayerCharacter = InNewCharacter;


	if (!HasAuthority())
	{
		ServerPossessCharacter(InNewCharacter, CurrentVelocity, InMode);
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

void ACPlayerController::ServerPossessCharacter_Implementation(ACPlayerCharacter* InNewCharacter, FVector CurrentVelocity, EChangeMode InMode)
{
	PossessCharacter(InNewCharacter, CurrentVelocity, InMode);
	
}

void ACPlayerController::UnPossessCharacter(FVector& OutVelocity, EChangeMode InMode)
{
	if (PlayerCharacter)
	{
		OutVelocity = PlayerCharacter->GetCharacterMovement()->Velocity;
		PlayerCharacter->GetMovementComponent()->StopMovementImmediately();
		
		switch (InMode)
		{
		case EChangeMode::None:
			HideCharacter(PlayerCharacter);
			break;
		case EChangeMode::Action:
			break;
		case EChangeMode::Concerto:
			PlayerCharacter->GetActionComponent()->OnActionStopped.AddDynamic(this, &ACPlayerController::OnActionStopped_HideCharacter);
			break;
		default:
			break;
		}

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
			ServerUnPossessCharacter(OutVelocity, InMode);
		}
		else
		{
			UnPossess();
		}
	}
}

void ACPlayerController::ServerUnPossessCharacter_Implementation(FVector OutVelocity, EChangeMode InMode)
{
	NetMulticastUnPossessCharacter(InMode);
	UnPossessCharacter(OutVelocity, InMode);
}

void ACPlayerController::NetMulticastUnPossessCharacter_Implementation(EChangeMode InMode)
{
}


bool ACPlayerController::GetChangeCharacterLocation(FVector& OutLocation, AActor* PlayerActor, AActor* TargetActor, EChangeMode InMode) const
{

	float Distance = 150.f;

	if (TargetActor || InMode == EChangeMode::Concerto)
	{
		FVector ChangeVector = PlayerCameraActor->GetActorRightVector().GetSafeNormal();
		OutLocation = (ChangeVector * Distance) + PlayerActor->GetActorLocation();
		CLog::Print(GetNameSafe(TargetActor));
		return true;
	}

	return false;
}

void ACPlayerController::OnInputKey_R()
{
	if (!PlayerCharacter) return;
	PlayerCharacter->StartResonanceLiberation();
}

void ACPlayerController::OnInputKey_F()
{
}

void ACPlayerController::OnInputKey_Q()
{
}

void ACPlayerController::OnInputKey_E()
{
	if (!PlayerCharacter) return;
	PlayerCharacter->StartResonanceSkill();
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

void ACPlayerController::OnSpaceBar()
{
	if (!PlayerCharacter) return;
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
	if (PlayerCharacter)
	{
		PlayerCharacter->StartNormalAttack();
	}
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

void ACPlayerController::OnStartSprint()
{
	PlayerCharacter->StartSprint();
}

void ACPlayerController::OnStopSprint()
{
	PlayerCharacter->StopSprint();
}

void ACPlayerController::OnActionCreateFinished(UCActionComponent* OwningComp)
{
	BindActionWidgets(OwningComp);
}

void ACPlayerController::BindActionWidgets_Implementation(UCActionComponent* OwningComp)
{
}

void ACPlayerController::ChangePlayerCharacter(uint32 InIndex)
{
	if ((int32)InIndex >= MaxPlayerCharacterCount) return;
	if (!ensure(PlayerCharacter)) return;

	ACPlayerCharacter* NextPlayerCharacter = Cast<ACPlayerCharacter>(PlayerCharacters[InIndex]);

	if (!ensure(NextPlayerCharacter)) return;

	if ((!NextPlayerCharacter->GetCanCharacterChange())) return;
	
	if (PlayerCharacter->GetActionComponent()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag("Action.ResonanceLiberation"))) return;

	EChangeMode InMode = EChangeMode::None;

	FGameplayTagContainer ActionTags;
	ActionTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Attack"));
	ActionTags.AddTag(FGameplayTag::RequestGameplayTag("Action.ResonanceSkill"));
	if (PlayerCharacter->GetActionComponent()->ActiveGameplayTags.HasAny(ActionTags))
	{
		InMode = EChangeMode::Concerto;
	}

	FVector Velocity;
	UnPossessCharacter(Velocity, InMode);
	PlayerCharacterCurrentIndex = InIndex;
	PossessCharacter(NextPlayerCharacter, Velocity, InMode);

}

void ACPlayerController::HideCharacter(ACPlayerCharacter* HideCharacter)
{
	//HideCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HideCharacter->GetCapsuleComponent()->SetCollisionProfileName("BehindPlayerCharacter");
	HideCharacter->SetAllVisibility(false);
	HideCharacter->SetOnField(false);
}

void ACPlayerController::OnActionStopped_HideCharacter(UCActionComponent* OwningComp, UCAction* Action)
{
	ACPlayerCharacter* PC = Cast<ACPlayerCharacter>(OwningComp->GetOwner());

	if (PC) HideCharacter(PC);

	OwningComp->OnActionStopped.RemoveDynamic(this, &ACPlayerController::OnActionStopped_HideCharacter);
}

void ACPlayerController::ShowCharacter(ACPlayerCharacter* ShowCharacter)
{
	//ShowCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShowCharacter->GetCapsuleComponent()->SetCollisionProfileName("PlayerCharacter");
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
	//DOREPLIFETIME(ACPlayerController, MinimapCameraActor);
	DOREPLIFETIME(ACPlayerController, PlayerCharacterCurrentIndex);
}
