
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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	float StaminaDrainMag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	float StaminaDrainDuration;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	float StaminaDrainInterval;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	FGameplayTag StaminaDrainID;
	//UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	//FGameplayTag StaminaID;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	float RegenDelay;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	float StaminaRegenMag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	float StaminaRegenDuration;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	float StaminaRegenInterval;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Stamina")
	FGameplayTag StaminaRegenID;
	
	//UPROPERTY()
	bool bIsOutOfHealth;
//	UPROPERTY()
	bool bIsOutOfStamina;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Attribute|Immunity")
	bool bIsDamageImmune = false;
private:
	//Ongoing Modifiers 
	UPROPERTY(VisibleAnywhere)
	TArray<FAttributeModifier> ActiveHealthModifiers;
	UPROPERTY(VisibleAnywhere)
	TArray<FAttributeModifier> ActiveStaminaModifiers;
	
	
	UPROPERTY()
	TMap<FGameplayTag, FModifierTimerHandle> ActiveModifierTimers;
	
	FTimerHandle StaminaRegenDelayHandle;
	void ClampAttribute(FAttributeData& Attribute,float Min,float Max);
	void SetHealth(float NewValue);
	void SetStamina(float NewValue);
	void ApplyAttributeChange(EAttributeTarget Target, float Magnitude);
	/*
	*/
	
	
public:
	virtual UWorld* GetWorld() const override;
	
	void InitializeAttributes();
	void RecalculateAttribute(FAttributeData& Attribute,const TArray<FAttributeModifier>& Modifiers,float Min,float Max);
	void ApplyModifier(FAttributeModifier Modifier);
	void RemoveModifier(FGameplayTag ModifierID);
	/*
	
	
	
	

	*/
	void StartStaminaDrain();
	void StopStaminaDrain();
	void StartStaminaRegen();
	void StopStaminaRegen();
	void ApplyDamage(float FinalDamage);
	float GetHealth() const;
	float GetMaxHealth() const;
	float GetStamina() const;
	float GetMaxStamina() const;
	void ApplyHealing(float Amount);
	
};
