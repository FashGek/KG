// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "KGLoadMap.h"

#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "KGEditorEngine.generated.h"

UCLASS(CustomConstructor)
class UKGEditorEngine : public UEditorEngine
{
	GENERATED_UCLASS_BODY()

	UKGEditorEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	{}

	KG_LOADMAP_DEFINITION()
};
