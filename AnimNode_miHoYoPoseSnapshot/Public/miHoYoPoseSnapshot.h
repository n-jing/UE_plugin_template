// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/PoseSnapshot.h"
#include "Animation/AnimationAsset.h"
#include "miHoYoPoseSnapshot.generated.h"

/** A custom pose for a skeletal mesh */
USTRUCT(BlueprintType)
struct ANIMNODE_MIHOYOPOSESNAPSHOT_API FmiHoYoPoseSnapshot
{
	GENERATED_BODY()

public:
	FmiHoYoPoseSnapshot()
	{}

	FPoseSnapshot Pose;
	/** Array of transforms per-bone */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "miHoYoSnapshot")
		TArray<float> BSChannel;
};
