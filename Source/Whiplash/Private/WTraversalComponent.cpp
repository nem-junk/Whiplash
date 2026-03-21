
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
	const USplineComponent* ClosestLedge = FindLedgeClosestToActor(ActorLocation);
	if (!IsValid(ClosestLedge) || MinLedgeWidth){TraversalTraceResultOut.bHasFrontLedge=false;return;}
	
	const float HalfMinLedgeWidth = MinLedgeWidth / 2;
	const FVector ClosestHitLocation = ClosestLedge->FindLocationClosestToWorldLocation(HitLocation,ESplineCoordinateSpace::Local);
	const float ClosestHitDistance = 
		FMath::Clamp(ClosestLedge->GetDistanceAlongSplineAtLocation(ClosestHitLocation,ESplineCoordinateSpace::Local),
			HalfMinLedgeWidth,ClosestLedge->GetSplineLength()-HalfMinLedgeWidth);
	
	const FTransform FrontLedgeTransform = ClosestLedge->GetTransformAtDistanceAlongSpline(ClosestHitDistance,ESplineCoordinateSpace::World);
	TraversalTraceResultOut.FrontLedgeLocation = FrontLedgeTransform.GetLocation();
	TraversalTraceResultOut.FrontLedgeNormal = FrontLedgeTransform.GetRotation().GetUpVector();
	TraversalTraceResultOut.bHasFrontLedge = true;
	if (!OppositeLedges.Contains(ClosestLedge))
	{
		TraversalTraceResultOut.bHasBackLedge = false;
	}
	const USplineComponent* OppositeLedge = *OppositeLedges.Find(ClosestLedge);
	const FTransform BackLedgeTransform = 
		OppositeLedge->FindTransformClosestToWorldLocation(TraversalTraceResultOut.FrontLedgeLocation,ESplineCoordinateSpace::World);
	TraversalTraceResultOut.BackLedgeLocation = BackLedgeTransform.GetLocation();
	TraversalTraceResultOut.BackLedgeNormal = BackLedgeTransform.GetRotation().GetUpVector();
	TraversalTraceResultOut.bHasBackLedge = true;
	
}
