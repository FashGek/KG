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
	
	/** ��Ϸ�Ѷ� */
	UPROPERTY(globalconfig)
	float GameDifficulty;

	/* Ĭ���ӽ������ﶫ��, ����������Pickup�󣬿��Բ��ټӣ������������б������������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<TSubclassOf<AKGInventory> > DefaultInventory;	

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	UFUNCTION(BlueprintImplementableEvent)
	void PostInitGame(const FString& Options);
	
	virtual void Reset();
	virtual void RestartGame();
	/** ����һ�������� */
	virtual void RestartFromLastCheckpoint();
	/** ������һ�� */
	virtual void RestartChapter();
	virtual bool IsEnemy(AController* First, AController* Second);
	virtual void Killed(class AController* Killer, class AController* KilledPlayer, class APawn* KilledPawn, TSubclassOf<UDamageType> DamageType);
	/** ��Ϊ��һ��ŮNPC�����������Ҫ֪ͨ�� */
	virtual void NotifyKilled(class AController* Killer, AController* Killed, APawn* KilledPawn, TSubclassOf<UDamageType> DamageType);
	/** ����GameSessio�࣬�������������ڱ�GameSessionָ��Ϊ�գ�����*/
	virtual TSubclassOf<class AGameSession> GetGameSessionClass() const;

	virtual APlayerController* Login(class UPlayer* NewPlayer, const FString& Portal, const FString& Options, const TSharedPtr<class FUniqueNetId>& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void RestartPlayer(class AController* NewPlayer) override;

	virtual void SetPlayerDefaults(APawn* PlayerPawn) override;
	virtual void GiveDefaultsInventory(APawn* PlayerPawn);
};
