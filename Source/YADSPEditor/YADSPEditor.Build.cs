// Copyright Tom Duby. All Rights Reserved.

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
				"YADSP",
				"GameActionPlugin",
				"GSheetLocSystemPlugin",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"AssetTools",
				"PropertyEditor",
				"Projects",
				"ToolMenus",
				"GraphEditor",
				"InputCore",
				"EditorFramework",
				"DeveloperSettings",
				"UMG",
			}
		);
	}
}