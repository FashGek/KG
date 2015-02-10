
#pragma once

#include "KGPlayerController.generated.h"

struct FDeferredFireInput
{
	/* 开火模式序号 */
	uint8 FireMode;
	/* true, 开火， false, 停火*/
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


	// 重写此接口Engine才会处理用户输入
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;


	virtual void PlayerTick(float DeltaTime) override;

	virtual void CheckAutoWeaponSwitch(AKGWeapon* TestWeapon);

	UPROPERTY(GlobalConfig, BlueprintReadOnly, Category = Weapon)
	bool bAutoWeaponSwitch;

	UPROPERTY()
	class AKGHUD* MyKGHUD;

protected:

	/** 前进方向改变量 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementForwardAxis;

	/** 左右方向改变量 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementStrafeAxis;

	/* 武器操作 */
	virtual void OnFire();

	virtual void MoveForward(float Val);	/** 处理向前移动 */
	virtual void MoveBackward(float Val);	/** 处理向后移动 */
	virtual void MoveLeft(float Val);	/** 处理向左移动 */
	virtual void MoveRight(float Val);	/** 处理向右移动 */

	/** stores fire inputs until after movement has been executed (default would be fire -> movement -> render, this causes movement -> fire -> render)
	* makes weapons feel a little more responsive while strafing
	* 不明觉厉
	*/
	TArray< FDeferredFireInput, TInlineAllocator<2> > DeferredFireInputs;
private:

	UPROPERTY()
	AKGCharacter* KGCharacter;

public:
	void ApplyDeferredFireInputs();

};
