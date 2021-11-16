
// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimNode_miHoYoPoseSnapshot.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimTrace.h"

/////////////////////////////////////////////////////
// FAnimNode_miHoYoPoseSnapshot

FAnimNode_miHoYoPoseSnapshot::FAnimNode_miHoYoPoseSnapshot()
	: SnapshotName(NAME_None)
	, Mode(EmiHoYoSnapshotSourceMode::NamedSnapshot)
	, MappedSourceMeshName(NAME_None)
	, MappedTargetMeshName(NAME_None)
	, TargetBoneNameMesh(NAME_None)
{
}


void FAnimNode_miHoYoPoseSnapshot::PreUpdate(const UAnimInstance* InAnimInstance)
{
	// cache the currently used skeletal mesh's bone names
	USkeletalMesh* CurrentSkeletalMesh = nullptr;
	if (InAnimInstance->GetSkelMeshComponent() && InAnimInstance->GetSkelMeshComponent()->IsRegistered())
	{
		CurrentSkeletalMesh = InAnimInstance->GetSkelMeshComponent()->SkeletalMesh;
	}

	if (CurrentSkeletalMesh)
	{
		if (TargetBoneNameMesh != CurrentSkeletalMesh->GetFName())
		{
			// cache bone names for the target mesh
			TargetBoneNames.Reset();
			TargetBoneNames.AddDefaulted(CurrentSkeletalMesh->RefSkeleton.GetNum());

			for (int32 BoneIndex = 0; BoneIndex < CurrentSkeletalMesh->RefSkeleton.GetNum(); ++BoneIndex)
			{
				TargetBoneNames[BoneIndex] = CurrentSkeletalMesh->RefSkeleton.GetBoneName(BoneIndex);
			}
		}

		TargetBoneNameMesh = CurrentSkeletalMesh->GetFName();
	}
	else
	{
		TargetBoneNameMesh = NAME_Name;
	}
}

void FAnimNode_miHoYoPoseSnapshot::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread)
	// Evaluate any BP logic plugged into this node
	GetEvaluateGraphExposedInputs().Execute(Context);

	TRACE_ANIM_NODE_VALUE(Context, TEXT("Snapshot Name"), Snapshot.SnapshotName);
}

void FAnimNode_miHoYoPoseSnapshot::Evaluate_AnyThread(FPoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Evaluate_AnyThread)
	FCompactPose& OutPose = Output.Pose;
	OutPose.ResetToRefPose();

	switch (Mode)
	{
	case EmiHoYoSnapshotSourceMode::NamedSnapshot:
	{
		const FPoseSnapshot test = *(Output.AnimInstanceProxy->GetPoseSnapshot(SnapshotName));
		bool is_same_type = std::is_same<decltype(test), const FmiHoYoPoseSnapshot>::value;
//		if (test->IsChildOf<FmiHoYoPoseSnapshot::StaticStruct()>())
		{

		}

		const FmiHoYoPoseSnapshot* PoseSnapshotData = static_cast<const FmiHoYoPoseSnapshot*>((Output.AnimInstanceProxy->GetPoseSnapshot(SnapshotName)));
		//const FmiHoYoPoseSnapshot* PoseSnapshotDataT = CastChecked<const FmiHoYoPoseSnapshot>(Output.AnimInstanceProxy->GetPoseSnapshot(SnapshotName));
		//if (const FmiHoYoPoseSnapshot* PoseSnapshotData = con_cast<const FmiHoYoPoseSnapshot*>(Output.AnimInstanceProxy->GetPoseSnapshot(SnapshotName)))
		{
			ApplyPose(*PoseSnapshotData, OutPose);
		}
	}
	break;
	case EmiHoYoSnapshotSourceMode::SnapshotPin:
	{
		ApplyPose(Snapshot, OutPose);
	}
	break;
	}
}

void FAnimNode_miHoYoPoseSnapshot::ApplyPose(const FmiHoYoPoseSnapshot& PoseSnapshot, FCompactPose& OutPose)
{
	const TArray<FTransform>& LocalTMs = PoseSnapshot.LocalTransforms;
	const FBoneContainer& RequiredBones = OutPose.GetBoneContainer();

	if (TargetBoneNameMesh == PoseSnapshot.SkeletalMeshName)
	{
		for (FCompactPoseBoneIndex PoseBoneIndex : OutPose.ForEachBoneIndex())
		{
			const FMeshPoseBoneIndex MeshBoneIndex = RequiredBones.MakeMeshPoseIndex(PoseBoneIndex);
			const int32 Index = MeshBoneIndex.GetInt();

			if (LocalTMs.IsValidIndex(Index))
			{
				OutPose[PoseBoneIndex] = LocalTMs[Index];
			}
		}
	}
	else
	{
		// per-bone matching required
		CacheBoneMapping(PoseSnapshot.SkeletalMeshName, TargetBoneNameMesh, PoseSnapshot.BoneNames, TargetBoneNames);

		for (FCompactPoseBoneIndex PoseBoneIndex : OutPose.ForEachBoneIndex())
		{
			const FMeshPoseBoneIndex MeshBoneIndex = RequiredBones.MakeMeshPoseIndex(PoseBoneIndex);
			const int32 Index = MeshBoneIndex.GetInt();

			if (SourceBoneMapping.IsValidIndex(Index))
			{
				const int32 SourceBoneIndex = SourceBoneMapping[Index];
				if (LocalTMs.IsValidIndex(SourceBoneIndex))
				{
					OutPose[PoseBoneIndex] = LocalTMs[SourceBoneIndex];
				}
			}
		}
	}
}

void FAnimNode_miHoYoPoseSnapshot::GatherDebugData(FNodeDebugData& DebugData)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)
	FString DebugLine = DebugData.GetNodeName(this) + " Snapshot Name:" + SnapshotName.ToString();
	DebugData.AddDebugItem(DebugLine, true);
}

void FAnimNode_miHoYoPoseSnapshot::CacheBoneMapping(FName InSourceMeshName, FName InTargetMeshName, const TArray<FName>& InSourceBoneNames, const TArray<FName>& InTargetBoneNames)
{
	if (InSourceMeshName != MappedSourceMeshName || InTargetMeshName != MappedTargetMeshName)
	{
		SourceBoneMapping.Reset();

		for (int32 BoneIndex = 0; BoneIndex < InTargetBoneNames.Num(); ++BoneIndex)
		{
			SourceBoneMapping.Add(InSourceBoneNames.IndexOfByKey(InTargetBoneNames[BoneIndex]));
		}

		MappedSourceMeshName = InSourceMeshName;
		MappedTargetMeshName = InTargetMeshName;
	}
}