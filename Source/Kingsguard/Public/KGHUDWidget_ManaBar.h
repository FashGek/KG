// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KGHUDWidget.h"
#include "KGHUDWidget_ManaBar.generated.h"

/**
 * 
 */
UCLASS()
class KINGSGUARD_API UKGHUDWidget_ManaBar : public UKGHUDWidget
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeWidget(AKGHUD* Hud);
	virtual void Draw_Implementation(float DeltaTime);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	UTexture2D* TextureAtlas;
	
};
