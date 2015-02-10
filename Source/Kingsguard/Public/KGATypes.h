/************************************************************************/
/* author: xlt                                                          */
/************************************************************************/

#pragma once


#include "KGATypes.generated.h"

UENUM()
namespace ETextHorzPos
{
	enum Type
	{
		Left,
		Center,
		Right,
		MAX
	};
}

UENUM()
namespace ETextVertPos
{
	enum Type
	{
		Top,
		Center,
		Bottom,
		MAX
	};
}

// UVs
USTRUCT()
struct FTextureUVs
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureUVs")
	float U;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureUVs")
	float V;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureUVs")
	float UL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureUVs")
	float VL;

	FTextureUVs() :
		U(0.0f),
		V(0.0f),
		UL(0.0f),
		VL(0.0f)
	{};

	FTextureUVs(float inU, float inV, float inUL, float inVL)
	{
		U = inU; V = inV; UL = inUL; VL = inUL;
	}
};

// 可渲染对像
USTRUCT(BlueprintType)
struct FHUDRenderObject
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	bool bHidden;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	float RenderPriority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	FVector2D Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	FVector2D Size;

	// 字体颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	FLinearColor RenderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	float RenderOpacity;

	FHUDRenderObject()
	{
		RenderPriority = 0.0f;
		RenderColor = FLinearColor::White;
		RenderOpacity = 1.0f;
	};

public:
	virtual float GetWidth() { return Size.X; }
	virtual float GetHeight() { return Size.Y; }
};

USTRUCT(BlueprintType)
struct FHUDRenderObject_Texutre : public FHUDRenderObject
{
	GENERATED_USTRUCT_BODY()

	// 材质
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	UTexture* Atlas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	FTextureUVs UVs;

	// 偏移量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	FVector2D RenderOffset;
	
	// 旋转角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	float Rotation;

	// 绕着哪一个点旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderObject")
	FVector2D RotPivot;

	FHUDRenderObject_Texutre() : FHUDRenderObject()
	{
		Atlas = NULL;
		Rotation = 0.0f;
	}

public:
	virtual float GetWidth()
	{
		return (Size.X <= 0) ? UVs.UL : Size.X;
	}

	virtual float GetHeight()
	{
		return (Size.Y <= 0) ? UVs.VL : Size.Y;
	}
};

USTRUCT(BlueprintType)
struct FHUDRenderObject_Text : public FHUDRenderObject
{
	GENERATED_USTRUCT_BODY()
};
