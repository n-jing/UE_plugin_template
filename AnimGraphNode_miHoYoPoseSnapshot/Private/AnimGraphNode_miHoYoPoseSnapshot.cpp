// Copyright Epic Games, Inc. All Rights Reserved.UAnimGraphNode_miHoYoPoseSnapshot

#include "AnimGraphNode_miHoYoPoseSnapshot.h"

/////////////////////////////////////////////////////
// UAnimGraphNode_LocalCachePose

#define LOCTEXT_NAMESPACE "miHoYoPoseSnapshot"

UAnimGraphNode_miHoYoPoseSnapshot::UAnimGraphNode_miHoYoPoseSnapshot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UAnimGraphNode_miHoYoPoseSnapshot::GetTooltipText() const
{
	return LOCTEXT("UAnimGraphNode_miHoYoPoseSnapshot_Tooltip", "Returns miHoYo local space pose snapshot.");
}

FText UAnimGraphNode_miHoYoPoseSnapshot::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("UAnimGraphNode_miHoYoPoseSnapshot_Title", "miHoYo Pose Snapshot");
}

#undef LOCTEXT_NAMESPACE
