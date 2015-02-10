
#pragma once

#include "KGWeaponStateActive.generated.h"

UCLASS(CustomConstructor)
class UKGWeaponStateActive : public UKGWeaponState
{
	GENERATED_UCLASS_BODY()

	UKGWeaponStateActive(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual void BeginState(const UKGWeaponState* PrevState) override;
	virtual bool BeginFiringSequence(uint8 FireModeNum, bool bClientFired) override;

};