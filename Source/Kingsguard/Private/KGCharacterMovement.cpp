// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGCharacterMovement.h"

UKGCharacterMovement::UKGCharacterMovement(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UKGCharacterMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	AKGCharacter* KGOwner = Cast<AKGCharacter>(CharacterOwner);

	if (KGOwner != NULL) 
	{
		UMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

		const FVector InputVector = ConsumeInputVector();
		if (!HasValidData() || ShouldSkipUpdate(DeltaTime) || UpdatedComponent->IsSimulatingPhysics())
		{
			return;
		}



		if (CharacterOwner->IsLocallyControlled() || (!CharacterOwner->Controller && CharacterOwner->IsPlayingRootMotion()))
		{
			FNetworkPredictionData_Client_Character* ClientData = ((CharacterOwner->Role < ROLE_Authority) && (GetNetMode() == NM_Client)) ? GetPredictionData_Client_Character() : NULL;
			if (ClientData)
			{
				// Update our delta time for physics simulation
				// DeltaTime = UpdateTimeStampAndDeltaTime(DeltaTime, ClientData);
				// CurrentServerMoveTime = 
			}

			CharacterOwner->CheckJumpInput(DeltaTime);

			Acceleration = ScaleInputAcceleration(ConstrainInputAcceleration(InputVector));
			AnalogInputModifier = ComputeAnalogInputModifier();


			if (CharacterOwner->Role == ROLE_Authority) 
			{
				PerformMovement(DeltaTime);
			}
		}
	}

	// º∆À„ÀŸ∂»
	// AvgSpeed = 
	if (CharacterOwner != NULL)
	{
		AKGPlayerController* PC = Cast<AKGPlayerController>(CharacterOwner->Controller);
// 		if (PC != NULL && PC->PlayerInput != NULL)
// 		{
// 			PC->ApplyDeferredFireInputs();
// 		}

	}

}

bool UKGCharacterMovement::Is3DMovementMode() const
{
	return (MovementMode == MOVE_Flying) || (MovementMode == MOVE_Swimming);
}

void UKGCharacterMovement::PerformMovement(float DeltaTime)
{
	if (!CharacterOwner)
	{
		return;
	}

	Super::PerformMovement(DeltaTime);
}
