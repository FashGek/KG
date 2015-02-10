

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
	int32 Ammo;		// 刀的子弹可以认为无限

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

	/** 真, 武器没有激活时，可在枪套中显示，背包里只有一种武器可以在皮套里显示 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	bool bMustBeHolstered;

	/** 真, 武器可以被丢掉 */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bCanThrowWeapon;

	virtual void DropFrom(const FVector2D& StartLocation, const FVector& TossVelocity) override;

	/** 武器模型 */
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

	/**开始射击的勾子*/
	UFUNCTION(BlueprintNativeEvent)
	void OnStartedFiring();

	/** hook for the return to active state (was firing, refire timer expired, trigger relese and/or out of ammo)*/
	UFUNCTION(BlueprintNativeEvent)
	void OnStoppedFiring();
	
	/**是否连续射击 */
	virtual bool HandleContinuedFiring();

	/** hook when weapon fired, the refire delay pass, and the user still wants to fire (trigger still down) so the firing loop will repeat */
	UFUNCTION(BlueprintNativeEvent)
	void OnContinuedFiring();

	/**
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMultiPress(uint8 OtherFireMode);

	/** 切换武器中激活的那一部分
	    武器已经放在了Pawn身上
	 *@param: OverflowTime 前一个武器PutDown的tick delta
	 */
	virtual void BringUp(float OverflowTime = 0.0f);

	/** 武器切换的一部分 
	 * @retur: false 阻止武器切换 */
	virtual bool PutDown();

	/** 武器抬上时，允许钩子做一些特效 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnBringUp();

	/** 允许BP阻止武器切开 */
	UFUNCTION(BlueprintImplementableEvent)
	bool eventPreventPutDown();

	/** 把武器放到拥有者模型上 */
	UFUNCTION(BlueprintNativeEvent)
	void AttachToOwner();

	virtual void AttachToOwnerNative();
	
	/** 从人物模型上卸下武器 */
	UFUNCTION(BlueprintNativeEvent)
	void DetachFromOwner();

	virtual void DetachFromOwnerNative();
	
	
	virtual void GivenTo(AKGCharacter* NewOwner, bool bAutoActivate) override;
	virtual void ClientGivenTo_Internal(bool bAutoActivate) override;
	
	virtual void Removed() override;

	/** 正数，加弹药；负数，减弹药 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Weapon")
	virtual void AddAmmo(int32 Amount);

	/** 对于给定的开火模式，检查弹药是否满足消耗的弹药 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool HasAmmo(uint8 FireModeNum);

	/* 返回是否还有弹药，任何开火模式 */
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

