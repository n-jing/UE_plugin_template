// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
public class miHoYoSaveSnapShot : ModuleRules
{
    public miHoYoSaveSnapShot(ReadOnlyTargetRules Target) : base(Target)
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
            "ToolMenus",
            "AnimNode_miHoYoPoseSnapshot"
            });
    }
}
