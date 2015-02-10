// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KingsguardEditor : ModuleRules
{
    public KingsguardEditor(TargetInfo Target)
	{
        bFasterWithoutUnity = true;
        MinFilesUsingPrecompiledHeaderOverride = 1;

        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            // Hack for LINUXMessageBoxExt
            PublicDependencyModuleNames.AddRange(new string[] { "SDL2" });
        }

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd", "Slate", "SlateCore", "SlateRHIRenderer", "Kingsguard", "OnlineSubsystem", "OnlineSubsystemUtils", "UMG" });
	}
}
