// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGGameSession.h"


AKGGameSession::AKGGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FString AKGGameSession::ApproveLogin(const FString& Options)
{
	return Super::ApproveLogin(Options);
}

void AKGGameSession::InitOptions(const FString& Options)
{
	Super::InitOptions(Options);

	KGGameMode = Cast<AKGBaseGameMode>(GetWorld()->GetAuthGameMode());
}

bool AKGGameSession::ProcessAutoLogin()
{
	return Super::ProcessAutoLogin();
}

void AKGGameSession::Destroyed()
{
	Super::Destroyed();
}
