// Fill out your copyright notice in the Description page of Project Settings.


#include "CLobbyCharacter.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "Global.h"

// Sets default values
ACLobbyCharacter::ACLobbyCharacter()
{
	CHelpers::CreateSceneComponent<UTextRenderComponent>(this, &UserNameComp, "UserNameComp", RootComponent);
	UserNameComp->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	UserNameComp->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	UserNameComp->SetIsReplicated(true);
	UserNameComp->SetText("");
}

// Called when the game starts or when spawned
void ACLobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACLobbyCharacter::SetUserName(FText InName)
{
	UserName = InName;
	Rep_UserName();
}

void ACLobbyCharacter::Rep_UserName()
{
	UserNameComp->SetText(UserName);
}

void ACLobbyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACLobbyCharacter, UserName);
}
