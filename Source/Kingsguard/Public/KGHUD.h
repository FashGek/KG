// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "KGHUD.generated.h"

/**
 * c++ 11 override 参考 http://www.cnblogs.com/yxwkf/p/3853229.html
 */
UCLASS(Config=Game)
class KINGSGUARD_API AKGHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:

	// hold hud by this controller, at the end of game it maybe play the women role
	class AKGPlayerController* KGPlayerOwner;

	// 当前活动的Wiget
	UPROPERTY(Transient)
	TArray<class UKGHUDWidget*> HudWigets;
	// 需要加载的Wiget字符串
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TArray<FString> HudWidgetClasses;	

	/** 小/中/大字体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  FONT)
	class UFont* SmallFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  FONT)
	class UFont* MediumFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FONT)
	class UFont* LargeFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FONT)
	class UFont* NumberFont;


	/** HUD 透明度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	float WidgetOpactiy;

	/** 从DefaultGame.ini里配置的HUD字符串 */
	UPROPERTY(Config=Game)
	TArray<FString> RequiredHudWidgetClasses;

	/** 供BPHUD重写*/
	virtual void BeginPlay() /*override*/;

	virtual void PostInitializeComponents() /*override*/;
	virtual void ShowDebugInfo(float& YL, float& YPos) override;

	/** 从资源字符串中加载HUDWidget子类 */
	TSubclassOf<UKGHUDWidget> ResolveHudWidgetByName(const TCHAR* ResourceName);

	/** 创建添加HUDWidget类 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = HUD)
	virtual UKGHUDWidget* AddHudWidget(TSubclassOf<UKGHUDWidget> NewWidgetClass);

	virtual void PostRender() /*override*/;
	/** primary draw HUD */
	virtual void DrawHUD() override;

public:

	// hudwidgets 渲染时用的基本透明度
	UPROPERTY(globalconfig)
	float HUDWidgetOpacity;

	// hudwidgets 有边框时的透明度
	UPROPERTY(globalconfig)
	float HUDWidgetBorderOpacity;

	// hud widget 有slate背景的透明度
	UPROPERTY(globalconfig)
	float HUDWidgetSlateOpacity;

protected:

	/** 从字符串中反射构造出相应的类 */
	virtual void BuildHudWidget(FString NewWidgetString);

public:

	void DrawString(FText Text, float X, float Y, ETextHorzPos::Type HozAlignment, ETextVertPos::Type VertAlignment, UFont* font, FLinearColor Color, float Scale = 1.0, bool bOutline = false);
	void DrawNumber(int Number, float X, float Y, FLinearColor Color, float GlowOpacity, float Scale, int MinDigits = 0, bool bRightAlign = false);

};
