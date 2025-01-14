using UnrealBuildTool;

public class YADSPActorEditor : ModuleRules
{
	public YADSPActorEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"YADSP",
				"YADSPActorEditor"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
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
				"GraphEditor",
			}
		);
	}
}