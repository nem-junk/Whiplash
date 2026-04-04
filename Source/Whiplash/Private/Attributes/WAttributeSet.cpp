
#include "Attributes/WAttributeSet.h"


void UWAttributeSet::InitializeAttributes()
{
	Health.BaseValue=Health.CurrentValue=MaxHealth.CurrentValue;
	Stamina.BaseValue=Stamina.CurrentValue=MaxStamina.CurrentValue;
	bIsOutOfHealth=false;
	bIsOutOfStamina=false;
}

void UWAttributeSet::RecalculateAttribute(FAttributeData& Attribute,TArray<FAttributeModifier>& Modifier, float Min, float Max)
{
	float FinalValue = Health.BaseValue;
	
	
}

void UWAttributeSet::ClampAttribute(FAttributeData& Attribute, float Min, float Max)
{
	if (Min<=Max) Attribute.CurrentValue=FMath::Clamp(Attribute.CurrentValue, Min, Max);
	
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

