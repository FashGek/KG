// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGCharacter.h"
#include "KGCharacterMovement.h"
#include "Animation/AnimInstance.h"


AKGCharacter::AKGCharacter(const class FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKGCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	// 设置Capsule碰撞检测大小
	GetCapsuleComponent()->SetCapsuleSize(46.f, 92.0f);

	// 设置相机
	CharacterCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	CharacterCameraComponent->AttachParent = GetCapsuleComponent();
	// 设置相机高度
	DefautBaseEyeHeight = 71.f;
	BaseEyeHeight = DefautBaseEyeHeight;	// 父类拥有的
	CharacterCameraComponent->RelativeLocation = FVector(0, 0, DefautBaseEyeHeight);

	// 第一人称模型
	FirstPersonMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
	FirstPersonMesh->AttachParent = CharacterCameraComponent;
	FirstPersonMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// 玩家移动对象
	// KGCharacterMovement = Cast<UKGCharacterMovement>(GetCharacterMovement());

	HealthMax = 100.0f;	//    不要直接引用数字
	ManaMax = 20000.0f;	//  
}

void AKGCharacter::Destroyed()
{
	// 死亡与销毁是不同的
	Super::Destroyed();
}

void AKGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKGCharacter::Restart()
{
	Super::Restart();
}

void AKGCharacter::BeginPlay()
{
	//  
	// 现在模型太丑,档住了屏幕，所以先隐藏, 实现在UpdateHiddenComponent
	GetMesh()->SetOwnerNoSee(false);

	if (GetWorld()->GetNetMode() !=  NM_DedicatedServer)
	{
		APlayerController* PC = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (PC != NULL && PC->MyHUD != NULL)
		{
			PC->MyHUD->AddPostRenderedActor(this);
		}
	}

	if (Health == 0) 
	{
		Health = HealthMax;
	}

	Super::BeginPlay();
}

void AKGCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// 找到前进的方向
		const FRotator Rotation = GetControlRotation();
		FRotator YawRotation = (KGCharacterMovement && KGCharacterMovement->Is3DMovementMode()) ? Rotation : FRotator(0, Rotation.Yaw, 0);

		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
	}
}

void AKGCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
	}
}

void AKGCharacter::AddInventory(AKGInventory* InvToAdd, bool bAutoActivate)
{
	if (InvToAdd != NULL)
	{
		if (InventoryList == NULL)
		{
			InventoryList = InvToAdd;
		}
		else if (InventoryList == InvToAdd)
		{
			//   log
		}
		else 
		{
			AKGInventory* Last = InventoryList;
			while (Last->NextInventory != NULL)
			{
				// avoid recursion
				if (Last->NextInventory == InvToAdd)
				{
					//   Log
					return;
				}
				Last->NextInventory = InvToAdd;
			}
		}
		InvToAdd->GivenTo(this, bAutoActivate);
	}
}

void AKGCharacter::RemoveInventory(AKGInventory* InvRemove)
{

}

void AKGCharacter::StartFire(uint8 FireModeNum)
{
	//   Log

	if (bFeigningDeath)
	{
		FeignDeath();
	}
	else if (Weapon != NULL && EmoteCount == 0)
	{
		Weapon->StartFire(FireModeNum);
	}
}

void AKGCharacter::PlayFeignDeath()
{
	// 为什么直译为假装受伤
	if (bFeigningDeath)
	{
		//  
		if (Weapon != nullptr /*&& Weapon->*/)
		{
		}
	}
}

void AKGCharacter::FeignDeath()
{
	//  
}

void AKGCharacter::StopFire(uint8 FireModeNum)
{
	if (Weapon != NULL)
	{
		Weapon->StopFire(FireModeNum);
	}
	else
	{
		SetPendingFire(FireModeNum, false);
	}
}

void AKGCharacter::StopFiring()
{
	for (int32 i = 0; i < PendingFire.Num(); i++)
	{
		if (PendingFire[i])
		{
			StopFire(i);
		}
	}
}

void AKGCharacter::AddDefaultInventory(TArray<TSubclassOf<AKGInventory> > DefaultInventoryToAdd)
{
	for (int i = 0; i < DefaultCharacterInventory.Num(); i++)
	{
		AddInventory(GetWorld()->SpawnActor<AKGInventory>(DefaultCharacterInventory[i], FVector(0.0f), FRotator(0, 0, 0)), true);
	}

	//
	for (int i = 0; i < DefaultInventoryToAdd.Num(); i++)
	{
		AddInventory(GetWorld()->SpawnActor<AKGInventory>(DefaultInventoryToAdd[i], FVector(0.0f), FRotator(0, 0, 0)), true);
	}
}

bool AKGCharacter::IsInInventory(const AKGInventory* TestInv) const
{
	for (AKGInventory* Inv = InventoryList; Inv != NULL; Inv = Inv->GetNext())
	{
		if (Inv == TestInv)
		{
			return true;
		}
	}
	return false;
}

void AKGCharacter::SwitchWeapon(AKGWeapon* NewWeapon)
{
	if (NewWeapon != NULL &&!IsDead())
	{
		if (Role == ROLE_Authority)
		{
			ClientSwitchWeapon(NewWeapon);
		}
		//  
	}
}

void AKGCharacter::ClientSwitchWeapon_Implementation(AKGWeapon* NewWeapon)
{
	LocalSwitchWeapon(NewWeapon);
}

void AKGCharacter::LocalSwitchWeapon(AKGWeapon* NewWeapon)
{
	if (!IsDead())
	{
		if (NewWeapon != NULL && (NewWeapon->GetOwner() == NULL || (Role == ROLE_Authority && !IsInInventory(NewWeapon))))
		{
			// 这里是延迟调用，保证一些数据正确性
			ClientSwitchWeapon(NewWeapon);
		}
		else
		{
			if (Weapon == NULL)			// 玩家没有武器，就初始化一件
			{
				if (NewWeapon != NULL)
				{
					// 初始化装备
					PendingWeapon = NewWeapon;
					WeaponChanged();
				}
			}
			else if (NewWeapon != NULL)		// 玩家已经手持武器，所以要放下当前武器，装备切换
			{
				if (NewWeapon != Weapon)
				{
					if (Weapon->PutDown())
					{
						PendingWeapon = NewWeapon;
					}
				}
				else if (PendingWeapon != NULL) // 如果切换的武器与手上的是同一把，就把当前武器拉上，并不切换
				{
					PendingWeapon = NULL;
					Weapon->BringUp();
				}
			}
			else if (Weapon != NULL && PendingWeapon != NULL && PendingWeapon->PutDown())		// 如果玩家手持武器，又已经在切换武器中，则忽略此次切换，并拉上武器
			{
				PendingWeapon = NULL;
				Weapon->BringUp();
			}
		}
	}
}

void AKGCharacter::WeaponChanged(float OverflowTime /*= 0.0f*/)
{
	if (PendingWeapon != NULL)
	{
		checkSlow(IsInInventory(PendingWeapon));
		Weapon = PendingWeapon;
		Weapon->BringUp(OverflowTime);
		WeaponClass = Weapon->GetClass();
		// 
		UpdateWeaponAttachment();
	}
	else if (Weapon != NULL)
	{
		// 复位weapon为拉上
		Weapon->BringUp(OverflowTime);
	}
	else 
	{
		WeaponClass = NULL;
		UpdateWeaponAttachment();
	}
}

void AKGCharacter::UpdateWeaponAttachment()
{
	if (GetNetMode() != NM_DedicatedServer)
	{

	}
}

void AKGCharacter::AddAmmo(const FStoredAmmo& AmmoToAdd)
{
	AKGWeapon* ExistingWeapon = FindInventoryType<AKGWeapon>(AmmoToAdd.Type, true);
	if (ExistingWeapon != NULL)
	{

	}
	else 
	{
		for (int i = 0; i < SavedAmmo.Num(); i++)
		{
			if (SavedAmmo[i].Type == AmmoToAdd.Type)
			{
				SavedAmmo[i].Amount += AmmoToAdd.Amount;
				if (SavedAmmo[i].Amount <= 0)
				{
					SavedAmmo.RemoveAt(i);
				}
				return;
			}
		}

		if (AmmoToAdd.Amount > 0)
		{
			new (SavedAmmo)FStoredAmmo(AmmoToAdd); // odd
		}
	}
}

AKGInventory* AKGCharacter::K2_FindInventoryType(TSubclassOf<AKGInventory> Type, bool bExactClass /*= false*/) const
{
	for (TInventoryIterator<> It(this); It; ++It)
	{
		if (bExactClass ? (It->GetClass() == Type) : It->IsA(Type))
		{
			return *It;
		}
	}
	return NULL;
}

void AKGCharacter::AllAmmo()
{

}

bool AKGCharacter::HasMaxAmmo(TSubclassOf<AKGWeapon> Type) const
{
	if (Type != NULL)
	{
		// 计算玩家携带的
		int32 AllAmmoAmount = 0;
		for (int32 i = 0; i < SavedAmmo.Num(); i++)
		{
			if (SavedAmmo[i].Type == Type)
			{
				AllAmmoAmount += SavedAmmo[i].Amount;
			}
		}
		// 计算武器弹甲的
		//  
		return AllAmmoAmount >= Type.GetDefaultObject()->MaxAmmo;
	}
	else 
	{
		return true;
	}
}

int32 AKGCharacter::GetAmmoAmount(TSubclassOf<AKGWeapon> Type) const
{
	if (Type == NULL)
	{
		return 0;
	}
	else
	{
		int32 AllAmmoAmount = 0;
		for (int32 i = 0; i < SavedAmmo.Num(); i++)
		{
			if (SavedAmmo[i].Type == Type)
			{
				AllAmmoAmount += SavedAmmo[i].Amount;
			}
		}

		return AllAmmoAmount;
	}
}
