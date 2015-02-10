
#include "Kingsguard.h"
#include "KGHUDWidget_HealthBar.h"

UKGHUDWidget_HealthBar::UKGHUDWidget_HealthBar(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Tex(TEXT("Texture2D'/Game/RestrictedAssets/UI/HUD_SF/HUD_I5C'"));
	TextureAtlas = Tex.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> TexBgIa4(TEXT("Texture2D'/Game/RestrictedAssets/UI/HUD_SF/HUD_IA4'"));
	TextureBgIA4 = TexBgIa4.Object;
}

void UKGHUDWidget_HealthBar::InitializeWidget(AKGHUD* Hud)
{
	Super::InitializeWidget(Hud);
}


void UKGHUDWidget_HealthBar::Draw_Implementation(float DeltaTime)
{
	// draw background
	float BgWidth = 221.0f, BgHeight = 234.0f, BgMU = .0f, BgMV = 256.0f, BgMUL = BgWidth, BgMVL = BgHeight;
	DrawTexture(TextureAtlas, 0, 0, BgWidth, BgHeight, BgMU, BgMV, BgMUL, BgMVL);
	DrawTexture(TextureBgIA4, 20, 80, 173.0f, 173.0f, 166, 219, 173, 173);
	// draw 血条外框
	float OutlineLocationX = 90.0f, OutlineLocationY = 51.0f, Degree = 15.0f;
	//DrawTexture(TextureAtlas, OutlineLocationX, OutlineLocationY, 38.0f, 205.0f, 453, 38, 38, 205, 1.0, FLinearColor::White, FVector2D(0, 0), 30, FVector2D(0, 1));
	DrawTexture(TextureAtlas, OutlineLocationX, OutlineLocationY, 38.0f, 205.0f, 453, 38, 38, 205, 1.0, FLinearColor::White, FVector2D(0, 0), Degree);
	// draw 血条背景, 坑爹，资源在另外一张纹理上
	// draw 血条
	DrawTexture(TextureAtlas, OutlineLocationX, OutlineLocationY, 38.0f, 205.0f, 406, 40, 38, 205, 1.0, FLinearColor::White, FVector2D(0, 0), Degree);
}
