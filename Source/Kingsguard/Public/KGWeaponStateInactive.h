

#pragma once

//#include "KGWeaponStateInactive.genetated.h"
#include "KGWeaponStateInactive.generated.h"


UCLASS(CustomConstructor)
class UKGWeaponStateInactive : public UKGWeaponState
{
	GENERATED_UCLASS_BODY()

	UKGWeaponStateInactive(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual void BringUp(float OverflowTime)
	{
		// GetOuterAKGWeapon()->GotoEquippingState(OverflowTime);
	}
};
