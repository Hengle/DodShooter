// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dod : ModuleRules
{
	public Dod(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ModularGameplayActors", "LoadingScreen",
			"ControlFlows", "CommonUI", "CommonGame", "GameplayTags", "UMG", "ModularGameplay", "EnhancedInput",
			"GameplayAbilities", "AnimGraphRuntime"
		});
	}
}