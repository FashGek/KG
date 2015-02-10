
#pragma once

#include "KGPlayerController.generated.h"

struct FDeferredFireInput
{
	/* ����ģʽ��� */
	uint8 FireMode;
	/* true, ���� false, ͣ��*/
	bool bStartFire;

	FDeferredFireInput(uint8 InFireMode, bool bStartFire) : FireMode(InFireMode), bStartFire(bStartFire)
	{}
};

UCLASS(config=Game)
class KINGSGUARD_API AKGPlayerController : public AKGBasePlayerController
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = PlyaerController)
	virtual AKGCharacter* GetKGCharacter();

	virtual void BeginPlay() override;
	virtual void InitInputSystem() override;
	virtual void SetupInputComponent() override;
	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostInitializeComponents() override;
	virtual void SetPawn(APawn* InPawn);


	virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& HiddenComponents) /*override*/;


	// ��д�˽ӿ�Engine�Żᴦ���û�����
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;


	virtual void PlayerTick(float DeltaTime) override;

	virtual void CheckAutoWeaponSwitch(AKGWeapon* TestWeapon);

	UPROPERTY(GlobalConfig, BlueprintReadOnly, Category = Weapon)
	bool bAutoWeaponSwitch;

	UPROPERTY()
	class AKGHUD* MyKGHUD;

protected:

	/** ǰ������ı��� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementForwardAxis;

	/** ���ҷ���ı��� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementStrafeAxis;

	/* �������� */
	virtual void OnFire();

	virtual void MoveForward(float Val);	/** ������ǰ�ƶ� */
	virtual void MoveBackward(float Val);	/** ��������ƶ� */
	virtual void MoveLeft(float Val);	/** ���������ƶ� */
	virtual void MoveRight(float Val);	/** ���������ƶ� */

	/** stores fire inputs until after movement has been executed (default would be fire -> movement -> render, this causes movement -> fire -> render)
	* makes weapons feel a little more responsive while strafing
	* ��������
	*/
	TArray< FDeferredFireInput, TInlineAllocator<2> > DeferredFireInputs;
private:

	UPROPERTY()
	AKGCharacter* KGCharacter;

public:
	void ApplyDeferredFireInputs();

};
