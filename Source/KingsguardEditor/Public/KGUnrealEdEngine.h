// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "KGLoadMap.h"

#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "KGWorldSettings.h"
//#include "UTMenuGameMode.h"

#include "KGUnrealEdEngine.generated.h"

UCLASS(CustomConstructor)
class UKGUnrealEdEngine : public UUnrealEdEngine
{
	GENERATED_UCLASS_BODY()

	UKGUnrealEdEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	{}

	KG_LOADMAP_DEFINITION()
};
