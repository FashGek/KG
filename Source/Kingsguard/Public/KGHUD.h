// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "KGHUD.generated.h"

/**
 * c++ 11 override �ο� http://www.cnblogs.com/yxwkf/p/3853229.html
 */
UCLASS(Config=Game)
class KINGSGUARD_API AKGHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:

	// hold hud by this controller, at the end of game it maybe play the women role
	class AKGPlayerController* KGPlayerOwner;

	// ��ǰ���Wiget
	UPROPERTY(Transient)
	TArray<class UKGHUDWidget*> HudWigets;
	// ��Ҫ���ص�Wiget�ַ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TArray<FString> HudWidgetClasses;	

	/** С/��/������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  FONT)
	class UFont* SmallFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  FONT)
	class UFont* MediumFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FONT)
	class UFont* LargeFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FONT)
	class UFont* NumberFont;


	/** HUD ͸���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	float WidgetOpactiy;

	/** ��DefaultGame.ini�����õ�HUD�ַ��� */
	UPROPERTY(Config=Game)
	TArray<FString> RequiredHudWidgetClasses;

	/** ��BPHUD��д*/
	virtual void BeginPlay() /*override*/;

	virtual void PostInitializeComponents() /*override*/;
	virtual void ShowDebugInfo(float& YL, float& YPos) override;

	/** ����Դ�ַ����м���HUDWidget���� */
	TSubclassOf<UKGHUDWidget> ResolveHudWidgetByName(const TCHAR* ResourceName);

	/** �������HUDWidget�� */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = HUD)
	virtual UKGHUDWidget* AddHudWidget(TSubclassOf<UKGHUDWidget> NewWidgetClass);

	virtual void PostRender() /*override*/;
	/** primary draw HUD */
	virtual void DrawHUD() override;

public:

	// hudwidgets ��Ⱦʱ�õĻ���͸����
	UPROPERTY(globalconfig)
	float HUDWidgetOpacity;

	// hudwidgets �б߿�ʱ��͸����
	UPROPERTY(globalconfig)
	float HUDWidgetBorderOpacity;

	// hud widget ��slate������͸����
	UPROPERTY(globalconfig)
	float HUDWidgetSlateOpacity;

protected:

	/** ���ַ����з��乹�����Ӧ���� */
	virtual void BuildHudWidget(FString NewWidgetString);

public:

	void DrawString(FText Text, float X, float Y, ETextHorzPos::Type HozAlignment, ETextVertPos::Type VertAlignment, UFont* font, FLinearColor Color, float Scale = 1.0, bool bOutline = false);
	void DrawNumber(int Number, float X, float Y, FLinearColor Color, float GlowOpacity, float Scale, int MinDigits = 0, bool bRightAlign = false);

};
