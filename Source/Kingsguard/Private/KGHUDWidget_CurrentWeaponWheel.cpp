// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGHUDWidget_CurrentWeaponWheel.h"


UKGHUDWidget_CurrentWeaponWheel::UKGHUDWidget_CurrentWeaponWheel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Tex(TEXT("Texture2D'/Game/RestrictedAssets/UI/PowerWheel_SF/PowerWheel_I1E'"));
	TexturePowerWheelBackground = Tex.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Tex2(TEXT("Texture2D'/Game/RestrictedAssets/UI/PowerWheel_SF/powerwheel_IAD'"));
	TexturePowerWheelNumBackground = Tex2.Object;

}


void UKGHUDWidget_CurrentWeaponWheel::InitializeWidget(AKGHUD* Hud)
{
	Super::InitializeWidget(Hud);
}

void UKGHUDWidget_CurrentWeaponWheel::Draw_Implementation(float DeltaTime)
{
	// DrawTexture(TexturePowerWheelBackground, 230, 230, 60.0f, 60.0f, 0, 0, 60, 60, 1.0f, FLinearColor::Black);
	// DrawTexture(TexturePowerWheelNumBackground, 190, 250, 48.0f, 48.0f, 0, 0, 48, 48, 1.0f, FLinearColor::Black);
}
