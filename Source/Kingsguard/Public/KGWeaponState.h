
#pragma once

#include "KGWeaponState.generated.h"

/**
  * EditInlinNew: �༭����New��ť���Թ���һ������
  * CustomConstructor: ��ֹ���������Զ��������캯��
  * Within: ��һ���ⲿ����֮��Ӧ?what the fuck?(Inner class)�������ˣ�����ǿ�ư�KGWeaponState��KGWeapon��������
 */
UCLASS(DefaultToInstanced, EditInlineNew, CustomConstructor, Within=KGWeapon)
class UKGWeaponState : public UObject
{
	GENERATED_UCLASS_BODY()
	UKGWeaponState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{}

	inline AKGCharacter* GetKGOwner()
	{
		return GetOuterAKGWeapon()->GetKGOwner();
	}

	virtual UWorld* GetWorld() const override
	{
		return GetOuterAKGWeapon()->GetWorld();
	}

	virtual void BeginState(const UKGWeaponState* PrevState)
	{}
	virtual void EndState()
	{}

	virtual bool BeginFiringSequence(uint8 FireModeNum, bool bClientFired)
	{
		return false;
	}

	virtual void EndFiringSequence(uint8 FireModeNum)
	{}

	/*��,��ǰ֡������������ͬ�� */
	virtual bool WillSpawnShot()
	{
		return false;
	}

	virtual void BringUp(float OverflowTime)
	{

	}

	virtual void PutDown()
	{
		//GetOuterAKGWeapon()->UnEquip();
	}

	virtual bool IsFiring() const
	{
		return false;
	}

	virtual void UpdateTiming()
	{}
	virtual void Tick(float DeltaTime)
	{}
};
