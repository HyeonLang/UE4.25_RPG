
#include "CPlayerState.h"
#include "Net/UnrealNetwork.h"

#include "Player/CInventory.h"

ACPlayerState::ACPlayerState()
{

}

void ACPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		PlayerInventory = NewObject<UCInventory>(this);
	}
}





void ACPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayerState, PlayerInventory);
}