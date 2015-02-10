// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameSession.h"
#include "KGGameSession.generated.h"

/**
 * 
 */
UCLASS()
class KINGSGUARD_API AKGGameSession : public AGameSession
{
	GENERATED_UCLASS_BODY()
	
public:
	
	// ������ҵ�½������
	FString ApproveLogin(const FString& Options);

	// GameMode
	AKGBaseGameMode* KGGameMode;

public:

	virtual void InitOptions(const FString& Options);
	virtual bool ProcessAutoLogin();
	virtual void Destroyed();
};
