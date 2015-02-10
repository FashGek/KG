
#pragma once

#include "KGWeaponState.generated.h"

/**
  * EditInlinNew: 编辑器，New按钮可以构造一个对象
  * CustomConstructor: 阻止构建工具自动声明构造函数
  * Within: 有一个外部类与之对应?what the fuck?(Inner class)，明白了，这是强制把KGWeaponState与KGWeapon关联起来
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

	/*真,当前帧开火，用于网络同步 */
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
