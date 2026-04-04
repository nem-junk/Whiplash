
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
	float FinalValue = Attribute.BaseValue;
	for (FAttributeModifier& Modifier : Modifiers)
	{
		if (Modifier.ModifierOperation==EModifierOperation::Add)
		{
			FinalValue+=Modifier.Magnitude;
		}
	}
	for (FAttributeModifier& Modifier : Modifiers)
	{
		if (Modifier.ModifierOperation==EModifierOperation::Multiply)
		{
			FinalValue*=Modifier.Magnitude;
		}
	}
	for (FAttributeModifier& Modifier : Modifiers)
	{
		if (Modifier.ModifierOperation == EModifierOperation::Override)
		{
			FinalValue=Modifier.Magnitude;
		}
	}
	
	ClampAttribute(Attribute,Min,Max);
	Attribute.CurrentValue=FinalValue;
	
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

