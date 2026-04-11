#include "Weapons/WWeaponStateComponent.h"
#

static const float StandingStillSpeedThreshold = 80.1f;

UWWeaponStateComponent::UWWeaponStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UWWeaponStateComponent::InitializeComponent()
{
	Super::InitializeComponent();
//	WCachedPawn = Cast<>
}

void UWWeaponStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWWeaponStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!WeaponProperties)return;
	bMinSpread=IsSpreadAtMinimum(DeltaTime);
	bMinMultipliers=AreMultipliersAtMinimum(DeltaTime);
	bApplyFirstShotAccuracy= WeaponProperties->bHasFirstShotAccuracy and bMinMultipliers and bMinSpread;
}


bool UWWeaponStateComponent::IsSpreadAtMinimum(float DeltaTime)
{
	float TimeSinceLastFired = GetWorld()->TimeSince(TimeLastFired);
	float DecayThreshold = 60.0f/WeaponProperties->WeaponHandling.RoundsPerMinute*1.5;
	if (TimeLastFired>0 and TimeSinceLastFired>DecayThreshold)
	{
		AccumulatedSpreadAngle = FMath::Max(0,
			AccumulatedSpreadAngle-(WeaponProperties->WeaponHandling.SpreadAngleAccumulationDecayPerSecond*DeltaTime)); 
		if (AccumulatedSpreadAngle < KINDA_SMALL_NUMBER) AccumulatedSpreadAngle = 0;
	}
	return AccumulatedSpreadAngle <= KINDA_SMALL_NUMBER;
}

bool UWWeaponStateComponent::AreMultipliersAtMinimum(float DeltaTime)
{
	return false;
}

