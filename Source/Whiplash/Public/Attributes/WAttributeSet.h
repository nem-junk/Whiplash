
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTags.h"
////////////////////////////////////
#include "WAttributeSet.generated.h"
////////////////////////////////////
USTRUCT()
struct FWAttributeData
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag AffectedAttribute;  
	UPROPERTY()
	float BaseValue = 0.0f;          

	UPROPERTY()
	float CurrentValue = 0;    
	
	UPROPERTY()
	float Modifiers= 0 ;

	UPROPERTY()
	FTimerHandle TimerHandle;       
};



UCLASS()
class WHIPLASH_API UWAttributeSet : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WAttributeSet|Health")
	float Health = 100.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WAttributeSet|Stamina")
	float Stamina = 100.f;
	
};
