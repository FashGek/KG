
#include "Kingsguard.h"


#include "KGPlayerInput.h"

AKGPlayerController::AKGPlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AKGPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	Super::ProcessPlayerInput(DeltaTime, bGamePaused);
}

void AKGPlayerController::InitInputSystem()
{
	// 绑定UKGPlayerInput
	if (PlayerInput == NULL)
	{
		PlayerInput = ConstructObject<UKGPlayerInput>(UKGPlayerInput::StaticClass(), this);
	}

	Super::InitInputSystem();
}

void AKGPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AKGPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AKGPlayerController::MoveForward);
	InputComponent->BindAxis("MoveBackward", this, &AKGPlayerController::MoveBackward);
	InputComponent->BindAxis("MoveLeft", this, &AKGPlayerController::MoveLeft);
	InputComponent->BindAxis("MoveRight", this, &AKGPlayerController::MoveRight);





	InputComponent->BindAction("StartFire", IE_Pressed, this, &AKGPlayerController::OnFire);


}

AKGCharacter* AKGPlayerController::GetKGCharacter()
{
	return KGCharacter;
}

void AKGPlayerController::SetPawn(APawn* InPawn)
{
	AController::SetPawn(InPawn);

	KGCharacter = Cast<AKGCharacter>(InPawn);
}

void AKGPlayerController::MoveForward(float Value)
{
	if (Value != 0.0f && KGCharacter != NULL)
	{
		MovementForwardAxis = Value;
		KGCharacter->MoveForward(Value);
	}
	else if (GetSpectatorPawn() != NULL)
	{
		/* 这个东西有点像那个比如多塔/星际之类的，玩家死了还会让玩家能够移动，但此时并不是移动玩家的模型，只移动视角 */
		GetSpectatorPawn()->MoveForward(Value);
	}
}

bool AKGPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	// 首先处理UKGPlayerInput
	UKGPlayerInput* Input = Cast<UKGPlayerInput>(PlayerInput);

	if (Input != NULL)
	{

	}

	return Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
}

void AKGPlayerController::MoveBackward(float Value)
{
	MoveForward(Value * -1);
}

void AKGPlayerController::MoveLeft(float Value)
{
	MoveRight(Value * -1);
}

void AKGPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f && KGCharacter != NULL)
	{
		MovementStrafeAxis = Value;
		KGCharacter->MoveRight(Value);
	} 
	else if (GetSpectatorPawn() != NULL)
	{
		GetSpectatorPawn()->MoveRight(Value);
	}
}

void AKGPlayerController::OnFire()
{
	if (GetPawn() != NULL)
	{
		new(DeferredFireInputs) FDeferredFireInput(0, true);
	}

}

void AKGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);


	if (GetPawn() != NULL || Cast<UKGCharacterMovement>(GetPawn()->GetMovementComponent()) != NULL)
	{
		ApplyDeferredFireInputs();
	}
}

void AKGPlayerController::ApplyDeferredFireInputs()
{
	for (FDeferredFireInput& Input : DeferredFireInputs)
	{
		if (Input.bStartFire)
		{
			if (KGCharacter != NULL)
			{
				if (StateName == NAME_Playing)
				{
					KGCharacter->StartFire(Input.FireMode);
				}
			}
			else if (GetPawn() != nullptr)	// 不就是NULL吗
			{
				GetPawn()->PawnStartFire(Input.FireMode);
			}
		}
		else if (KGCharacter != NULL)
		{
			KGCharacter->StopFire(Input.FireMode);
		}
	}
	DeferredFireInputs.Empty();
}

void AKGPlayerController::CheckAutoWeaponSwitch(AKGWeapon* TestWeapon)
{
	if (KGCharacter != NULL && IsLocalPlayerController())
	{
		AKGWeapon* CurrWeapon = KGCharacter->GetPendingWeapon();
		if (CurrWeapon == NULL)
		{
			CurrWeapon = KGCharacter->GetWeapon();
		}
		if (CurrWeapon == NULL/* || (bAutoWeaponSwitch && !KGCharacter->IsPendingFire(CurrWeapon->GetCurrentFireMode()) && TestWeapon->GetAutoSwitchPriority() > CurrWeapon->GetAutoSwitchPriority())*/)
		{
			KGCharacter->SwitchWeapon(TestWeapon);
		}
	}
}

static void HideComponentTree(const UPrimitiveComponent* Primitive, TSet<FPrimitiveComponentId>& HiddenComponents)
{
	if (Primitive != NULL)
	{
		HiddenComponents.Add(Primitive->ComponentId);
		TArray<USceneComponent*> Children;
		Primitive->GetChildrenComponents(true, Children);
		for (int32 i = 0; i < Children.Num(); i++)
		{
			UPrimitiveComponent* ChildPrim = Cast<UPrimitiveComponent>(Children[i]);
			if (ChildPrim != NULL)
			{
				HiddenComponents.Add(ChildPrim->ComponentId);
			}
		}
	}
}

void AKGPlayerController::UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& HiddenComponents)
{
	Super::UpdateHiddenComponents(ViewLocation, HiddenComponents);
	
	// 隐藏必要的模型细节
	AKGCharacter* P = Cast<AKGCharacter>(GetViewTarget());

	if (P != NULL)
	{
		HideComponentTree(P->GetMesh(), HiddenComponents);
	}
}
