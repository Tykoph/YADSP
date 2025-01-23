// Copyright 2025 Tom Duby. All Rights Reserved.

using UnrealBuildTool;

public class YADSPEditor : ModuleRules
{
	public YADSPEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"YADSP"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"YADSP",
				"UnrealEd",
				"AssetTools",
				"PropertyEditor",
				"Projects",
				"ToolMenus",
				"GraphEditor"
			}
		);
	}
}