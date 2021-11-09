// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloWorld.h"
#include "Engine.h"

void UHelloWorld::SayHello_Internal() {
	if (GEngine) {
		const int32 AlwaysAddKey = -1;

		GEngine->AddOnScreenDebugMessage(AlwaysAddKey,
			5.0f,
			FColor::Purple,
			TEXT("Hello World!")
		);
	}
}

void UHelloWorld::SaySomething_Internal(const TArray<FString>& InWords)
{
	FString OutString(TEXT("SaySomething Called:"));
	for (const auto& Word : InWords)
		OutString += Word;

	if (GEngine) {
		const int32 AlwaysAddKey = -1;

		GEngine->AddOnScreenDebugMessage(AlwaysAddKey,
			5.0f,
			FColor::Purple,
			OutString
		);
	}
}
