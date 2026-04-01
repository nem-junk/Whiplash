
#include "Component/WAbilityComponent.h"

#include "WCoreTypes.h"
#include "Abilities/WAbility.h"
#include "Engine/ActorChannel.h"
#include "GeometryCollection/GeometryCollectionParticlesData.h"
#include "Tags/WTagComponent.h"


UWAbilityComponent::UWAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UWAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	TagComponent = GetOwner()->FindComponentByClass<UWTagComponent>();
	if (!TagComponent) WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("TagComp is null"));
	for (TSubclassOf<UWAbility> AbilityClass : DefaultAbilities)
	{
		AddAbility(GetOwner(), AbilityClass);
	}

}


void UWAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWAbilityComponent::AddAbility(AActor* Instigator, TSubclassOf<UWAbility> AbilityClass)
{
	if (!ensure(AbilityClass)) return;	
												/*the player will manage the actions*/
	UWAbility* NewAbility = NewObject<UWAbility>(GetOwner(), AbilityClass);
	if (ensure(NewAbility))
	{
		NewAbility->Initialize(this);
		Abilities.Add(NewAbility);
		if (NewAbility->bAutoStart && ensure(NewAbility->CanStart(Instigator)))
		{
			// NewAbility->StartAction(Instigator);
		}
	}
}

UWAbility* UWAbilityComponent::GetAbility(TSubclassOf<UWAbility> AbilityClass)
{
	for (UWAbility* Ability : Abilities)
	{
		if (Ability && Ability->IsA(AbilityClass))
		{
			return Ability;
		}
	}
	WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("GetAbility() -> return null from GetAbility"));
	
	return nullptr;
}

bool UWAbilityComponent::StartAbilityByTag(AActor* Instigator, FGameplayTag AbilityName)
{
	for (UWAbility* Ability : Abilities)
	{
		if (Ability && Ability->AbilityTag==AbilityName)
		{
			if (!Ability->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("CanStart is false: %s"),*AbilityName.ToString());
				continue;
			}
			// run the ability here when replicated ServerStartAction maybe 
			Ability->StartAbility(Instigator);
			return true;
		}
	}
	WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("StartAbilityByTag() -> return null"));
	return false;
}

bool UWAbilityComponent::StopAbilityByTag(AActor* Instigator, FGameplayTag AbilityName)
{
	for (UWAbility* Ability : Abilities)
	{
		if (Ability && Ability->AbilityTag==AbilityName)
		{
			if (!Ability->IsRunning())
			{
				Ability->StopAbility(Instigator);
				return true;
			}
		}
	}
	WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("StopAbilityByTag() -> return null"));
	return false;
}


void UWAbilityComponent::RemoveAbility(UWAbility* AbilityToRemove)
{
	if (!ensure(AbilityToRemove && !AbilityToRemove->IsRunning())) return;
	Abilities.Remove(AbilityToRemove);
}

