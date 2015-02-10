

#pragma once

#include "KGHUDWidget_Test.generated.h"

UCLASS()
class UKGHUDWidget_Test : public UKGHUDWidget
{
	GENERATED_UCLASS_BODY()

	virtual void Draw_Implementation(float DeltaTime);
	virtual void InitializeWidget(AKGHUD* Hud);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	UTexture2D* TextureAtlas;
};
