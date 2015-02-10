
#pragma  once

#include "KGATypes.h"
#include "KGHUDWidget.generated.h"

const float WIDGET_DEFAULT_Y_RESOLUTION = 1080;		// 1080P


// 前置声明
class AKGHUD;

UCLASS(BlueprintType, Blueprintable)
class KINGSGUARD_API UKGHUDWidget : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	// Widget相对于屏幕及原点的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	FVector2D Position;

	// Wdiget 大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	FVector2D Size;

	// 位置是相对于原点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wigget")
	FVector2D Origin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	FVector2D ScreenPosition;

	// 透明度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	float Opacity;

	virtual void InitializeWidget(AKGHUD* Hud);

	virtual bool IsHidden();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	virtual void SetHidden(bool bIsHidden);

	UFUNCTION(BlueprintNativeEvent)
	bool ShouldDraw();

	// draw 的三大阶段
	/**
	 * @param DeltaTime 这是上一次绘制的时间差
	 **/
	virtual void PreDraw(float DeltaTime, AKGHUD* InKGHUDOwner, UCanvas* InCanvas, FVector2D InCanvasCenter);

	UFUNCTION(BlueprintNativeEvent)
	void Draw(float DeltaTime);

	virtual void PostDraw(float RenderTime);

	// 持有此Widget的HUD
	UPROPERTY(BlueprintReadOnly, Category = "Widget Live")
	class AKGHUD* KGHUDOwner;

	// 引用拥有此widget的player controller 
	UPROPERTY(BlueprintReadOnly, Category = "Widget Live")
	class AKGPlayerController* KGPlayerOwner;
	
	// 引用拥有此widget的character 
	UPROPERTY(BlueprintReadOnly, Category = "Widget Live")
	class AKGCharacter* KGCharacterOwner;


	virtual UCanvas* GetCanvas();
	virtual FVector2D GetRenderPosition();
	virtual FVector2D GetRenderSize();
	virtual float GetRenderScale();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	virtual FLinearColor ApplyHUDColor(FLinearColor DrawColor);

protected:

	// if true will not render this widget
	UPROPERTY(BlueprintReadOnly, Category = "Widget Live")
	uint32 bHidden : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Live")
	float LastRenderTime;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets Live")
	FVector2D RenderPosition;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Live")
	class UCanvas* Canvas;

	UPROPERTY(BlueprintReadOnly, Category = "Wdiget Live")
	FVector2D RenderSize;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Live")
	float RenderScale;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Live")
	FVector2D CanvasCenter;

public:

	UFUNCTION(BlueprintCallable, Category = "Widgets", Meta = (AutoCreateRefTerm = "RenderInfo"))
	FVector2D DrawText(FText Text, float X, float Y, UFont* Font, float TextScale = 1.0, float DrawOpacity = 1.0f, FLinearColor DrawColor = FLinearColor::White, ETextHorzPos::Type TextHorzAligment = ETextHorzPos::Left, ETextVertPos::Type TextVertAligment = ETextVertPos::Top);

	FVector2D DrawText(FText Text, float X, float Y, UFont* Font, bool bDrawShadow = false, FVector2D ShadowDirection = FVector2D(0, 0), FLinearColor ShadowColor = FLinearColor::Black, bool bDrawOutline = false, FLinearColor OutlineColor = FLinearColor::Black, float TextScale = 1.0, float DrawOpacity = 1.0, FLinearColor DrawColor = FLinearColor::White, ETextHorzPos::Type TextHorzAligment = ETextHorzPos::Left, ETextVertPos::Type TextVertAligment = ETextVertPos::Top
		, const FFontRenderInfo& RenderInfo
#if CPP
		= FFontRenderInfo()
#endif
		);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	virtual void DrawTexture(UTexture* Texture, float X, float Y, float Width, float Height, float MaterialU = 0.0f, float MaterialV = 0.0f, float MaterialUL = 1.0f, float MaterialVL = 1.0f, float DrawOpactiy = 1.0f, FLinearColor DrawColor = FLinearColor::White, FVector2D RenderOffset = FVector2D(0.0f, 0.0f), float Rotation = 0, FVector2D RotPivot = FVector2D(0.5f, 0.5f));

	UFUNCTION(BlueprintCallable, Category = "Widget")
	virtual void DrawMaterial(UMaterialInstance* Material, float X, float Y, float Width, float Height, float MaterialU = 0.0f, float MaterialV = 0.0f, float MaterialUL = 1.0f, float MaterialVL = 1.0f, float DrawOpactiy = 1.0f, FLinearColor DrawColor = FLinearColor::White, FVector2D RenderOffset = FVector2D(0.0f, 0.0f), float Rotation = 0, FVector2D RotPivot = FVector2D(0.5f, 0.5f));


private:

	UFUNCTION(BlueprintCallable, Category = "Render Objects")
	virtual void DrawAllRenderObjects(float DeltaTime, FVector2D DrawOffset);

	UFUNCTION(BlueprintCallable, Category = "Render Objects")
	virtual void RenderObj_Texture(FHUDRenderObject_Texutre& TextureObject, FVector2D DrawOffset = FVector2D(0, 0));

	UFUNCTION(BlueprintCallable, Category = "Render Objects")
	virtual void RenderObj_TextureAt(FHUDRenderObject_Texutre& TextureObject, float X, float Y, float Width, float Height);

	UFUNCTION(BlueprintCallable, Category = "Render Objects")
	virtual void RenderObj_Text(FHUDRenderObject_Text& TextObject, FVector2D DrawOffset = FVector2D(0, 0));

	UFUNCTION(BlueprintCallable, Category = "Render Objects")
	virtual void RenderObjt_TextAt(FHUDRenderObject_Text& TextObject, float X, float Y);

	UPROPERTY()
	TArray<UStructProperty*> RenderObjectList;	// 该Widget需要渲染的元素
};
