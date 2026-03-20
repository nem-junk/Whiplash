
#include "WTraversalComponent.h"
#include "Components/SplineComponent.h"


UWTraversalComponent::UWTraversalComponent()
{
	PrimaryComponentTick.bCanEverTick = true; // change this to false later

}


void UWTraversalComponent::Initialize(const TArray<USplineComponent*>& NewLedges,
                                      const TMap<USplineComponent*,USplineComponent*>& NewOppositeLedges)
{
	Ledges = NewLedges;
	OppositeLedges = NewOppositeLedges;
}

USplineComponent* UWTraversalComponent::FindLedgeClosestToActor(const FVector& ActorLocation) const
{
	if (Ledges.IsEmpty()) {return nullptr;}
	int32 ClosestIndex = INDEX_NONE;
	float ClosestDistance = MAX_flt;
	for ( int32 Index = 0; Index < Ledges.Num(); Index++ )
	{
		if (IsValid(Ledges[Index]))
		{
			const FVector ClosestPoint = Ledges[Index]-> FindLocationClosestToWorldLocation(ActorLocation,ESplineCoordinateSpace::World);
			const FVector ClosestUp = Ledges[Index]->FindUpVectorClosestToWorldLocation(ActorLocation,ESplineCoordinateSpace::World)*10;
			const float Distance = FVector::Dist(ClosestPoint + ClosestUp,ActorLocation);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestIndex = Index;
			}
			
		}
	}
	return Ledges.IsValidIndex(ClosestIndex) ? Ledges[ClosestIndex] : nullptr;
}


void UWTraversalComponent::GetLedgeTransforms_Implementation(const FVector& HitLocation, const FVector& ActorLocation,
	FTraversalCheckResult& TraversalTraceResultOut)
{
	const
	
}
