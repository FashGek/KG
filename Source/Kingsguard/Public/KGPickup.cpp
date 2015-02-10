
#include "Kingsguard.h"

AKGPickup::AKGPickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCanBeDamaged = false;

	Collision = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("Capsule"));
	Collision->SetCollisionProfileName(TEXT("Pickup"));
	Collision->InitCapsuleSize(64.0f, 75.0f);

	if (!IsPendingKill())
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &AKGPickup::OnOverlapBegin);
		Collision->OnComponentEndOverlap.AddDynamic(this, &AKGPickup::OnEndBegin);
	}

	RootComponent = Collision;

	SetReplicates(true);

	PrimaryActorTick.bCanEverTick = true;

	PickupType = PC_Minor;

	PickupMessageString = NSLOCTEXT("PickupMessage", "ItemPickedUp", "Item snagged");
}

void AKGPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKGPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AKGPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

void AKGPickup::OnOverlapBegin(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	APawn* P = Cast<APawn>(OtherActor);
	APlayerController* PC = Cast<APlayerController>(P->GetController());
	if (P != NULL && PC != NULL && !P->bTearOff && !GetWorld()->LineTraceTest(P->GetActorLocation(), GetActorLocation(), ECC_Pawn, FCollisionQueryParams(), WorldResponseParams))
	{
		EnableInput(PC);
	}
}

void AKGPickup::ProcessTouch_Implementation(APawn* TouchedBy)
{
	// TODO
	if (Role == ROLE_Authority && TouchedBy->Controller != NULL && AllowPickupBy(TouchedBy, true))
	{
		GiveTo(TouchedBy);
	}
}

bool AKGPickup::AllowPickupBy_Implementation(APawn* Other, bool bDefaultAllowPickup)
{
	bool bAllowPickup = bDefaultAllowPickup;
	AKGGameMode* KGGameMode = GetWorld()->GetAuthGameMode<AKGGameMode>();
	return true;
}

void AKGPickup::GiveTo_Implementation(APawn* Target)
{

}

void AKGPickup::OnEndBegin(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* P = Cast<APawn>(OtherActor);
	APlayerController* PC = Cast<APlayerController>(P->GetController());
	if (P != NULL && P != NULL && !P->bTearOff && !GetWorld()->LineTraceTest(P->GetActorLocation(), GetActorLocation(), ECC_Pawn, FCollisionQueryParams(), WorldResponseParams))
	{
		DisableInput(PC);
	}
}

void AKGPickup::HandleKeyDown(APawn* Target)
{
	if (Target != NULL)
	{
		ProcessTouch(Target);
	}
}
