#pragma once

#include "CoreMinimal.h"
#include "FWAttributeData.generated.h"
//#include "UObject/Object.h"

USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_BODY()
	
	UPROPERTY()
	float BaseValue;
	UPROPERTY()
	float CurrentValue;
	
	FAttributeData(float InBaseValue = 0.f)
	: BaseValue(InBaseValue),
	 CurrentValue(InBaseValue)
{
}
	
};
