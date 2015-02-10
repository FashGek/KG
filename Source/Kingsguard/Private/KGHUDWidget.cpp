
#include "Kingsguard.h"


UKGHUDWidget::UKGHUDWidget(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Opacity = 1.0f;
	Origin = FVector2D(0.0f, 0.0f);
	ScreenPosition = FVector2D(0.0f, 0.0f);

}

FVector2D UKGHUDWidget::DrawText(FText Text, float X, float Y, UFont* Font, float TextScale /*= 1.0*/, float DrawOpacity /*= 1.0f*/, FLinearColor DrawColor /*= FLinearColor::White*/, ETextHorzPos::Type TextHorzAligment /*= ETextHorzPos::Left*/, ETextVertPos::Type TextVertAligment /*= ETextVertPos::Top*/)
{
	return DrawText(Text, X, Y, Font, false, FVector2D::ZeroVector, FLinearColor::Black, false, FLinearColor::Black, TextScale, DrawOpacity, DrawColor, TextHorzAligment, TextVertAligment);
}

FVector2D UKGHUDWidget::DrawText(FText Text, float X, float Y, UFont* Font, bool bDrawShadow /*= false*/, FVector2D ShadowDirection /*= FVector2D(0, 0)*/, FLinearColor ShadowColor /*= FLinearColor::Black*/, bool bDrawOutline /*= false*/, FLinearColor OutlineColor /*= FLinearColor::Black*/, float TextScale /*= 1.0*/, float DrawOpacity /*= 1.0*/, FLinearColor DrawColor /*= FLinearColor::White*/, ETextHorzPos::Type TextHorzAligment /*= ETextHorzPos::Left*/, ETextVertPos::Type TextVertAligment /*= ETextVertPos::Top */, const FFontRenderInfo& RenderInfo)
{
	float XL = 0.0f, YL = 0.0f;
	
	if (Font && !Text.IsEmpty())
	{
		Canvas->StrLen(Font, Text.ToString(), XL, YL);

		FVector2D RenderPos = FVector2D(RenderPosition.X + XL, RenderPosition.Y + YL);

		DrawColor.A = Opacity * DrawOpacity * KGHUDOwner->WidgetOpactiy;
		Canvas->DrawColor = DrawColor.ToFColor(false);

		FCanvasTextItem TextItem(RenderPos, Text, Font, DrawColor);
		TextItem.FontRenderInfo = RenderInfo;

		Canvas->DrawItem(TextItem);
	}

	return FVector2D(XL, YL);
}

UCanvas* UKGHUDWidget::GetCanvas()
{
	return Canvas;
}

FVector2D UKGHUDWidget::GetRenderPosition()
{
	return RenderPosition;
}

FVector2D UKGHUDWidget::GetRenderSize()
{
	return RenderSize;
}

float UKGHUDWidget::GetRenderScale()
{
	return RenderScale;
}

bool UKGHUDWidget::IsHidden()
{
	return bHidden;
}

void UKGHUDWidget::PreDraw(float DeltaTime, AKGHUD* InKGHUDOwner, UCanvas* InCanvas, FVector2D InCanvasCenter)
{
	KGHUDOwner = InKGHUDOwner;

	if (KGHUDOwner != NULL && KGHUDOwner->KGPlayerOwner != NULL)
	{
		KGPlayerOwner = KGHUDOwner->KGPlayerOwner;
		if (KGPlayerOwner != NULL)
		{
			KGCharacterOwner = Cast<AKGCharacter>(KGPlayerOwner->GetViewTarget());
		}
	}
	Canvas = InCanvas;
	CanvasCenter = InCanvasCenter;
}

void UKGHUDWidget::Draw_Implementation(float DeltaTime)
{
	DrawAllRenderObjects(DeltaTime, FVector2D(0, 0));
}

void UKGHUDWidget::PostDraw(float RenderedTime)
{
	// 清理数据
	LastRenderTime = RenderedTime;
	Canvas = NULL;
	KGPlayerOwner = NULL;
	KGCharacterOwner = NULL;
}

FLinearColor UKGHUDWidget::ApplyHUDColor(FLinearColor DrawColor)
{

	return DrawColor;
}

void UKGHUDWidget::SetHidden(bool bIsHidden)
{
	bHidden = bIsHidden;
}

void UKGHUDWidget::InitializeWidget(AKGHUD* Hud)
{
	for (TFieldIterator<UStructProperty> PropIt(GetClass()); PropIt; ++PropIt)
	{
		UStructProperty* Prop = NULL;
		float PropRenderPriority = 0.0;

		FString CPPType = PropIt->Struct->GetName();
		if (CPPType == TEXT("HUDRenderObject_Texture"))
		{
			Prop = *PropIt;
			FHUDRenderObject_Texutre* PtrToProperty = Prop->ContainerPtrToValuePtr<FHUDRenderObject_Texutre>(this, 0);
			if (PtrToProperty)
			{
				PropRenderPriority = PtrToProperty->RenderPriority;
			}
		}
		else if (CPPType == TEXT("HUDRenderObject_Text"))
		{
			Prop = *PropIt;
			FHUDRenderObject_Text* PtrToProperty = Prop->ContainerPtrToValuePtr<FHUDRenderObject_Text>(this, 0);
			if (PtrToProperty)
			{
				PropRenderPriority = PtrToProperty->RenderPriority;
			}
		}

		if (Prop)
		{
			int32 InsertPosition = -1;
			for (int32 i = 0; i < RenderObjectList.Num(); i++)
			{
				FHUDRenderObject* ObjPtr = RenderObjectList[i]->ContainerPtrToValuePtr<FHUDRenderObject>(this, 0);
				if (ObjPtr)
				{
					if (PropRenderPriority < ObjPtr->RenderPriority)
					{
						InsertPosition = i;
						break;
					}
				}
			}

			if (InsertPosition > 0)
			{
				RenderObjectList.Insert(Prop, InsertPosition);
			}
			else
			{
				RenderObjectList.Add(Prop);
			}
		}
	}
}



bool UKGHUDWidget::ShouldDraw_Implementation()
{
	return true;
}

void UKGHUDWidget::DrawAllRenderObjects(float DeltaTime, FVector2D DrawOffset)
{
	for (int32 i = 0; i < RenderObjectList.Num(); i++)
	{
		UStructProperty* Prop = RenderObjectList[i];
		FString CPPType = Prop->Struct->GetName();
		if (CPPType == TEXT("HUDRenderObject_Texture"))
		{
			FHUDRenderObject_Texutre* PtrToTexture = RenderObjectList[i]->ContainerPtrToValuePtr<FHUDRenderObject_Texutre>(this, 0);
			if (PtrToTexture)
			{
				RenderObj_Texture(*PtrToTexture, DrawOffset);
			}
		}
		else if (CPPType == TEXT("HUDRenderObject_Text"))
		{
			FHUDRenderObject_Text* PtrToText = RenderObjectList[i]->ContainerPtrToValuePtr<FHUDRenderObject_Text>(this, 0);
			if (PtrToText)
			{
				RenderObj_Text(*PtrToText, DrawOffset);
			}
		}
	}
}

void UKGHUDWidget::RenderObj_Texture(FHUDRenderObject_Texutre& TextureObject, FVector2D DrawOffset /*= FVector2D(0, 0)*/)
{
	FVector2D RenderSize = FVector2D(TextureObject.GetWidth(), TextureObject.GetHeight());
	RenderObj_TextureAt(TextureObject, (TextureObject.Position.X + DrawOffset.X), (TextureObject.Position.Y + DrawOffset.Y), RenderSize.X, RenderSize.Y);
}

void UKGHUDWidget::RenderObj_TextureAt(FHUDRenderObject_Texutre& TextureObject, float X, float Y, float Width, float Height)
{
	if (TextureObject.bHidden || TextureObject.Atlas == NULL || Width < 0.0 || Height < 0.0)
	{
		return;
	}

	FLinearColor RenderColor = TextureObject.RenderColor;
	float Opacity = KGHUDOwner->HUDWidgetOpacity; // 这里是几个透明度的叠加

	DrawTexture(TextureObject.Atlas,
					X,
					Y,
					Width,
					Height,
					TextureObject.UVs.U, TextureObject.UVs.V, TextureObject.UVs.UL, TextureObject.UVs.VL,
					TextureObject.RenderOpacity * Opacity,
					RenderColor,
					TextureObject.RenderOffset,
					TextureObject.Rotation,
					TextureObject.RotPivot);
}

void UKGHUDWidget::RenderObj_Text(FHUDRenderObject_Text& TextObject, FVector2D DrawOffset /*= FVector2D(0, 0)*/)
{

}

void UKGHUDWidget::RenderObjt_TextAt(FHUDRenderObject_Text& TextObject, float X, float Y)
{

}

void UKGHUDWidget::DrawTexture(UTexture* Texture, float X, float Y, float Width, float Height, float U /*= 0.0f*/, float V /*= 0.0f*/, float UL /*= 1.0f*/, float VL /*= 1.0f*/, float DrawOpactiy /*= 1.0f*/, FLinearColor DrawColor /*= FLinearColor::White*/, FVector2D RenderOffset /*= FVector2D(0.0f, 0.0f)*/, float Rotation /*= 0*/, FVector2D RotPivot /*= FVector2D(0.5f, 0.5f)*/)
{
	float ImageAspectScale = Height > 0 ? Width / Height : 0.0f;

	if (Texture && Texture->Resource)
	{
		FVector2D RenderPos = FVector2D(RenderPosition.X + X - (Width * RenderOffset.X), RenderPosition.Y + Y - (Height * RenderOffset.Y));

		U = U / Texture->Resource->GetSizeX();
		V = V / Texture->Resource->GetSizeY();
		UL = U + (UL / Texture->Resource->GetSizeX());
		VL = V + (VL / Texture->Resource->GetSizeY());

		DrawColor.A = Opacity * DrawOpactiy * KGHUDOwner->WidgetOpactiy;

		FCanvasTileItem ImageItem(RenderPos, Texture->Resource, FVector2D(Width, Height), FVector2D(U, V), FVector2D(UL, VL), DrawColor);
		ImageItem.Rotation = FRotator(0, Rotation, 0);
		ImageItem.PivotPoint = RotPivot;
		ImageItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;
		Canvas->DrawItem(ImageItem);
	}
}

void UKGHUDWidget::DrawMaterial(UMaterialInstance* Material, float X, float Y, float Width, float Height, float MaterialU /*= 0.0f*/, float MaterialV /*= 0.0f*/, float MaterialUL /*= 1.0f*/, float MaterialVL /*= 1.0f*/, float DrawOpactiy /*= 1.0f*/, FLinearColor DrawColor /*= FLinearColor::White*/, FVector2D RenderOffset /*= FVector2D(0.0f, 0.0f)*/, float Rotation /*= 0*/, FVector2D RotPivot /*= FVector2D(0.5f, 0.5f)*/)
{

}
