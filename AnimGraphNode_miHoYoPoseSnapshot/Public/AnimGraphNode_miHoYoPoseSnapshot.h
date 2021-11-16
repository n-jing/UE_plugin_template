// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AnimGraphNode_Base.h"
#include "AnimGraphNode_RefPoseBase.h"
#include "AnimNode_miHoYoPoseSnapshot.h"
#include "AnimGraphNode_miHoYoPoseSnapshot.generated.h"

UCLASS(MinimalAPI)
class UAnimGraphNode_miHoYoPoseSnapshot : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditAnywhere, Category = Settings)
		FAnimNode_miHoYoPoseSnapshot Node;

	// UEdGraphNode interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// End of UEdGraphNode interface
};
