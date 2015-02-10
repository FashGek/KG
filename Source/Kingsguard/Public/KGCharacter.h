// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KGCharacterMovement.h"
#include "KGCharacter.generated.h"


// forward declaration
class AKGInventory;
class AKGWeapon;


/* Ammo counter */
USTRUCT(BlueprintType)
struct FStoredAmmo 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ammo)
	TSubclassOf<class AKGWeapon> Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ammo)
	int32 Amount;
};

UCLASS(config = Game, collapsecategories, hidecategories = (Clothing, Lighting, AutoExposure, LensFlares, AmbientOcclusion, DepthOfField, MotionBlur, Misc, ScreenSpaceReflections, Bloom, SceneColor, Film, AmbientCubemap, AgentPhysics, Attachment, Avoidance, PlanarMovement, AI, Replication, Input, Actor, Tags, GlobalIllumination))
class KINGSGUARD_API AKGCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()


	friend void UKGCharacterMovement::PerformMovement(float DeltaSeconds);


	//====================
	/** 玩家模型 只看到两只手 */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FirstPersonMesh;
	
	/** 第一人称摄像机-眼睛 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CharacterCameraComponent;

	/**  */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	class UKGCharacterMovement* KGCharacterMovement;

	/** 生命 */
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
	int32 Health;
	/** 生命上限 */
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
	int32 HealthMax;
	/** 魔法，其实是收集的魂的数量,也就是不会有补魂的道具 */
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
	int32 Mana;
	/** 魔法上限 */
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
	int32 ManaMax;

	UPROPERTY()
	int32 EmoteCount;

	/* 玩家检起了子弹，但是还没有对应的武器 */
	UPROPERTY()
	TArray<FStoredAmmo> SavedAmmo;

	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void AddAmmo(const FStoredAmmo& AmmoToAdd);

	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual bool HasMaxAmmo(TSubclassOf<AKGWeapon> Type) const;

	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual int32 GetAmmoAmount(TSubclassOf<AKGWeapon> Type) const;

	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void AllAmmo();

	// 背包迭代器
	template<typename> friend class TInventoryIterator;

	// 背包处理
	inline AKGInventory* GetInventory()
	{
		return InventoryList;
	}

	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void AddInventory(AKGInventory* InvToAdd, bool bAutoActivate);

	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void RemoveInventory(AKGInventory* InvRemove);

	/* 从背包里找出具体类型的背包 */
	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual AKGInventory* K2_FindInventoryType(TSubclassOf<AKGInventory> Type, bool bExactClass = false) const;

	template<typename InvClass>
	inline InvClass* FindInventoryType(TSubclassOf<InvClass> Type, bool bExactClass = false) const
	{
		InvClass* Result = (InvClass*)K2_FindInventoryType(Type, bExactClass);
		checkSlow(Result == NULL || Result->IsA(InvClass::StaticClass()));
		return Result;
	}

	/* 切换武器 */
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void SwitchWeapon(AKGWeapon* NewWeapon);

	inline bool IsPendingFire(uint8 FireMode) const
	{
		// 玩家能开火且FireMode有效
		return !IsFiringDisabled() && (FireMode < PendingFire.Num() && PendingFire[FireMode] != 0);
	}

	inline void SetPendingFire(uint8 FireMode, bool bNowFiring)
	{
		// 数组大小不够，扩大并初始为0
		if (PendingFire.Num() < FireMode + 1)
		{
			PendingFire.SetNumZeroed(FireMode + 1);
		}
		PendingFire[FireMode] = bNowFiring ? 1 : 0;
	}

	inline void ClearPendingFire()
	{
		for (int32 i = 0; i < PendingFire.Num(); i++)
		{
			PendingFire[i] = 0;
		}
	}

	inline AKGWeapon* GetWeapon() const
	{
		return Weapon;
	}

	inline AKGWeapon* GetPendingWeapon() const
	{
		return PendingWeapon;
	}

	bool IsInInventory(const AKGInventory* TestInv) const;
	/* 当装备取下，并拉低时，把pendingweapon转为weapon,并拉上*/
	virtual void WeaponChanged(float OverflowTime = 0.0f);

	UFUNCTION()
	virtual void UpdateWeaponAttachment();

public:

	UPROPERTY()
	float DefautBaseEyeHeight;

public:

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;


	virtual void Restart() override;

	virtual void AddDefaultInventory(TArray<TSubclassOf<AKGInventory> > DefaultInventoryToAdd);

	UFUNCTION(BlueprintCallable, Category = Pawn)
	bool IsDead();

	/* 武器开火 */
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void StartFire(uint8 FireModeNum);

	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void StopFire(uint8 FireModeNum);

	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void StopFiring();

	virtual void PawnStartFire(uint8 FireModeNum = 0) override
	{
		StartFire(FireModeNum);
	}

	UFUNCTION(Exec, BlueprintCallable, Category = Pawn)
	virtual void FeignDeath();

// 	UFUNCTION(Reliable, Server, WithValidation, Category = Pawn)
// 		void ServerFeignDeath();

	UFUNCTION()
	virtual void PlayFeignDeath();

protected:
	UPROPERTY(BlueprintReadOnly, Category = Pawn)
	bool bDisallowWeaponFiring;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = PlayFeignDeath, Category = Pawn)
	bool bFeigningDeath;


public:
// 	UFUNCTION(BlueprintCallable, Category = Pawn)
// 	virtual void DisallowWeaponFiring(bool bDisallowed);

	inline bool IsFiringDisabled() const
	{
		return bDisallowWeaponFiring;
	}

	/** 向前/后移动 */
	virtual void MoveForward(float Val);
	/** 向左/右移动 */
	virtual void MoveRight(float Val);

public:
	UPROPERTY(EditAnywhere, Category = "Pawn")
	TArray<TSubclassOf<AKGInventory> > DefaultCharacterInventory;

protected:

	/* 客户端切换武器，与服务器独立执行 */
	virtual void LocalSwitchWeapon(AKGWeapon* NewWeapon);

	UFUNCTION(Client, Reliable)
	virtual void ClientSwitchWeapon(AKGWeapon* NewWeapon);

	// firemodes with input currently being held down (pending or actually firing), 1，开火；0，没有开火
	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
	TArray<uint8> PendingFire;

	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
	AKGInventory* InventoryList;	// 背包

	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
	AKGWeapon* PendingWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
	class AKGWeapon* Weapon;

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = UpdateWeaponAttachment, Category = "Pawn")
	TSubclassOf<AKGWeapon> WeaponClass;


// 	class AKGWeaponAttachment* WeaponAttachment;
// 
// 	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = UpdateWeaponAttachment, Category = "Pawn")
// 	TSubclassOf<AKGWeaponAttachment> WeaponAttachmentClass;

};

inline bool AKGCharacter::IsDead()
{
	return bTearOff || bPendingKillPending;
}
