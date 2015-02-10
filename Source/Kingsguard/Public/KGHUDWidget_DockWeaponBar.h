
#pragma once

#include "KGHUDWidget_DockWeaponBar.generated.h"

UCLASS()
class UKGHUDWidget_DockWeaponBar : public UKGHUDWidget
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeWidget(AKGHUD* Hud);
	virtual void Draw_Implementation(float DeltaTime);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	UTexture2D* TextureWeaponWhell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	UTexture2D* TextureDockBkg;
};
