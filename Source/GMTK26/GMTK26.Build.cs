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

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GMTK26",
			"GMTK26/AI",
			"GMTK26/AbilitySystem",
			"GMTK26/Variant_Platforming",
			"GMTK26/Variant_Platforming/Animation",
			"GMTK26/Variant_Combat",
			"GMTK26/Variant_Combat/AI",
			"GMTK26/Variant_Combat/Animation",
			"GMTK26/Variant_Combat/Gameplay",
			"GMTK26/Variant_Combat/Interfaces",
			"GMTK26/Variant_Combat/UI",
			"GMTK26/Variant_SideScrolling",
			"GMTK26/Variant_SideScrolling/AI",
			"GMTK26/Variant_SideScrolling/Gameplay",
			"GMTK26/Variant_SideScrolling/Interfaces",
			"GMTK26/Variant_SideScrolling/UI",
			"GMTK26/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
