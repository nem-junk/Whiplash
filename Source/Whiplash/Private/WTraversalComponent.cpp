
#include "WTraversalComponent.h"

UWTraversalComponent::UWTraversalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UWTraversalComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UWTraversalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

