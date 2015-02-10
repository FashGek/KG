
#pragma once

 
#include "KGPickup.h"
#include "KGInventory.h"

#include "KGPickupInventory.generated.h"

UCLASS(Blueprintable)
class KINGSGUARD_API AKGPickupInventory : public AKGPickup
{
	GENERATED_UCLASS_BODY()

public:
	
	inline const UMeshComponent* GetMesh()
	{
		return Mesh;
	}

	static void CreatePickupMesh(AActor* Pickup, UMeshComponent*& PickupMesh, TSubclassOf<AKGInventory> PickupInvnetoryType, float MeshFloatHeight, const FRotator& RatationOffset, bool bAllowRotation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickupDealy)
	float FloatHeight;
	// 旋转量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickupDealy)
	FRotator RotationOffset;
	// 是否允许旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickupDelay)
	bool bAllowRotatingPickup;

	virtual void BeginPlay() override;

#if WITH_EDITOR

	virtual void CreateEditorPickupMesh();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PreEditUndo() override;
	virtual void PostEditUndo() override;

#endif

	UFUNCTION(BlueprintCallable, Category = Pickup)
	virtual void SetInventoryType(TSubclassOf<AKGInventory> NewType);

	inline TSubclassOf<AKGInventory> GetInventoryType() const
	{
		return InventoryType;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic)
	void InventoryTypeUpdated();

	virtual bool AllowPickupBy_Implementation(APawn* Other, bool bDefaultAllowPickup) override;
	virtual void GiveTo_Implementation(APawn* Target) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, ReplicatedUsing = InventoryTypeUpdated, Category = Inventory)
	TSubclassOf<AKGInventory> InventoryType;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Pickup)
	UMeshComponent* Mesh;

#if WITH_EDITORONLY_DATA

	UPROPERTY(Transient)
	UMeshComponent* EditorMesh;		// 编辑器中预览用

#endif

};
