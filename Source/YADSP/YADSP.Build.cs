// Copyright Epic Games, Inc. All Rights Reserved.
// Copyright Tom Duby. All Rights Reserved.

using UnrealBuildTool;

public class YADSP : ModuleRules
{
	public YADSP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"InputCore",
				"EnhancedInput",
				"UMG",
				"GSheetLocSystemPlugin",
				"GameActionPlugin"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
		);

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new []
			{
				"UnrealEd",
			});
		}
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}