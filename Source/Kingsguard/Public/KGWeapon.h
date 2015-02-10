

#pragma once

#include "KGInventory.h"
#include "KGWeapon.generated.h"

UCLASS(Blueprintable, Abstract/*, NotPlaceable*/, Config = Game)
class KINGSGUARD_API AKGWeapon : public AKGInventory
{
	GENERATED_UCLASS_BODY()

	friend class UKGWeaponState;
	friend class UKGWeaponStateInactive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, /*Replicated, ReplicatedUsing = OnRep_Ammo, */Category = "Weapon")
	int32 Ammo;		// �����ӵ�������Ϊ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaxAmmo;

	// each firemode needs different ammo cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<int32> AmmoCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.1"))
	TArray<float> FireInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<float> Spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<USoundBase*> FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<USoundBase*> FireLoopingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<UAnimMontage*> FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<UParticleSystem*> FireEffect;

	/** ��, ����û�м���ʱ������ǹ������ʾ��������ֻ��һ������������Ƥ������ʾ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	bool bMustBeHolstered;

	/** ��, �������Ա����� */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bCanThrowWeapon;

	virtual void DropFrom(const FVector2D& StartLocation, const FVector& TossVelocity) override;

	/** ����ģ�� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* Mesh;

	USkeletalMeshComponent* GetMesh() const { return Mesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector FireOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BringUpTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float PutDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* BringUpAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* PutDownAnim;

	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;

	virtual void RegisterAllComponents() override
	{
		if (GetWorld()->WorldType == EWorldType::Editor || GetWorld()->WorldType == EWorldType::Preview)
		{
			Super::RegisterAllComponents();
		}
		else
		{
			RootComponent = NULL;	// ? this was set for the editor view, but we don't want it
		}
	}

	virtual UMeshComponent* GetPickupMeshTemplate_Implementation(FVector& OverrideScale) const;

	virtual void GotoState(class UKGWeaponState* NewState);

	virtual void StateChanged() {}

	/** firing entry pont */
	virtual void StartFire(uint8 FireModeNum);
	virtual void StopFire(uint8 FireModeNum);

	/** net mode */
	// TODO
// 	UFUNCTION(Server, Reliable, WithValidation)
// 	virtual void ServerStartFire(uint8 FireModeNum, bool bClientFired);
// 
// 	UFUNCTION(Server, Reliable, WithValidation)
// 	virtual void ServerStopFire(uint8 FireModeNum);

	virtual bool BeginFiringSequence(uint8 FireModeNum, bool bClientFired);
	virtual bool EndFiringSequence(uint8 FireModeNum);

	/** return true if it can fire again */
	virtual bool CanFireAgain();

	/**��ʼ����Ĺ���*/
	UFUNCTION(BlueprintNativeEvent)
	void OnStartedFiring();

	/** hook for the return to active state (was firing, refire timer expired, trigger relese and/or out of ammo)*/
	UFUNCTION(BlueprintNativeEvent)
	void OnStoppedFiring();
	
	/**�Ƿ�������� */
	virtual bool HandleContinuedFiring();

	/** hook when weapon fired, the refire delay pass, and the user still wants to fire (trigger still down) so the firing loop will repeat */
	UFUNCTION(BlueprintNativeEvent)
	void OnContinuedFiring();

	/**
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMultiPress(uint8 OtherFireMode);

	/** �л������м������һ����
	    �����Ѿ�������Pawn����
	 *@param: OverflowTime ǰһ������PutDown��tick delta
	 */
	virtual void BringUp(float OverflowTime = 0.0f);

	/** �����л���һ���� 
	 * @retur: false ��ֹ�����л� */
	virtual bool PutDown();

	/** ����̧��ʱ����������һЩ��Ч */
	UFUNCTION(BlueprintImplementableEvent)
	void OnBringUp();

	/** ����BP��ֹ�����п� */
	UFUNCTION(BlueprintImplementableEvent)
	bool eventPreventPutDown();

	/** �������ŵ�ӵ����ģ���� */
	UFUNCTION(BlueprintNativeEvent)
	void AttachToOwner();

	virtual void AttachToOwnerNative();
	
	/** ������ģ����ж������ */
	UFUNCTION(BlueprintNativeEvent)
	void DetachFromOwner();

	virtual void DetachFromOwnerNative();
	
	
	virtual void GivenTo(AKGCharacter* NewOwner, bool bAutoActivate) override;
	virtual void ClientGivenTo_Internal(bool bAutoActivate) override;
	
	virtual void Removed() override;

	/** �������ӵ�ҩ������������ҩ */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Weapon")
	virtual void AddAmmo(int32 Amount);

	/** ���ڸ����Ŀ���ģʽ����鵯ҩ�Ƿ��������ĵĵ�ҩ */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool HasAmmo(uint8 FireModeNum);

	/* �����Ƿ��е�ҩ���κο���ģʽ */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool HasAnyAmmo();


	inline uint8 GetCurrentFireMode()
	{
		return CurrentFireMode;
	}

	inline void GoToActiveState()
	{
		GotoState(ActiveState);
	}

	virtual void Destroyed() override;

	virtual bool StackPickup_Implementation(AKGInventory* ContainedInv);

	virtual void Tick(float DeltaSeconds) override;

	inline UKGWeaponState* GetCurrentState()
	{
		return CurrentState;
	}



protected:

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UKGWeaponState* CurrentState;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	uint8 CurrentFireMode;

	UPROPERTY(Instanced, BlueprintReadOnly, Category = "State")
	UKGWeaponState* ActiveState;

	UPROPERTY(Instanced, BlueprintReadOnly, Category = "State")
	UKGWeaponState* InactiveState;

};

