// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HelloWorldNodeEditor : ModuleRules
{
    public HelloWorldNodeEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
        PrivateDependencyModuleNames.AddRange(new string[] {
            "UnrealEd",
            "Slate",
            "SlateCore",
            "EditorStyle",
            "GraphEditor",
            "BlueprintGraph",
            "KismetCompiler",
            "HelloWorldNode"
        });
    }
}
