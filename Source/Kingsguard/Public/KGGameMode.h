// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KINGSGUARD_API AKGGameMode : public AKGBaseGameMode
{
	GENERATED_UCLASS_BODY()

public:
	
	/** 游戏难度 */
	UPROPERTY(globalconfig)
	float GameDifficulty;

	/* 默认扔进背包里东西, 但是呢有了Pickup后，可以不再加，但是如果玩家有保存可以这样做 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<TSubclassOf<AKGInventory> > DefaultInventory;	

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	UFUNCTION(BlueprintImplementableEvent)
	void PostInitGame(const FString& Options);
	
	virtual void Reset();
	virtual void RestartGame();
	/** 从上一个检查点玩 */
	virtual void RestartFromLastCheckpoint();
	/** 重玩这一章 */
	virtual void RestartChapter();
	virtual bool IsEnemy(AController* First, AController* Second);
	virtual void Killed(class AController* Killer, class AController* KilledPlayer, class APawn* KilledPawn, TSubclassOf<UDamageType> DamageType);
	/** 因为有一个女NPC，玩家死了需要通知她 */
	virtual void NotifyKilled(class AController* Killer, AController* Killed, APawn* KilledPawn, TSubclassOf<UDamageType> DamageType);
	/** 设置GameSessio类，现在引擎老是在报GameSession指针为空，不懂*/
	virtual TSubclassOf<class AGameSession> GetGameSessionClass() const;

	virtual APlayerController* Login(class UPlayer* NewPlayer, const FString& Portal, const FString& Options, const TSharedPtr<class FUniqueNetId>& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void RestartPlayer(class AController* NewPlayer) override;

	virtual void SetPlayerDefaults(APawn* PlayerPawn) override;
	virtual void GiveDefaultsInventory(APawn* PlayerPawn);
};
