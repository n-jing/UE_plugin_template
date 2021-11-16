// Fill out your copyright notice in the Description page of Project Settings.



#include "miHoYoSaveSnapShot.h"

#include "Animation/AnimInstance.h"



void UmiHoYoSaveSnapShot::miHoYoSnapshotPose(FmiHoYoPoseSnapshot& Snapshot, USkeletalMeshComponent* SkeletalMeshComponent)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	SkeletalMeshComponent->SnapshotPose(Snapshot.Pose);

	TArray<FName> CurveName;
	AnimInstance->GetAllCurveNames(CurveName);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("curve num: %d"), CurveName.Num()));

}