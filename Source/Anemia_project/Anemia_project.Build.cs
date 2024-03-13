// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Anemia_project : ModuleRules
{
	public Anemia_project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
