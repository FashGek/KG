// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KGBasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KINGSGUARD_API AKGBasePlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	virtual void SetupInputComponent() override;

};
