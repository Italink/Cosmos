// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CosmosEditor : ModuleRules 
{
	public CosmosEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(new string[] {
			"Cosmos"
		});

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"RHI",
            "RenderCore",
            "Cosmos"
        });

		
		PrivateDependencyModuleNames.AddRange(new string[] {
			"Projects",
			"UnrealEd",
			"LevelEditor",
			"Slate",
			"SlateCore",
			"UMG",
			"UMGEditor",
			"ToolMenus",
			"Blutility",
			"XmlParser",
			"DesktopPlatform",
			"LandscapeEditor",
			"EditorScriptingUtilities",
			"Foliage",
			"WorldBrowser",
			"EditorStyle",
			"HairStrandsCore",
			"AssetRegistry",
            "GaussianSplattingEditor",
			"Json",
			"Niagara"
        });
	}
}
