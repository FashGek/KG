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
	
	// ��ʵ����������������ģ�����������������ν
	UPROPERTY(BlueprintReadOnly, Replicated, Category = Inventory)
	AKGInventory* NextInventory;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	AKGCharacter* KGOwner;

	/* ��ָ�������Ʒʱ���� */
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void eventGivenTo(AKGCharacter* NewOwner, bool bAutoActivate);
	virtual void GivenTo(AKGCharacter* NewOwner, bool bAutoActivate);
	/* ɾ��ָ�������Ʒʱ���� */
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void eventRemoved();
	virtual void Removed();
	/** �ͻ��˴���ӵ����ת�� */
	UFUNCTION(Client, Reliable)
	void ClientGivenTo(APawn* NewInstigator, bool bAutoActive);
	virtual void ClientGivenTo_Internal(bool bAutoActive);
	/** �ͻ��˵���, ������ */
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

	/** ʰ��ʱ������ */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
	USoundBase* PickupSound;

	/** ����˺�ʱ������ */
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	USoundBase* PainSound;

	/* �Ƿ�����ѵ��������� */
	UFUNCTION(BlueprintNativeEvent)
	bool StackPickup(AKGInventory* ContainedInv);

public:

	virtual bool HUDShouldRender(UKGHUDWidget* TargetWidget);
};

/** ������������� */

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
			// ��߼��30�Σ���ֹ����ѭ��
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
		// ȷ���Ǳ����������ڵ����������Ҽ�鶫����һ�µ�
		return (TestInv->GetOwner() == Holder && TestInv->GetKGOwner() == Holder && TestInv->IsA(InvType::StaticClass()));
	}

private:

	const AKGCharacter* Holder;
	AKGInventory* CurrentInv;
	AKGInventory* NextInv;
	int32 Count;
};
