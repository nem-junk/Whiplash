#pragma once
#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "FWAttributeModifier.generated.h"



UENUM(BlueprintType)
enum class EModifierOperation : uint8
{
	Add =0 UMETA(DisplayName = "Add (Base)"),
	Multiply = 1 UMETA(DisplayName = "Multiply"),
	Override = 2 UMETA(DisplayName = "Override")
};


USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()
	
	float Magnitude;
	
	float Duration;
	
	float Interval;//tick value
	
	EModifierOperation ModifierOperation;
	
	FGameplayTag ID;//remove in future
	
	FAttributeModifier(float InMagnitude=0,
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
	ID(InID)
	{
		
	}
	
};
USTRUCT(BlueprintType)
struct FModifierTimerHandle
{
	GENERATED_BODY()

	FTimerHandle TickHandle;

	FTimerHandle ExpiryHandle;
};

