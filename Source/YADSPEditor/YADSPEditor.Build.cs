using UnrealBuildTool;

public class YADSPEditor : ModuleRules
{
    public YADSPEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "YADSP",
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
                "GraphEditor",
            }
        );
    }
}