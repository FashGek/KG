
#include "Kingsguard.h"
#include "KGHUDWidget_Test.h"

UKGHUDWidget_Test::UKGHUDWidget_Test(const FObjectInitializer& ObejctInitializer)
	: Super(ObejctInitializer)
{
	Position = FVector2D(0.0f, 0.0f);
	Size = FVector2D(200.0f, 200.0f);
	ScreenPosition = FVector2D(0.5f, 0.5f);
	Origin = FVector2D(0.5f, 0.5f);
	RenderPosition = FVector2D(0.0f, 0.0f);

	// º”‘ÿ≤ƒ÷ 
// 	static ConstructorHelpers::FObjectFinder<UTexture2D> Tex(TEXT("Texture2D'/Game/RestrictedAssets/UI/HUD_SF/HUD_I46.HUD_I46'"));
// 	TextureAltas = Tex.Object;
	static ConstructorHelpers::FObjectFinder<UTexture2D> Tex(TEXT("Texture2D'/Game/RestrictedAssets/UI/UTScoreboard01.UTScoreboard01'"));
	TextureAtlas = Tex.Object;
}

void UKGHUDWidget_Test::InitializeWidget(AKGHUD* Hud)
{
	Super::InitializeWidget(Hud);
}

void UKGHUDWidget_Test::Draw_Implementation(float DeltaTime)
{
	// DrawText(FText::FromString(TEXT("Hello Widget")), 0, 0, KGHUDOwner->LargeFont, 1.0, 1.0, FLinearColor::Black, ETextHorzPos::Left, ETextVertPos::Top);
	DrawTexture(TextureAtlas, 0, 0, 1920.0, 108.0f, 4, 2, 124, 128, 1.0);

	DrawTexture(TextureAtlas, 20, 54, 301, 98, 162, 14, 301, 98.0, 1.0f, FLinearColor::White, FVector2D(0.0, 0.5));

	DrawTexture(TextureAtlas, 341, 54, 4, 99, 488, 13, 4, 99, 1.0f, FLinearColor::White, FVector2D(0.0, 0.5));
}
