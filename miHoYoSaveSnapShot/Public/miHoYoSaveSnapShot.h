// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Animation/AnimTypes.h"
#include "Animation/Skeleton.h"
#include "Components/SkeletalMeshComponent.h"
#include "miHoYoPoseSnapshot.h"

#include "miHoYoSaveSnapShot.generated.h"

/**
 *
 */
UCLASS()
class MIHOYOSAVESNAPSHOT_API UmiHoYoSaveSnapShot : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Pose")
	static void miHoYoSnapshotPose(UPARAM(ref) FmiHoYoPoseSnapshot& Snapshot, USkeletalMeshComponent* SkeletalMeshComponent);


};
