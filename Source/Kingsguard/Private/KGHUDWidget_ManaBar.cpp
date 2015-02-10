// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGHUDWidget_ManaBar.h"

UKGHUDWidget_ManaBar::UKGHUDWidget_ManaBar(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Tex(TEXT("Texture2D'/Game/RestrictedAssets/UI/HUD_SF/HUD_I85'"));
	TextureAtlas = Tex.Object;
}

void UKGHUDWidget_ManaBar::InitializeWidget(AKGHUD* Hud)
{
	Super::InitializeWidget(Hud);
}


void UKGHUDWidget_ManaBar::Draw_Implementation(float DeltaTime)
{
	// Drawbackground
	DrawTexture(TextureAtlas, 130, 300, 240.0f, 209.0f, 29, 19, 240, 209, 0.3f, FLinearColor::White, FVector2D(0, 0), -120, FVector2D(0, 0));
	// À¶ÌõÍâ¿ò
	float ManaX = 80.0f, ManaY = 45.0f, Degree = 30.0f;
	DrawTexture(TextureAtlas, ManaX, ManaY, 38.0f, 205.0f, 59, 333, 28, 170, 1.0, FLinearColor::White, FVector2D(0, 0), Degree, FVector2D(0, 1));
	// 
	
	DrawTexture(TextureAtlas, ManaX, ManaY, 38.0f, 205.0f, 107, 333, 29, 170, 1.0f, FLinearColor::White, FVector2D(0, 0), Degree, FVector2D(0, 1));
}


