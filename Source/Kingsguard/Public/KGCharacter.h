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
	/** ���ģ�� ֻ������ֻ�� */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FirstPersonMesh;
	
	/** ��һ�˳������-�۾� */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CharacterCameraComponent;

	/**  */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	class UKGCharacterMovement* KGCharacterMovement;

	/** ���� */
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
	int32 Health;
	/** �������� */
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
	int32 HealthMax;
	/** ħ������ʵ���ռ��Ļ������,Ҳ���ǲ����в���ĵ��� */
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
	int32 Mana;
	/** ħ������ */
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
	int32 ManaMax;

	UPROPERTY()
	int32 EmoteCount;

	/* ��Ҽ������ӵ������ǻ�û�ж�Ӧ������ */
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

	// ����������
	template<typename> friend class TInventoryIterator;

	// ��������
	inline AKGInventory* GetInventory()
	{
		return InventoryList;
	}

	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void AddInventory(AKGInventory* InvToAdd, bool bAutoActivate);

	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void RemoveInventory(AKGInventory* InvRemove);

	/* �ӱ������ҳ��������͵ı��� */
	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual AKGInventory* K2_FindInventoryType(TSubclassOf<AKGInventory> Type, bool bExactClass = false) const;

	template<typename InvClass>
	inline InvClass* FindInventoryType(TSubclassOf<InvClass> Type, bool bExactClass = false) const
	{
		InvClass* Result = (InvClass*)K2_FindInventoryType(Type, bExactClass);
		checkSlow(Result == NULL || Result->IsA(InvClass::StaticClass()));
		return Result;
	}

	/* �л����� */
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void SwitchWeapon(AKGWeapon* NewWeapon);

	inline bool IsPendingFire(uint8 FireMode) const
	{
		// ����ܿ�����FireMode��Ч
		return !IsFiringDisabled() && (FireMode < PendingFire.Num() && PendingFire[FireMode] != 0);
	}

	inline void SetPendingFire(uint8 FireMode, bool bNowFiring)
	{
		// �����С���������󲢳�ʼΪ0
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
	/* ��װ��ȡ�£�������ʱ����pendingweaponתΪweapon,������*/
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

	/* �������� */
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

	/** ��ǰ/���ƶ� */
	virtual void MoveForward(float Val);
	/** ����/���ƶ� */
	virtual void MoveRight(float Val);

public:
	UPROPERTY(EditAnywhere, Category = "Pawn")
	TArray<TSubclassOf<AKGInventory> > DefaultCharacterInventory;

protected:

	/* �ͻ����л������������������ִ�� */
	virtual void LocalSwitchWeapon(AKGWeapon* NewWeapon);

	UFUNCTION(Client, Reliable)
	virtual void ClientSwitchWeapon(AKGWeapon* NewWeapon);

	// firemodes with input currently being held down (pending or actually firing), 1������0��û�п���
	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
	TArray<uint8> PendingFire;

	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
	AKGInventory* InventoryList;	// ����

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
