
#include "Abilities/WAbility.h"

#include "Component/WAbilityComponent.h"

void UWAbility::Initialize(UWAbilityComponent* NewAbilityComponent)
{
	AbilityComponent = NewAbilityComponent;
}

bool UWAbility::IsRunning() const
{
	return ActionData.bIsRunning;
}

UWAbilityComponent* UWAbility::GetOwningComponent() const
{
	return AbilityComponent;
}

bool UWAbility::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;
	UWAbilityComponent* AbilityComp = GetOwningComponent();
	if (AbilityComp == nullptr) return false;
	return !AbilityComp->ActiveGameplayTags.HasAny(BlockedTags);
	
}
