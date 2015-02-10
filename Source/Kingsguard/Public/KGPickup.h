

#pragma once

#include "KGPickup.generated.h"


UENUM()
enum EPickupClassification
{
	PC_Minor,
	PC_Major,
	PC_Super
};

UCLASS(abstract, Blueprintable, meta = (ChildCanTick))
class KINGSGUARD_API AKGPickup : public AActor
{
	GENERATED_UCLASS_BODY()

	/** ��ײ��� */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	UCapsuleComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	USoundBase* TakenSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pickup)
	TEnumAsByte<EPickupClassification> PickupType;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()	// ���Ӿ�Ȼ����һ����ֵ�BUG
	virtual void OnOverlapBegin(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	virtual void OnEndBegin(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* bp �ﴦ���˰�ť�󣬵������������������� */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	virtual void HandleKeyDown(APawn* Target);

	UFUNCTION(BlueprintNativeEvent)
	bool AllowPickupBy(APawn* Other, bool bDefaultAllowPickup);

	UFUNCTION(BlueprintNativeEvent)
	void ProcessTouch(APawn* TouchedBy);

	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly)
	void GiveTo(APawn* Target);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pickup)
	FText PickupMessageString;


};
