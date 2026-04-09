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

UENUM(BlueprintType)
enum class EAttributeTarget : uint8
{
	Health =0 UMETA(DisplayName = "Health"),
	Stamina =1 UMETA(DisplayName = "Stamina")
};

USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Interval;//tick value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EModifierOperation ModifierOperation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttributeTarget Target; // whom to modify
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ID;//remove in future
	
	FAttributeModifier(float InMagnitude=0,
		float InDuration=0,
		float InInterval=0,
		EModifierOperation InModifierOperation=EModifierOperation::Add,
		EAttributeTarget InTarget=EAttributeTarget::Health,
		FGameplayTag InID = FGameplayTag::EmptyTag
		)
	: Magnitude(InMagnitude),
	Duration(InDuration),
	Interval(InInterval),
	ModifierOperation(InModifierOperation),
	Target(InTarget),
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

