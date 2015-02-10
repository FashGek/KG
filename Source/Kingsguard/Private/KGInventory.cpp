// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGInventory.h"



AKGInventory::AKGInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetReplicates(true);
	bOnlyRelevantToOwner = true;
	bReplicateInstigator = true;

	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent, USceneComponent>(this, TEXT("DummyRoot"), false);

}

void AKGInventory::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKGInventory::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

bool AKGInventory::HUDShouldRender(UKGHUDWidget* TargetWidget)
{
	return false;
}

// 这里很可能将会导致内存泄露问题
void AKGInventory::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AKGInventory::Destroyed()
{
	if (KGOwner != NULL)
	{
		KGOwner->RemoveInventory(this);
	}

	Super::Destroyed();
}

void AKGInventory::DropFrom(const FVector2D& StartLocation, const FVector& TossVelocity)
{

}

void AKGInventory::GivenTo(AKGCharacter* NewOwner, bool bAutoActivate)
{
	Instigator = NewOwner;
	SetOwner(NewOwner);
	KGOwner = NewOwner;
	PrimaryActorTick.AddPrerequisite(KGOwner, KGOwner->PrimaryActorTick);
	eventGivenTo(NewOwner, bAutoActivate);
	ClientGivenTo(Instigator, bAutoActivate);
}

void AKGInventory::Removed()
{
	eventRemoved();

	if (KGOwner != NULL)
	{
		PrimaryActorTick.RemovePrerequisite(KGOwner, KGOwner->PrimaryActorTick);
	}

	Instigator = NULL;
	SetOwner(NULL);
	KGOwner = NULL;
	NextInventory = NULL;
}

void AKGInventory::ClientGivenTo_Implementation(APawn* NewInstigator, bool bAutoActivate)
{
	if (NewInstigator != nullptr)
	{
		Instigator = NewInstigator;
	}

	if (Instigator == NULL || !Cast<AKGCharacter>(Instigator)->IsInInventory(this))
	{
		bPendingClientGivenTo = true;
		bPendingAutoActivate = bAutoActivate;
		GetWorld()->GetTimerManager().SetTimer(this, &AKGInventory::CheckPendingClientGivenTo, 0.1f, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(this, &AKGInventory::CheckPendingClientGivenTo);
		bPendingClientGivenTo = false;
		ClientGivenTo_Internal(bAutoActivate);
		eventClientGivenTo(bAutoActivate);
	}
}

void AKGInventory::CheckPendingClientGivenTo()
{
	if (bPendingClientGivenTo && Instigator != NULL)
	{
		bPendingClientGivenTo = false;
		ClientGivenTo_Implementation(nullptr, bPendingAutoActivate); // 这里是有清除Timer的作用
	}
}

void AKGInventory::ClientGivenTo_Internal(bool bAutoActive)
{
	checkSlow(Instigator != NULL);
	SetOwner(Instigator);
	KGOwner = Cast<AKGCharacter>(Instigator);
	checkSlow(KGOwner != NULL);
	PrimaryActorTick.AddPrerequisite(KGOwner, KGOwner->PrimaryActorTick);
}

bool AKGInventory::StackPickup_Implementation(AKGInventory* ContainedInv)
{
	return false;
}

UMeshComponent* AKGInventory::GetPickupMeshTemplate_Implementation(FVector& OverrideScale) const
{
	return PickupMesh;
}
