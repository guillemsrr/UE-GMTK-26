// Copyright (c) Noümen Interactive. All Rights Reserved.

using UnrealBuildTool;

public class GMTK26 : ModuleRules
{
	public GMTK26(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore"
		});

		PublicIncludePaths.AddRange(new string[] {
			"GMTK26",
		});
	}
}
