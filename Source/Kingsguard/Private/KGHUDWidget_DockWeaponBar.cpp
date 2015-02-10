// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGHUDWidget_DockWeaponBar.h"

UKGHUDWidget_DockWeaponBar::UKGHUDWidget_DockWeaponBar(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Tex1(TEXT("Texture2D'/Game/RestrictedAssets/UI/PowerWheel_SF/shPC_dock_bkgd_-nopack'"));
	TextureDockBkg = Tex1.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Tex2(TEXT("Texture2D'/Game/RestrictedAssets/UI/PowerWheel_SF/powerwheel_I1E'"));
	TextureWeaponWhell = Tex2.Object;
}

void UKGHUDWidget_DockWeaponBar::InitializeWidget(AKGHUD* Hud)
{
	Super::InitializeWidget(Hud);
}


void UKGHUDWidget_DockWeaponBar::Draw_Implementation(float DeltaTime)
{
	// Draw dock background
	float DockWidth = 720.0f, DockHeight = 103.0f, RandoffsetX = 0.5f, RandoffsetY = 1.0f;
	DrawTexture(TextureDockBkg, Canvas->SizeX / 2.0f, Canvas->SizeY, DockWidth, DockHeight, 0, 0, 720, DockHeight, 1.0, FLinearColor::White, FVector2D(RandoffsetX, RandoffsetY));
	// »­Ô²»·
	float PowerwheelStartX = Canvas->SizeX / 2.0f - (DockWidth * RandoffsetX) + 30.0f;
	float PowerwheelStartY = Canvas->SizeY - (DockHeight * RandoffsetY) + 30.0f;
	DrawTexture(TextureWeaponWhell, PowerwheelStartX, PowerwheelStartY, 60, 60, 0, 0, 60, 60);
}
