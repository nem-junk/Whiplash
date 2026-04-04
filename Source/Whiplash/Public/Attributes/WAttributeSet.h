
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTags.h"
#include "FWAttributeData.h"
#include "FWAttributeModifier.h"

////////////////////////////////////
#include "WAttributeSet.generated.h"
////////////////////////////////////
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeChange,float/*OldValue*/,float/*NewValue*/);
DECLARE_MULTICAST_DELEGATE(FOnAttributeDepleted);

UCLASS()
class WHIPLASH_API UWAttributeSet : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Attribute|Health")
	FAttributeData Health = FAttributeData(100);
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Attribute|Health")
	FAttributeData MaxHealth = FAttributeData(100);
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Attribute|Stamina")
	FAttributeData Stamina = FAttributeData(700);
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Attribute|Stamina")
	FAttributeData MaxStamina = FAttributeData(700);
	
	FOnAttributeChange OnHealthChanged;
	FOnAttributeChange OnMaxHealthChanged;
	FOnAttributeChange OnStaminaChanged;
	FOnAttributeDepleted  OnOutOfHealth;
	FOnAttributeDepleted  OnOutOfStamina;
	
	UPROPERTY()
	bool bIsOutOfHealth = false;
	UPROPERTY()
	bool bIsOutOfStamina = false;
private:
	UPROPERTY(VisibleAnywhere)
	TArray<FAttributeModifier> ActiveModifiers;
	UPROPERTY()
	TMap<FGameplayTag, FModifierTimerHandle> ActiveModifierTimers;
	
	FTimerHandle StaminaRegenDelayHandle;
	
	
public:
	UWorld* GetWorld() const override;
};
