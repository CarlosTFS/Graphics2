// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSProject : ModuleRules
{
	public FPSProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Add UMG module dependency for UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
    }
}
