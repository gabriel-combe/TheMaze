// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheMaze : ModuleRules
{
	public TheMaze(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"AImodule",
			"GameplayTasks",
			"NavigationSystem"
		});
	}
}
