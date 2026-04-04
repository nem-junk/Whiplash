
#include "Attributes/WAttributeSet.h"


void UWAttributeSet::InitializeAttributes()
{
	Health.BaseValue=Health.CurrentValue=MaxHealth.CurrentValue;
	Stamina.BaseValue=Stamina.CurrentValue=MaxStamina.CurrentValue;
	bIsOutOfHealth=false;
	bIsOutOfStamina=false;
}

void UWAttributeSet::RecalculateAttribute(FAttributeData& Attribute,TArray<FAttributeModifier>& Modifiers, float Min, float Max)
{
	bool bHasOverride = false;
	float OverrideValue = 0;
	float TotalAdd = 0;
	float TotalMultiply = 1;
	
	float FinalValue = Attribute.BaseValue;
	
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

void UWAttributeSet::ClampAttribute(FAttributeData& Attribute, float Min, float Max)
{
	if (Min<=Max) Attribute.CurrentValue=FMath::Clamp(Attribute.CurrentValue, Min, Max);
	
}

void UWAttributeSet::SetHealth(float NewValue)
{
	float OldValue = Health.CurrentValue;
	Health.CurrentValue=NewValue;
	ClampAttribute(Health,0,MaxHealth.CurrentValue);
	OnHealthChanged.Broadcast(OldValue,Health.CurrentValue);
	if (Health.CurrentValue<=0 and !bIsOutOfHealth){OnOutOfHealth.Broadcast(); bIsOutOfHealth=true;}
	if (Health.CurrentValue>0 and bIsOutOfHealth){bIsOutOfHealth = false;}	
	
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

