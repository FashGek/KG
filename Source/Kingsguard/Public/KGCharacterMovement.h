// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KGCharacterMovement.generated.h"

/**
 * 
 */
UCLASS()
class KINGSGUARD_API UKGCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void PerformMovement(float DeltaTime) override;

	/** 返回True 如果不需要约束到水平面 */
	virtual bool Is3DMovementMode() const;

};
