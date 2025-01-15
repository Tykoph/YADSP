using UnrealBuildTool;

public class YADSPActorEd : ModuleRules
{
    public YADSPActorEd(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "YADSP",
                "YADSPActorEd"
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