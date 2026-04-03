#pragma once

#include "CoreMinimal.h"
#include "AttributeDataStruct.generated.h"
//#include "UObject/Object.h"

USTRUCT()
struct FAttributeData
{
	GENERATED_BODY()
	
	UPROPERTY()
	float BaseValue;
	UPROPERTY()
	float CurrentValue;
	
	FAttributeData(float InBaseValue = 0.f,float InCurrentValue = 0.f)
	: BaseValue(InBaseValue),
	 CurrentValue(InCurrentValue)
{
}
	
};
