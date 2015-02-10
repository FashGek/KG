// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kingsguard.h"
#include "KGInventory.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract, /*notplaceable,*/ meta = (ChildCanTick))
class KINGSGUARD_API AKGInventory : public AActor
{
	GENERATED_UCLASS_BODY()

	friend void AKGCharacter::AddInventory(AKGInventory* InvToAdd, bool bAutoActivate);
	friend void AKGCharacter::RemoveInventory(AKGInventory* InvRemove);
	template<typename> friend class TInventoryIterator;


	virtual void PostInitializeComponents() override;
	virtual void PreInitializeComponents() override;
	
protected:
	
	// 其实我是倾向于用数组的，而不是用链表，无所谓
	UPROPERTY(BlueprintReadOnly, Replicated, Category = Inventory)
	AKGInventory* NextInventory;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	AKGCharacter* KGOwner;

	/* 给指定玩家物品时调用 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void eventGivenTo(AKGCharacter* NewOwner, bool bAutoActivate);
	virtual void GivenTo(AKGCharacter* NewOwner, bool bAutoActivate);
	/* 删除指定玩家物品时调用 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void eventRemoved();
	virtual void Removed();
	/** 客户端处理，拥有者转移 */
	UFUNCTION(Client, Reliable)
	void ClientGivenTo(APawn* NewInstigator, bool bAutoActive);
	virtual void ClientGivenTo_Internal(bool bAutoActive);
	/** 客户端调用, 给东西 */
	UFUNCTION(BlueprintImplementableEvent)
	void eventClientGivenTo(bool bAutoActive);

	void CheckPendingClientGivenTo();

	uint32 bPendingClientGivenTo : 1;
	uint32 bPendingAutoActivate : 1;

	UPROPERTY(EditDefaultsOnly, Category = Pickup)
	UMeshComponent* PickupMesh;

public:

	AKGInventory* GetNext() const
	{
		return NextInventory;
	}

	AKGCharacter* GetKGOwner() const
	{
		return KGOwner;
	}
	virtual void DropFrom(const FVector2D& StartLocation, const FVector& TossVelocity);
	virtual void Destroyed() override;

	// virtual void InitializeDroppedPickup(class );
	UFUNCTION(BlueprintNativeEvent)
	UMeshComponent* GetPickupMeshTemplate(FVector& OverrideScale) const;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FText DisplayName;

	/** 拾起时的声音 */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
	USoundBase* PickupSound;

	/** 造成伤害时的声音 */
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	USoundBase* PainSound;

	/* 是否允许堆叠背包格子 */
	UFUNCTION(BlueprintNativeEvent)
	bool StackPickup(AKGInventory* ContainedInv);

public:

	virtual bool HUDShouldRender(UKGHUDWidget* TargetWidget);
};

/** 背包链表迭代器 */

template<typename InvType = AKGInventory>
class KINGSGUARD_API TInventoryIterator
{

public:

	TInventoryIterator(const AKGCharacter* InvHolder) : Holder(InvHolder), Count(0)
	{
		if (Holder != NULL)
		{
			CurrentInv = Holder->InventoryList;
			if (CurrentInv != NULL)
			{
				NextInv = CurrentInv->NextInventory;
				if (!IsValidForIterator(CurrentInv))
				{
					++(*this);
				}
			}
			else
			{
				NextInv = NULL;
			}
		}
		else 
		{
			CurrentInv = NULL;
			NextInv = NULL;
		}
	}

	void operator++()
	{
		do 
		{
			// 最高检查30次，防止无限循环
			Count++;
			if (Count > 30)
			{
				CurrentInv = NULL;
			}

			CurrentInv = NextInv;
			if (CurrentInv != NULL)
			{
				NextInv = CurrentInv->NextInventory;
			}
		} while (CurrentInv != NULL && !IsValidForIterator(CurrentInv));

	}

	FORCEINLINE bool IsValid() const
	{
		return CurrentInv != NULL;
	}

	FORCEINLINE operator bool() const
	{
		return IsValid();
	}

	FORCEINLINE InvType* operator*() const
	{
		checkSlow(CurrentInv != NULL && CurrentInv->IsA(InvType::StaticClass()));
		return (InvType*)CurrentInv;
	}

	FORCEINLINE InvType* operator->() const
	{
		checkSlow(CurrentInv != NULL && CurrentInv->IsA(InvType::StaticClass()));
		return (InvType*)CurrentInv;
	}

private:

	inline bool IsValidForIterator(AKGInventory* TestInv)
	{
		// 确保是背包的主人在迭代背包，且检查东西是一致的
		return (TestInv->GetOwner() == Holder && TestInv->GetKGOwner() == Holder && TestInv->IsA(InvType::StaticClass()));
	}

private:

	const AKGCharacter* Holder;
	AKGInventory* CurrentInv;
	AKGInventory* NextInv;
	int32 Count;
};
