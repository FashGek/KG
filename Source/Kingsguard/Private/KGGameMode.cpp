// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"

AKGGameMode::AKGGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default paw class to blupeint character               "Class'/Game/RestrictedAssets/Blueprints/WIP/Steve/SteveUTCharacter.SteveUTCharacter_C'"            
	// "Class'/Game/RestrictedAssets/Blueprints/WIP/WenShan/WenShanKGCharacter.WenShanKGCharacter_C'"
	static ConstructorHelpers::FObjectFinder<UClass> WenShanPawnObject(TEXT("Class'/Game/RestrictedAssets/Blueprints/WIP/WenShan/WenShanKGCharacter.WenShangKGCharacter_C'"));
	if (WenShanPawnObject.Object != NULL) 
	{
		DefaultPawnClass = (UClass*)WenShanPawnObject.Object;
	}

	// hud class
	HUDClass = AKGHUD::StaticClass();

	// playercontroller class
	PlayerControllerClass = AKGPlayerController::StaticClass();

}

void AKGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	PostInitGame(Options);
}

void AKGGameMode::Reset()
{
	Super::Reset();
}

void AKGGameMode::RestartGame()
{
	Super::RestartGame();
}

bool AKGGameMode::IsEnemy(AController* First, AController* Second)
{

	return First != Second;
}

void AKGGameMode::Killed(class AController* Killer, class AController* KilledPlayer, class APawn* KilledPawn, TSubclassOf<UDamageType> DamageType)
{

}

void AKGGameMode::NotifyKilled(class AController* Killer, AController* Killed, APawn* KilledPawn, TSubclassOf<UDamageType> DamageType)
{

}

void AKGGameMode::RestartFromLastCheckpoint()
{

}

void AKGGameMode::RestartChapter()
{

}

TSubclassOf<class AGameSession> AKGGameMode::GetGameSessionClass() const
{
	return AKGGameSession::StaticClass();
}

APlayerController* AKGGameMode::Login(class UPlayer* NewPlayer, const FString& Portal, const FString& Options, const TSharedPtr<class FUniqueNetId>& UniqueId, FString& ErrorMessage)
{
	return Super::Login(NewPlayer, Portal, Options, UniqueId, ErrorMessage);
}

void AKGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
}

static bool bSetPlayerDefaultsSpawnInventory = false;

void AKGGameMode::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);

	if (bSetPlayerDefaultsSpawnInventory)
	{
		GiveDefaultsInventory(PlayerPawn);
	}
}

void AKGGameMode::GiveDefaultsInventory(APawn* PlayerPawn)
{
	AKGCharacter* KGCharacter = Cast<AKGCharacter>(PlayerPawn);
	if (KGCharacter != NULL)
	{
		KGCharacter->AddDefaultInventory(DefaultInventory);
	}

}

void AKGGameMode::RestartPlayer(class AController* NewPlayer)
{
	if (NewPlayer == NULL)
	{
		UE_LOG(KG, Warning, TEXT("Restart a player with a bad player"));
		return;
	}

	bSetPlayerDefaultsSpawnInventory = true;
	Super::RestartPlayer(NewPlayer);
	bSetPlayerDefaultsSpawnInventory = false;


}
