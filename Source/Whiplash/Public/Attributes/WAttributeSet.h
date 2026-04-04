
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
	bool bIsOutOfHealth;
	UPROPERTY()
	bool bIsOutOfStamina;
private:
	
	UPROPERTY(VisibleAnywhere)
	TArray<FAttributeModifier> ActiveHealthModifiers;
	TArray<FAttributeModifier> ActiveStaminaModifiers;
	
	
	UPROPERTY()
	TMap<FGameplayTag, FModifierTimerHandle> ActiveModifierTimers;
	
	FTimerHandle StaminaRegenDelayHandle;
	void ClampAttribute(FAttributeData& Attribute,float Min,float Max);
	/*void SetHealth(float NewValue);
	void SetStamina(float NewValue);*/
	
	
public:
	UWorld* GetWorld() const override;
	
	void InitializeAttributes();
	void RecalculateAttribute(FAttributeData& Attribute,TArray<FAttributeModifier>& Modifier,float Min,float Max);
	/*void ApplyModifiers(FAttributeModifier Modifier);
	void RemoveModifiers(FGameplayTag ModifierID);
	
	
	void ApplyDamage(float FinalDamage,const FDamageEvent& DamageEvent);
	void ApplyHealing(float Amount);
	void StartStaminaDrain();
	void StopStaminaDrain();
	void StartStaminaRegen();
	void StopStaminaRegen();*/
	float GetHealth() const;
	float GetMaxHealth() const;
	float GetStamina() const;
	float GetMaxStamina() const;
	
};
