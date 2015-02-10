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
	
	// 允许玩家登陆服务器
	FString ApproveLogin(const FString& Options);

	// GameMode
	AKGBaseGameMode* KGGameMode;

public:

	virtual void InitOptions(const FString& Options);
	virtual bool ProcessAutoLogin();
	virtual void Destroyed();
};
