// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HelloWorldNode : ModuleRules
{
    public HelloWorldNode(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
