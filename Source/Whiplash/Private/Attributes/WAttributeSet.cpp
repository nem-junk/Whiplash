
#include "Attributes/WAttributeSet.h"

#include <Programs/UnrealBuildAccelerator/Core/Public/UbaBase.h>


#include "LandscapeEditLayerTypes.h"
#include "LocalizationDescriptor.h"
#include "WCoreTypes.h"


void UWAttributeSet::InitializeAttributes()
{
	Health.BaseValue=Health.CurrentValue=MaxHealth.CurrentValue;
	Stamina.BaseValue=Stamina.CurrentValue=MaxStamina.CurrentValue;
	bIsOutOfHealth=false;
	bIsOutOfStamina=false;
}
void UWAttributeSet::RecalculateAttribute(FAttributeData& Attribute,const TArray<FAttributeModifier>& Modifiers, float Min, float Max)
{
	bool bHasOverride = false;
	float OverrideValue = 0;
	float TotalAdd = 0;
	float TotalMultiply = 1;
	
	float FinalValue = Attribute.CurrentValue;
	
	for (const FAttributeModifier& Modifier : Modifiers)
	{
		switch (Modifier.ModifierOperation)
		{
		case EModifierOperation::Override:
			bHasOverride = true;
			OverrideValue=Modifier.Magnitude;
			break;
		case EModifierOperation::Add:
			TotalAdd+=Modifier.Magnitude;
			break;
		case EModifierOperation::Multiply:
			TotalMultiply*=Modifier.Magnitude;
			break;
		}
	}
	if (bHasOverride)
	{
		FinalValue = OverrideValue;
	}
	else
	{
		FinalValue = (FinalValue + TotalAdd)*TotalMultiply;
	}
	Attribute.CurrentValue=FinalValue;
	ClampAttribute(Attribute,Min,Max);
	
}


void UWAttributeSet::ApplyModifier(FAttributeModifier Modifier)
{
	if (Modifier.Duration == 0 )
	{
		ApplyAttributeChange(Modifier.Target,Modifier.Magnitude);
		return;
	}
	
	if (Modifier.Target==EAttributeTarget::Health)
	{
		ActiveHealthModifiers.Add(Modifier);
	}
	else if (Modifier.Target==EAttributeTarget::Stamina)
	{
		ActiveStaminaModifiers.Add(Modifier);
		
	}
	FModifierTimerHandle Handles;
	
	if (Modifier.Interval > 0 and GetWorld())
	{
		FTimerDelegate TickDelegate;
		TickDelegate.BindWeakLambda(this,[this , Target = Modifier.Target, Mag = Modifier.Magnitude]()
		{
			ApplyAttributeChange(Target,Mag);
		});
		GetWorld()->GetTimerManager().SetTimer(Handles.TickHandle,TickDelegate,Modifier.Interval,true);
	}
	if (Modifier.Duration > 0 and GetWorld())
	{
		FTimerDelegate ExpiryDelegate;
		ExpiryDelegate.BindWeakLambda(this,[this,ModID = Modifier.ID]()
		{
			RemoveModifier(ModID);
		});
		GetWorld()->GetTimerManager().SetTimer(Handles.ExpiryHandle,ExpiryDelegate,Modifier.Duration,false);
	}
	ActiveModifierTimers.Add(Modifier.ID,Handles);
}

void UWAttributeSet::RemoveModifier(FGameplayTag ModifierID)
{
	//WHIPLASH_LOG(LogWhiplashAbility, Warning, TEXT("RemoveModifier called — ID: %s"), *ModifierID.ToString());
	FModifierTimerHandle* ModifierToRemove = ActiveModifierTimers.Find(ModifierID);
	//WHIPLASH_LOG(LogWhiplashAbility, Warning, TEXT("RemoveModifier — Found: %s"), ModifierToRemove ? TEXT("YES") : TEXT("NO"));
	if (!ModifierToRemove) return;
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ModifierToRemove->TickHandle);
		GetWorld()->GetTimerManager().ClearTimer(ModifierToRemove->ExpiryHandle);
		
	}
	ActiveModifierTimers.Remove(ModifierID);
	int32 HealthRemoved = ActiveHealthModifiers.RemoveAll([ModifierID](const
		FAttributeModifier& Element)
	{
		return Element.ID == ModifierID;
	});
	if (HealthRemoved>0)
	{
		RecalculateAttribute(Health,ActiveHealthModifiers,0,MaxHealth.CurrentValue);
		return;
	}
	int32 StaminaRemoved = ActiveStaminaModifiers.RemoveAll([ModifierID](const FAttributeModifier& Element)
	{
		return Element.ID == ModifierID;
	});
	if (StaminaRemoved>0)
	{
		RecalculateAttribute(Stamina,ActiveStaminaModifiers,0,MaxStamina.CurrentValue);
		return;
	}
}

void UWAttributeSet::StartStaminaDrain()
{
	StopStaminaRegen();
	if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(StaminaRegenDelayHandle);
	FAttributeModifier StaminaDrain(-StaminaDrainMag,StaminaDrainDuration,StaminaDrainInterval,EModifierOperation::Add,
		EAttributeTarget::Stamina,StaminaDrainID);
	//FAttributeModifier StaminaDrain = StaminaDrainMod;
	ApplyModifier(StaminaDrainMod);
}
void UWAttributeSet::StartStaminaRegen()
{
	if (GetStamina() >= MaxStamina.CurrentValue) return;
	ApplyModifier(StaminaRegenMod);
}


void UWAttributeSet::StopStaminaDrain()
{
	RemoveModifier(StaminaDrainMod.ID);
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenDelayHandle);
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenDelayHandle, [this]()
		{
			StartStaminaRegen();
		},
		RegenDelay, false);
	}
	
}


void UWAttributeSet::StopStaminaRegen()
{
	RemoveModifier(StaminaRegenMod.ID);
}

void UWAttributeSet::ApplyAttributeChange(EAttributeTarget Target, float Magnitude)
{
		switch (Target)
		{
		case EAttributeTarget::Health:
			SetHealth(GetHealth() + Magnitude);
			break;
		case EAttributeTarget::Stamina:
			SetStamina(GetStamina() + Magnitude);
			break;
		default: WHIPLASH_LOG(LogWhiplashAbility,Warning,TEXT("ApplyAttributeChange: Unknown Target"));
			break;
		}
}


void UWAttributeSet::ApplyDamage(float FinalDamage)
{
	if (bIsDamageImmune) return;
	SetHealth(GetHealth() - FinalDamage);
}

void UWAttributeSet::ClampAttribute(FAttributeData& Attribute, float Min, float Max)
{
	if (Min<=Max) Attribute.CurrentValue=FMath::Clamp(Attribute.CurrentValue, Min, Max);
	
}

void UWAttributeSet::SetHealth(float NewValue)
{
	float OldValue = Health.CurrentValue;
	
	Health.CurrentValue=NewValue;
	if (FMath::IsNearlyEqual(OldValue,Health.CurrentValue))
	{
		WHIPLASH_LOG(LogWhiplashAbility,Warning,TEXT("Health is nearly equal to OldHealth"));
		return;
	}
	
	ClampAttribute(Health,0,MaxHealth.CurrentValue);
	OnHealthChanged.Broadcast(OldValue,Health.CurrentValue);
	if (Health.CurrentValue<=0 and !bIsOutOfHealth){OnOutOfHealth.Broadcast(); bIsOutOfHealth=true;}
	else if (Health.CurrentValue>0 and bIsOutOfHealth){bIsOutOfHealth = false;}	
	
}

void UWAttributeSet::SetStamina(float NewValue)
{
	float OldValue = Stamina.CurrentValue;
	
	Stamina.CurrentValue=NewValue;
	if (FMath::IsNearlyEqual(OldValue,Stamina.CurrentValue))
	{
		WHIPLASH_LOG(LogWhiplashAbility,Warning,TEXT("Newstamina is nearly equal to Old"));
		return;
	}
	ClampAttribute(Stamina,0,MaxStamina.CurrentValue);
	OnStaminaChanged.Broadcast(OldValue,Stamina.CurrentValue);
	if (Stamina.CurrentValue<=0 and !bIsOutOfStamina)
	{
		OnOutOfStamina.Broadcast(); bIsOutOfStamina=true;
	}
	else if (Stamina.CurrentValue>0 and bIsOutOfStamina){bIsOutOfStamina = false;}
	if (Stamina.CurrentValue>= MaxStamina.CurrentValue and ActiveModifierTimers.Contains(StaminaRegenMod.ID))
	{
		
		StopStaminaRegen();
	}
}


void UWAttributeSet::ApplyHealing(float Amount)
{
	SetHealth(GetHealth() + Amount);
}



UWorld* UWAttributeSet::GetWorld() const   
{
	UObject* Outer = GetOuter();
	check(Outer);
	return Outer->GetWorld();
}

//////////////////////////////////////////////////////////////////////////////////////////////
///
float UWAttributeSet::GetHealth() const
{
	return Health.CurrentValue;
}

float UWAttributeSet::GetMaxHealth() const
{
	return MaxHealth.CurrentValue;
}

float UWAttributeSet::GetStamina() const
{
	return Stamina.CurrentValue;
}

float UWAttributeSet::GetMaxStamina() const
{
	return MaxStamina.CurrentValue;
}

