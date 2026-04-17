
#include "Abilities/WAbility.h"
#include "Tags/WTagComponent.h"
#include "Component/WAbilityComponent.h"

void UWAbility::Initialize(UWAbilityComponent* NewAbilityComponent)
{
	AbilityComponent = NewAbilityComponent;
	if (AbilityComponent) TagComponent = AbilityComponent->TagComponent;
	
}

bool UWAbility::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;
	UWAbilityComponent* AbilityComp = GetOwningComponent();
	if (AbilityComp == nullptr) return false;
	
	if (TagComponent && TagComponent->HasAnyTags(BlockedTags)) return false;
	else return true;
	
}

void UWAbility::StartAbility_Implementation(AActor* Instigator)
{
	UWAbilityComponent* AbilityComp = GetOwningComponent();
	if (AbilityComp)
	{
		OwnerPawn = Cast<APawn>(AbilityComp->GetOwner());	
	}
	
	
	if (TagComponent)
	{
		for (const FGameplayTag& Tag: GrantTags)
		{
			TagComponent->AddTags(Tag);
		}
	}

	
	ActionData.bIsRunning = true;
	ActionData.Instigator = Instigator;
}

void UWAbility::StopAbility_Implementation(AActor* Instigator)
{
	UWAbilityComponent* AbilityComp = GetOwningComponent();
	
	if (TagComponent)
	{
		for (const FGameplayTag& Tag: GrantTags)
		{
			TagComponent->RemoveTags(Tag);
		}
	}
	ActionData.bIsRunning = false;
	ActionData.Instigator = Instigator;
	
}

UWorld* UWAbility::GetWorld() const
{
	AActor*Actor = Cast<AActor>(GetOuter());
	if (Actor) return Actor->GetWorld();
	else return nullptr;
}

bool UWAbility::IsRunning() const
{
	return ActionData.bIsRunning;
}

UWAbilityComponent* UWAbility::GetOwningComponent() const
{
	return AbilityComponent;
}

