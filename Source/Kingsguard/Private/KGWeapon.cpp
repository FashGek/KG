


#include "Kingsguard.h"
#include "KGWeaponState.h"		// 这样才不会报编辑错误
#include "KGWeaponStateActive.h"
#include "KGWeaponStateInactive.h"

AKGWeapon::AKGWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(TEXT("PickupMesh0")))
{
	AmmoCost.Add(1);
	AmmoCost.Add(1);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	bCanThrowWeapon = true;

	Ammo = 20;
	MaxAmmo = 50;


	BringUpTime = 0.41f;
	PutDownTime = 0.3f;

	InactiveState = ObjectInitializer.CreateDefaultSubobject<UKGWeaponStateInactive>(this, TEXT("StateInactive"));
	ActiveState = ObjectInitializer.CreateDefaultSubobject<UKGWeaponStateActive>(this, TEXT("StateActive"));


	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Mesh1P"));
	Mesh->SetOnlyOwnerSee(true);
	Mesh->AttachParent = RootComponent;
	Mesh->bSelfShadowOnly = true;
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;

}

void AKGWeapon::DropFrom(const FVector2D& StartLocation, const FVector& TossVelocity)
{
	if (KGOwner && bMustBeHolstered)
	{
/*		DetachFromHolster(UTOwner->GetMesh()); */
	}

	if (!HasAnyAmmo())
	{
		Destroyed();
	}
	else
	{
		Super::DropFrom(StartLocation, TossVelocity);
	}
}

void AKGWeapon::StartFire(uint8 FireModeNum)
{
	if (!KGOwner->IsFiringDisabled())
	{
		bool bClientFired = BeginFiringSequence(FireModeNum, false);
		if (Role < ROLE_Authority)
		{
/*			ServerStartFire();*/
		}
	}
}

void AKGWeapon::StopFire(uint8 FireModeNum)
{
	EndFiringSequence(FireModeNum);
	if (Role < ROLE_Authority)
	{
		/*ServerStopFire(FireModeNum);*/
	}
}

bool AKGWeapon::EndFiringSequence(uint8 FireModeNum)
{
	if (KGOwner)
	{
		KGOwner->SetPendingFire(FireModeNum, false);
	}

// 	if ()
// 	{
// 	}
	return false;
}

bool AKGWeapon::BeginFiringSequence(uint8 FireModeNum, bool bClientFired)
{
	if (KGOwner)
	{
	}

	return false;
}

bool AKGWeapon::CanFireAgain()
{
	return (GetKGOwner() && (GetKGOwner()->GetPendingWeapon() == NULL) && (GetKGOwner()->IsPendingFire(GetCurrentFireMode()) && HasAmmo(GetCurrentFireMode())));
}

// 子类BP/Subclass勾子，默认为空实现
void AKGWeapon::OnStartedFiring_Implementation()
{}
void AKGWeapon::OnStoppedFiring_Implementation()
{}
void AKGWeapon::OnContinuedFiring_Implementation() 
{}
void AKGWeapon::OnMultiPress_Implementation(uint8 OtherFireMode)
{}

void AKGWeapon::BringUp(float OverflowTime /*= 0.0f*/)
{
	AttachToOwner();
	OnBringUp();	// BP
	CurrentState->BringUp(OverflowTime);
}

bool AKGWeapon::PutDown()
{
	if (eventPreventPutDown())
	{
		return false;
	}
	else
	{
		CurrentState->PutDown();
		return true;
	}
}

void AKGWeapon::AttachToOwner_Implementation()
{
	AttachToOwnerNative();
}

void AKGWeapon::AttachToOwnerNative()
{
	if (KGOwner == NULL)
	{
		return;
	}

	if (bMustBeHolstered)
	{

	}

	// santity check some settings
	
	// attach
	if (Mesh != NULL && Mesh->SkeletalMesh != NULL)
	{
		Mesh->AttachTo(KGOwner->FirstPersonMesh);
		if (Cast<APlayerController>(KGOwner->Controller) != NULL && KGOwner->IsLocallyControlled())
		{
			Mesh->LastRenderTime = GetWorld()->TimeSeconds;
			Mesh->bRecentlyRendered = true;
		}
	}

	// register all components now
	Super::RegisterAllComponents();
// 	if (GetNetMode() != NM_DedicatedServer)
// 	{
// 		UpdateOverlaps();
// 		//setskin
// 	}
}

void AKGWeapon::DetachFromOwner_Implementation()
{
	DetachFromOwnerNative();
}

void AKGWeapon::DetachFromOwnerNative()
{

}

void AKGWeapon::AddAmmo(int32 Amount)
{
	if (Role == ROLE_Authority)
	{
		Ammo = FMath::Clamp<int32>(Ammo + Amount, 0, MaxAmmo);

		// trigger weapon switch if necessary
		if (KGOwner != NULL && KGOwner->IsLocallyControlled())
		{
			// TODO OnRepo_Ammo();
		}
	}
}

bool AKGWeapon::HasAmmo(uint8 FireModeNum)
{
	return (AmmoCost.IsValidIndex(FireModeNum) && Ammo >= AmmoCost[FireModeNum]);
}

bool AKGWeapon::HasAnyAmmo()
{
	bool bHadCost = false;

	// 假设1， 大多数武器其实都只消耗一发子弹
	// 假设2, 对于不需要消耗子弹的开火模式，can't function effectively without the other one
	for (int32 i = GetCurrentFireMode() - 1; i >= 0; i--)
	{
		if (AmmoCost[i] > 0)
		{
			bHadCost = true;
			if (HasAmmo(i))
			{
				return true;
			}
		}
	}
	return !bHadCost;
}

bool AKGWeapon::HandleContinuedFiring()
{
	if (!CanFireAgain())
	{
		GoToActiveState();
		return false;
	}

	OnContinuedFiring();
	return true;
}

void AKGWeapon::Destroyed()
{
	Super::Destroyed();

	// GoToState(InactiveState);
}

void AKGWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AKGWeapon::GotoState(class UKGWeaponState* NewState)
{
	if (NewState == NULL || !NewState->IsIn(this))
	{
		UE_LOG(KG, Warning, TEXT("Attempt to send %s to invalid state %s"), *GetName(), *GetFullNameSafe(NewState));
	}
	else if (ensureMsgf(KGOwner != NULL || NewState == InactiveState, TEXT("Attemp to send %s to state %s while not owned"), *GetName(), *GetNameSafe(NewState)))
	{
		if (CurrentState != NewState)
		{
			UKGWeaponState* PrevState = CurrentState;
			if (CurrentState != NULL)
			{
				CurrentState->EndState();
			}
			
			if (CurrentState == PrevState)	
			{
				CurrentState = NewState;
				CurrentState->BeginState(PrevState);
				StateChanged();
			}
		}
	}


}

void AKGWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentState == NULL)
	{
		GotoState(InactiveState);
	}
	checkSlow(CurrentState != NULL);
}

void AKGWeapon::PostInitProperties()
{
	Super::PostInitProperties();


	if (DisplayName.IsEmpty() || (GetClass() != AKGWeapon::StaticClass() && DisplayName.EqualTo(GetClass()->GetSuperClass()->GetDefaultObject<AKGWeapon>()->DisplayName)))
	{
		DisplayName = FText::FromName(GetClass()->GetFName());
	}
}


void AKGCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}


void AKGWeapon::GivenTo(AKGCharacter* NewOwner, bool bAutoActivate)
{
	Super::GivenTo(NewOwner, bAutoActivate);
	// 对应武器玩家有弹药，就上
	/* 
	for (int32 i = 0; i < NewOwner->SavedAmmo.Num(); i++)
	{
		if (NewOwner->SavedAmmo[i].Type == GetClass(); i++)
		{
			AddAmmo(NewOwner->SavedAmmo[i].Ammount);
			NewOwner->SavedAmmo.RemoveAt(i);
			break;
		}
	}*/
}

void AKGWeapon::Removed()
{
	GotoState(InactiveState);
	DetachFromOwner();
	
	Super::Removed();
}

void AKGWeapon::ClientGivenTo_Internal(bool bAutoActivate)
{

	if (bMustBeHolstered && KGOwner && HasAnyAmmo())
	{

	}

	if (CurrentState == NULL)
	{
		GotoState(InactiveState);
	}

	Super::ClientGivenTo_Internal(bAutoActivate);

	// 切换优先级
	AKGPlayerController *KGPC = Cast<AKGPlayerController>(KGOwner->Controller);
	if (KGPC != NULL)
	{
		// AutoSwitchPriority = KGPC->GetWeaponAutoSwitchPriority(GetNameSafe(this), AutoSwitchPriority);
	}

	// 分配GroupSlot，如有需要


	if (bAutoActivate && KGPC != NULL)
	{
		KGPC->CheckAutoWeaponSwitch(this);
	}
}

bool AKGWeapon::StackPickup_Implementation(AKGInventory* ContainedInv)
{
	// AddAmmo(ContainedInv != NULL ? Cast<AKGWeapon>(ContainedInv))
	// 把武器中的弹药加到玩家身上
	return true;
}

UMeshComponent* AKGWeapon::GetPickupMeshTemplate_Implementation(FVector& OverrideScale) const
{
	return Super::GetPickupMeshTemplate_Implementation(OverrideScale);
}
