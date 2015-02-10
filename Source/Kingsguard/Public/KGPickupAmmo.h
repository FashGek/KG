
#pragma once

#include "KGCharacter.h"
#include "KGPickupAmmo.generated.h"

UCLASS(Blueprintable)
class KINGSGUARD_API AKGPickupAmmo : public AKGPickup
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, AssetRegistrySearchable, Category = Pickup)
	FStoredAmmo Ammo;

	virtual bool AllowPickupBy_Implementation(APawn* Other, bool bDefaultAllowPickup) override;
	virtual void GiveTo_Implementation(APawn* Target) override;
};