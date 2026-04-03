#pragma once
#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "WAttributeModifierStruct.generated.h"



UENUM(BlueprintType)
enum class EModifierOperation : uint8
{
	Add =0 UMETA(DisplayName = "Idle"),
	Multiply = 1 UMETA(DisplayName = "Multiply"),
	Override = 2 UMETA(DisplayName = "Override")
};


USTRUCT(BlueprintType)
struct FWAttributeModifierStruct
{
	GENERATED_BODY()
	UPROPERTY()
	float Magnitude;
	UPROPERTY()
	float Duration;
	UPROPERTY()
	float Interval;//tick value
	UPROPERTY()
	EModifierOperation ModifierOperation;
	UPROPERTY()
	FGameplayTag ID;
	UPROPERTY()
	bool bIsActive;
	
	FWAttributeModifierStruct(float InMagnitude=0,
		float InDuration=0,
		float InInterval=0,
		EModifierOperation InModifierOperation=EModifierOperation::Add,
		FGameplayTag InID = FGameplayTag::EmptyTag,
		bool InbIsActive=false
		)
	: Magnitude(InMagnitude),
	Duration(InDuration),
	Interval(InInterval),
	ModifierOperation(InModifierOperation),
	bIsActive(InbIsActive)
	{
		
	}
	
	
	
	
	
	
};

