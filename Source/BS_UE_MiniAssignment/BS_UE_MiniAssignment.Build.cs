// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BS_UE_MiniAssignment : ModuleRules
{
	public BS_UE_MiniAssignment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
