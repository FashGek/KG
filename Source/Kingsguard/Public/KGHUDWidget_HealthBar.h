
#pragma once

#include "KGHUDWidget_HealthBar.generated.h"

UCLASS()
class UKGHUDWidget_HealthBar : public UKGHUDWidget
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeWidget(AKGHUD* Hud);
	virtual void Draw_Implementation(float DeltaTime);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	UTexture2D* TextureAtlas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	UTexture2D* TextureBgIA4;
};
