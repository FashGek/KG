

#include "Kingsguard.h"
#include "KGPickup.h"
#include "KGPickupInventory.h"


AKGPickupInventory::AKGPickupInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FloatHeight = 50.0f;
	bAllowRotatingPickup = true;
}

void AKGPickupInventory::BeginPlay()
{
	AActor::BeginPlay();

	SetInventoryType(InventoryType);

}

#if WITH_EDITOR

void AKGPickupInventory::CreateEditorPickupMesh()
{
	if (GetWorld() != NULL && GetWorld()->WorldType == EWorldType::Editor)
	{
		CreatePickupMesh(this, EditorMesh, InventoryType, FloatHeight, RotationOffset, false);
		if (EditorMesh != NULL)
		{
			EditorMesh->SetHiddenInGame(true);
		}
	}
}

void AKGPickupInventory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CreateEditorPickupMesh();
}

void AKGPickupInventory::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 事实上KG是允许在编辑器里改变类的，因为会往背包里放武器、技能、收集物，只是有的可预览，有的不能
// 	if (PropertyChangedEvent.Property != NULL || PropertyChangedEvent.Property->GetName() == FName(TEXT("InventoryType")))
// 	{
// 		if (InventoryType->IsChildOf(AKGWeapon::StaticClass()))
// 		{
// 			InventoryType = NULL;
// 			FMessageDialog::Open(EAppMsgType::OK, FText::FromString(TEXT("Use KGPickupWeapon for weapon pickups")));
// 		}
// 	}

	CreateEditorPickupMesh();
}

void AKGPickupInventory::PreEditUndo()
{
	UnregisterComponentTree(EditorMesh);
	EditorMesh = NULL;
	Super::PreEditUndo();
}

void AKGPickupInventory::PostEditUndo()
{
	Super::PostEditUndo();
	CreateEditorPickupMesh();
}

#endif


void AKGPickupInventory::SetInventoryType(TSubclassOf<AKGInventory> NewType)
{
	InventoryType = NewType;

	InventoryTypeUpdated();
}

void AKGPickupInventory::InventoryTypeUpdated_Implementation()
{
	CreatePickupMesh(this, Mesh, InventoryType, FloatHeight, RotationOffset, bAllowRotatingPickup);

	TakenSound = (InventoryType != NULL) ? TakenSound = InventoryType.GetDefaultObject()->PickupSound : GetClass()->GetDefaultObject<AKGPickupInventory>()->TakenSound;
}

bool AKGPickupInventory::AllowPickupBy_Implementation(APawn* Other, bool bDefaultAllowPickup)
{

	bDefaultAllowPickup = bDefaultAllowPickup && Cast<AKGCharacter>(Other) != NULL;
	// bool bAllowPickup = bDefaultAllowPickup;

	return bDefaultAllowPickup;
}

void AKGPickupInventory::GiveTo_Implementation(APawn* Target)
{
	AKGCharacter* P = Cast<AKGCharacter>(Target);
	if (P != NULL && InventoryType != NULL)
	{
		AKGInventory* Existing = P->FindInventoryType(InventoryType, true);

		if (Existing == NULL || !Existing->StackPickup(NULL))
		{
			FActorSpawnParameters Params;
			Params.bNoCollisionFail = true;
			Params.Instigator = P;
			P->AddInventory(GetWorld()->SpawnActor<AKGInventory>(InventoryType, GetActorLocation(), GetActorRotation(), Params), true);

		}
	}
}

// NO
void AKGPickupInventory::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

/** recursively instance anything attached to the pickup mesh template */
static void CreatePickupMeshAttachments(AActor* Pickup, USceneComponent* CurrentAttachment, FName TemplateName, const TArray<USceneComponent*>& NativeCompList, const TArray<USCS_Node*>& BPNodes)
{
	for (int32 i = 0; i < NativeCompList.Num(); i++)
	{
		if (NativeCompList[i]->AttachParent == CurrentAttachment)
		{
			USceneComponent* NewComp = ConstructObject<USceneComponent>(NativeCompList[i]->GetClass(), Pickup, NAME_None, RF_NoFlags, NativeCompList[i]);
			NewComp->AttachParent = NULL;
			NewComp->AttachChildren.Empty();
			UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(NewComp);
			if (Prim != NULL)
			{
				Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			NewComp->RegisterComponent();
			NewComp->AttachTo(CurrentAttachment, NewComp->AttachSocketName);
			// recurse
			CreatePickupMeshAttachments(Pickup, NewComp, NativeCompList[i]->GetFName(), NativeCompList, BPNodes);
		}
	}
	for (int32 i = 0; i < BPNodes.Num(); i++)
	{
		if (BPNodes[i]->ComponentTemplate != NULL && BPNodes[i]->ParentComponentOrVariableName == TemplateName)
		{
			USceneComponent* NewComp = ConstructObject<USceneComponent>(BPNodes[i]->ComponentTemplate->GetClass(), Pickup, NAME_None, RF_NoFlags, BPNodes[i]->ComponentTemplate);
			NewComp->AttachParent = NULL;
			NewComp->AttachChildren.Empty();
			UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(NewComp);
			if (Prim != NULL)
			{
				Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			NewComp->RegisterComponent();
			NewComp->AttachTo(CurrentAttachment, BPNodes[i]->AttachToName);
			// recurse
			CreatePickupMeshAttachments(Pickup, NewComp, BPNodes[i]->VariableName, NativeCompList, BPNodes);
		}
	}
}

void AKGPickupInventory::CreatePickupMesh(AActor* Pickup, UMeshComponent*& PickupMesh, TSubclassOf<AKGInventory> PickupInventoryType, float MeshFloatHeight, const FRotator& RotationOffset, bool bAllowRotating)
{
	if (PickupInventoryType == NULL)
	{
		if (PickupMesh != NULL)
		{
			UnregisterComponentTree(PickupMesh);
			PickupMesh = NULL;
		}
	}
	else
	{
		FVector OverrideScale(FVector::ZeroVector);
		UMeshComponent* NewMesh = PickupInventoryType.GetDefaultObject()->GetPickupMeshTemplate(OverrideScale);
		if (NewMesh == NULL)
		{
			if (PickupMesh != NULL)
			{
				UnregisterComponentTree(PickupMesh);
				PickupMesh = NULL;
			}
		}
		else
		{
			USkeletalMeshComponent* SkelTemplate = Cast<USkeletalMeshComponent>(NewMesh);
			UStaticMeshComponent* StaticTemplate = Cast<UStaticMeshComponent>(NewMesh);
			if (PickupMesh == NULL || PickupMesh->GetClass() != NewMesh->GetClass() || PickupMesh->Materials != NewMesh->Materials ||
				(SkelTemplate != NULL && ((USkeletalMeshComponent*)PickupMesh)->SkeletalMesh != SkelTemplate->SkeletalMesh) ||
				(StaticTemplate != NULL && ((UStaticMeshComponent*)PickupMesh)->StaticMesh != StaticTemplate->StaticMesh))
			{
				if (PickupMesh != NULL)
				{
					UnregisterComponentTree(PickupMesh);
					PickupMesh = NULL;
				}
				PickupMesh = ConstructObject<UMeshComponent>(NewMesh->GetClass(), Pickup, NAME_None, RF_NoFlags, NewMesh);
				PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				PickupMesh->AttachParent = NULL;
				PickupMesh->AttachChildren.Empty();
				PickupMesh->RelativeRotation = FRotator::ZeroRotator;
				PickupMesh->RelativeLocation.Z += MeshFloatHeight;
				if (!OverrideScale.IsZero())
				{
					PickupMesh->SetWorldScale3D(OverrideScale);
				}
				if (SkelTemplate != NULL)
				{
					((USkeletalMeshComponent*)PickupMesh)->bForceRefpose = true;
				}
				PickupMesh->RegisterComponent();
				PickupMesh->AttachTo(Pickup->GetRootComponent());
				FVector Offset = Pickup->GetRootComponent()->ComponentToWorld.InverseTransformVectorNoScale(PickupMesh->Bounds.Origin - PickupMesh->GetComponentToWorld().GetLocation());
				PickupMesh->SetRelativeLocation(PickupMesh->GetRelativeTransform().GetLocation() - Offset);
				PickupMesh->SetRelativeRotation(PickupMesh->RelativeRotation + RotationOffset);
				// if there's a rotation component, set it up to rotate the pickup mesh
				if (bAllowRotating && Pickup->GetWorld()->WorldType != EWorldType::Editor) // not if editor preview, because that will cause the preview component to exist in game and we want it to be editor only
				{
					TArray<URotatingMovementComponent*> RotationComps;
					Pickup->GetComponents<URotatingMovementComponent>(RotationComps);
					if (RotationComps.Num() > 0)
					{
						RotationComps[0]->SetUpdatedComponent(PickupMesh);
						RotationComps[0]->PivotTranslation = Offset;
					}
				}

				// see if the pickup mesh has any attached children we should also instance
				TArray<USceneComponent*> NativeCompList;
				PickupInventoryType.GetDefaultObject()->GetComponents<USceneComponent>(NativeCompList);
				TArray<USCS_Node*> ConstructionNodes;
				{
					TArray<const UBlueprintGeneratedClass*> ParentBPClassStack;
					UBlueprintGeneratedClass::GetGeneratedClassesHierarchy(PickupInventoryType, ParentBPClassStack);
					for (int32 i = ParentBPClassStack.Num() - 1; i >= 0; i--)
					{
						const UBlueprintGeneratedClass* CurrentBPGClass = ParentBPClassStack[i];
						if (CurrentBPGClass->SimpleConstructionScript)
						{
							ConstructionNodes += CurrentBPGClass->SimpleConstructionScript->GetAllNodes();
						}
					}
				}
				CreatePickupMeshAttachments(Pickup, PickupMesh, NewMesh->GetFName(), NativeCompList, ConstructionNodes);
			}
			else if (PickupMesh->AttachParent != Pickup->GetRootComponent())
			{
				PickupMesh->AttachTo(Pickup->GetRootComponent(), NAME_None, EAttachLocation::SnapToTarget);
				FVector Offset = Pickup->GetRootComponent()->ComponentToWorld.InverseTransformVectorNoScale(PickupMesh->Bounds.Origin - PickupMesh->GetComponentToWorld().GetLocation());
				PickupMesh->SetRelativeLocation(PickupMesh->GetRelativeTransform().GetLocation() - Offset + FVector(0.0f, 0.0f, MeshFloatHeight));
			}
		}
	}
}
