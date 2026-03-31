#include "Tags/WTagComponent.h"
///////////////////////////////

UWTagComponent::UWTagComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWTagComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UWTagComponent::AddTags(FGameplayTag TagToAdd)
{
	if (!TagToAdd.IsValid()) return;	
	
	int32& Count = ActiveTags.FindOrAdd(TagToAdd,0);
	Count++;
	if (Count ==1 )
	{
		OnTagChanged.Broadcast(TagToAdd,true);
	}
}

void UWTagComponent::RemoveTags(FGameplayTag TagToRemove)
{
	if (!TagToRemove.IsValid()) return;
	if (int32* Count = ActiveTags.Find(TagToRemove))
	{
		*Count = *Count - 1;
		if (*Count <= 0)
		{
			ActiveTags.Remove(TagToRemove);
			OnTagChanged.Broadcast(TagToRemove,false);
		}
	}
}

bool UWTagComponent::HasTag(FGameplayTag TagToCheck) const
{
	return ActiveTags.Contains(TagToCheck);
}

bool UWTagComponent::HasTagExact(FGameplayTag TagToCheck) const
{	
	return ActiveTags.Contains(TagToCheck);
}
