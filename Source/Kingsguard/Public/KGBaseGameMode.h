// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "KGBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KINGSGUARD_API AKGBaseGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Game)
	FText DisplayName;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
};
