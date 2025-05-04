// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class Cosmos : ModuleRules
{
	public Cosmos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[]
        {
			"Cosmos",
        });
       
        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject",
			"Engine", 
			"InputCore",
			"EnhancedInput",
			"UMG",
            "Json",
			"HTTP",
            "WebSockets",
            "SignalProcessing",
            "LevelSequence",
            "MovieScene"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
		    "WidgetCard"
        });

        AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
