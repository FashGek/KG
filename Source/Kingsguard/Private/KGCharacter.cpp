// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGCharacter.h"
#include "KGCharacterMovement.h"
#include "Animation/AnimInstance.h"


AKGCharacter::AKGCharacter(const class FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKGCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	// ����Capsule��ײ����С
	GetCapsuleComponent()->SetCapsuleSize(46.f, 92.0f);

	// �������
	CharacterCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	CharacterCameraComponent->AttachParent = GetCapsuleComponent();
	// ��������߶�
	DefautBaseEyeHeight = 71.f;
	BaseEyeHeight = DefautBaseEyeHeight;	// ����ӵ�е�
	CharacterCameraComponent->RelativeLocation = FVector(0, 0, DefautBaseEyeHeight);

	// ��һ�˳�ģ��
	FirstPersonMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
	FirstPersonMesh->AttachParent = CharacterCameraComponent;
	FirstPersonMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// ����ƶ�����
	// KGCharacterMovement = Cast<UKGCharacterMovement>(GetCharacterMovement());

	HealthMax = 100.0f;	//    ��Ҫֱ����������
	ManaMax = 20000.0f;	//  
}

void AKGCharacter::Destroyed()
{
	// �����������ǲ�ͬ��
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
	// ����ģ��̫��,��ס����Ļ������������, ʵ����UpdateHiddenComponent
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
		// �ҵ�ǰ���ķ���
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
	// Ϊʲôֱ��Ϊ��װ����
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
			// �������ӳٵ��ã���֤һЩ������ȷ��
			ClientSwitchWeapon(NewWeapon);
		}
		else
		{
			if (Weapon == NULL)			// ���û���������ͳ�ʼ��һ��
			{
				if (NewWeapon != NULL)
				{
					// ��ʼ��װ��
					PendingWeapon = NewWeapon;
					WeaponChanged();
				}
			}
			else if (NewWeapon != NULL)		// ����Ѿ��ֳ�����������Ҫ���µ�ǰ������װ���л�
			{
				if (NewWeapon != Weapon)
				{
					if (Weapon->PutDown())
					{
						PendingWeapon = NewWeapon;
					}
				}
				else if (PendingWeapon != NULL) // ����л������������ϵ���ͬһ�ѣ��Ͱѵ�ǰ�������ϣ������л�
				{
					PendingWeapon = NULL;
					Weapon->BringUp();
				}
			}
			else if (Weapon != NULL && PendingWeapon != NULL && PendingWeapon->PutDown())		// �������ֳ����������Ѿ����л������У�����Դ˴��л�������������
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
		// ��λweaponΪ����
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
		// �������Я����
		int32 AllAmmoAmount = 0;
		for (int32 i = 0; i < SavedAmmo.Num(); i++)
		{
			if (SavedAmmo[i].Type == Type)
			{
				AllAmmoAmount += SavedAmmo[i].Amount;
			}
		}
		// �����������׵�
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
