// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

using UnrealBuildTool;

public class SteamBridge : ModuleRules
{
	public SteamBridge(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Steamworks"
        });

        AddEngineThirdPartyPrivateStaticDependencies(Target, "Steamworks");

        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Settings", "Projects" });
        }
    }
}
