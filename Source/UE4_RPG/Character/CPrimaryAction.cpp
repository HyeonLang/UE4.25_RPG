
#include "CPrimaryAction.h"

ACPrimaryAction::ACPrimaryAction()
{


}

void ACPrimaryAction::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPrimaryAction::EnableCombo()
{
	bCanCombo = true;
}

void ACPrimaryAction::DisableCombo()
{
	bCanCombo = false;
}


