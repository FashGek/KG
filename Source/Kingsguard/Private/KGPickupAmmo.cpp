
#include "Kingsguard.h"
#include "KGPickup.h"
#include "KGPickupAmmo.h"

AKGPickupAmmo::AKGPickupAmmo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Ammo.Amount = 10;
	PickupMessageString = NSLOCTEXT("PickupMessage", "AmmoPickedUp", "Ammo");
}

bool AKGPickupAmmo::AllowPickupBy_Implementation(APawn* Other, bool bDefaultAllowPickup)
{

	return Super::AllowPickupBy_Implementation(Other, bDefaultAllowPickup && Cast<AKGCharacter>(Other) != NULL && !((AKGCharacter*)Other)->HasMaxAmmo(Ammo.Type));
}

void AKGPickupAmmo::GiveTo_Implementation(APawn* Target)
{
	AKGCharacter* P = Cast<AKGCharacter>(Target);

	if (P != NULL)
	{
		P->AddAmmo(Ammo);
		AKGPickup::GiveTo_Implementation(Target);
	}
}
