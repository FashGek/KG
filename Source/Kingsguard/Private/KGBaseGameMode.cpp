// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"

AKGBaseGameMode::AKGBaseGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AKGBaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{



	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(KG, Log, TEXT("AKGBaseGameMode"));
}
