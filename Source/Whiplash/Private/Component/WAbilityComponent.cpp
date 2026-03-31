
#include "Component/WAbilityComponent.h"
#include "Abilities/WAbility.h"


UWAbilityComponent::UWAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UWAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
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

