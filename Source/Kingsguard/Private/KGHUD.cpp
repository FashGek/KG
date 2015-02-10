// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "KGHUD.h"

AKGHUD::AKGHUD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WidgetOpactiy = 1.0f;	// if is zero so not show it

	// 加载字体
	static ConstructorHelpers::FObjectFinder<UFont> SFont(TEXT("Font'/Game/RestrictedAssets/Fonts/fntSmallFont.fntSmallFont'"));
	SmallFont = SFont.Object;

	static ConstructorHelpers::FObjectFinder<UFont> MFont(TEXT("Font'/Game/RestrictedAssets/Fonts/fntMediumFont.fntMediumFont'"));
	MediumFont = MFont.Object;

	static ConstructorHelpers::FObjectFinder<UFont> LFont(TEXT("Font'/Game/RestrictedAssets/Fonts/fntLargeFont.fntLargeFont'"));
	LargeFont = LFont.Object;

	static ConstructorHelpers::FObjectFinder<UFont> NFont(TEXT("Font'/Game/RestrictedAssets/Fonts/fntNumbers.fntNumbers'"));
	NumberFont = NFont.Object;

}


void AKGHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKGHUD::ShowDebugInfo(float& YL, float& YPos)
{
	
}

void AKGHUD::PostRender()
{
	

	Super::PostRender();
}

void AKGHUD::DrawHUD()
{
	Super::DrawHUD();

	// DrawString(FText::FromString(TEXT("hello HUD")),300, 300, ETextHorzPos::Right, ETextVertPos::Bottom, MediumFont, FLinearColor::Blue, 1.0, false);

	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	for (int WidgetIndex = 0; WidgetIndex < HudWigets.Num(); WidgetIndex++)
	{
		// 如果设置为显示就设置参数，开始渲染
		if (HudWigets[WidgetIndex] && !HudWigets[WidgetIndex]->IsHidden())
		{
			HudWigets[WidgetIndex]->PreDraw(RenderDelta, this, Canvas, Center);
			if (HudWigets[WidgetIndex]->ShouldDraw())
			{
				HudWigets[WidgetIndex]->Draw(RenderDelta);
			}
			HudWigets[WidgetIndex]->PostDraw(GetWorld()->GetTimeSeconds());
		}
	}
}

// 这里Canvas还没有准备好，不能画任何东西
void AKGHUD::BeginPlay()
{
	Super::BeginPlay();

	// 分析Game.ini里widget
	for (int i = 0; i < RequiredHudWidgetClasses.Num(); i++)
	{
		BuildHudWidget(*RequiredHudWidgetClasses[i]);
	}

	float d = HUDWidgetOpacity;

}

void AKGHUD::DrawString(FText Text, float X, float Y, ETextHorzPos::Type HorzAlignment, ETextVertPos::Type VertAlignment, UFont* Font, FLinearColor Color, float Scale /*= 1.0*/, bool bOutline /*= false*/)
{
	FVector2D RenderPos = FVector2D(X, Y);

	float XL, YL;
	Canvas->TextSize(Font, Text.ToString(), XL, YL, Scale, Scale);

	if (HorzAlignment != ETextHorzPos::Left)
	{
		RenderPos.X -= HorzAlignment == ETextHorzPos::Right ? XL : XL * 0.5f;
	}

	if (VertAlignment != ETextVertPos::Top)
	{
		RenderPos.Y -= VertAlignment == ETextVertPos::Bottom ? YL : YL * 0.5f;
	}

	FCanvasTextItem TextItem(RenderPos, Text, Font, Color);

	if (bOutline)
	{
		TextItem.bOutlined = true;
		TextItem.OutlineColor = FLinearColor::Black;
	}

	TextItem.Scale = FVector2D(Scale, Scale);
	Canvas->DrawItem(TextItem);
}

void AKGHUD::DrawNumber(int Number, float X, float Y, FLinearColor Color, float GlowOpacity, float Scale, int MinDigits /*= 0*/, bool bRightAlign /*= false*/)
{

}

void AKGHUD::BuildHudWidget(FString NewWidgetString)
{

	TSubclassOf<UKGHUDWidget> NewWidgetClass = ResolveHudWidgetByName(*NewWidgetString);
	if (NewWidgetClass != NULL)
	{
		AddHudWidget(NewWidgetClass);
	}
}

TSubclassOf<UKGHUDWidget> AKGHUD::ResolveHudWidgetByName(const TCHAR* ResourceName)
{
	UClass* WidgetClass = LoadClass<UKGHUDWidget>(NULL, ResourceName, NULL, LOAD_NoWarn | LOAD_Quiet, NULL);
	// 这里意味着是CPP硬编码的
	if (WidgetClass != NULL)
	{
		return WidgetClass;
	}
	// 这里意味着是BP派生的
	FString BlueprintResouceName = FString::Printf(TEXT("%s_C"), ResourceName);
	WidgetClass = LoadClass<UKGHUDWidget>(NULL, *BlueprintResouceName, NULL, LOAD_NoWarn | LOAD_Quiet, NULL);
	if (WidgetClass != NULL)
	{
		return WidgetClass;
	}

	return NULL;
}

UKGHUDWidget* AKGHUD::AddHudWidget(TSubclassOf<UKGHUDWidget> NewWidgetClass)
{
	if (NewWidgetClass == NULL)
	{
		return NULL;
	}

	UKGHUDWidget* Widget = ConstructObject<UKGHUDWidget>(NewWidgetClass, GetTransientPackage());
	HudWigets.Add(Widget);

	Widget->InitializeWidget(this);

	return Widget;
}
