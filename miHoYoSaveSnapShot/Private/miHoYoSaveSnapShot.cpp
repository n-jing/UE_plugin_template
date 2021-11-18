// Fill out your copyright notice in the Description page of Project Settings.



#include "miHoYoSaveSnapShot.h"

#include "Animation/AnimInstance.h"



void UmiHoYoSaveSnapShot::miHoYoSnapshotPose(FmiHoYoPoseSnapshot& Snapshot, USkeletalMeshComponent* SkeletalMeshComponent)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	SkeletalMeshComponent->SnapshotPose(Snapshot.Pose);

	TArray<FName> CurveName;
	AnimInstance->GetAllCurveNames(CurveName);

	
	USkeleton* MySkeleton = SkeletalMeshComponent->SkeletalMesh->Skeleton;
	const FSmartNameMapping* CurveNameMapping = MySkeleton->GetSmartNameContainer(USkeleton::AnimCurveMappingName);
	
	TArray<float> CurveValue;
	for (auto& n : CurveName)
	{
		const USkeleton::AnimCurveUID CurveUID = CurveNameMapping->FindUID(n);
		float v = AnimInstance->GetCurveValue(n);
		Snapshot.BSChannel.Add(CurveUID, v); 

		CurveValue.Add(v);
	}

	FString CurveStr;
	for (auto& v : CurveValue)
	{
		CurveStr += FString::Printf(TEXT("%f, "), v);
	}

	UE_LOG(LogTemp, Log, TEXT("curve num: %d\n curve value: %s"), Snapshot.BSChannel.Num(), *CurveStr);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("curve num: %d\n curve value: %s"), Snapshot.BSChannel.Num(), *CurveStr));
}