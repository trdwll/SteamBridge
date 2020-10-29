// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

using UnrealBuildTool;

public class SteamBridgeCore : ModuleRules
{
	public SteamBridgeCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "SteamBridge",
            "Steamworks"
        });
    }
}
