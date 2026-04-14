#include "Weapons/WWeaponStateComponent.h"

#include "WCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "WCoreTypes.h"
#include "Animation/InputScaleBias.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/Interface/IWeapon.h"
#include "Interface/WComponentInterface.h"
#include "Tags/WTagComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Physics/WPhysicalMaterialWithTags.h"

static const float StandingStillSpeedThreshold = 80.1f;
//static constexpr float MultiplierNearlyEqualThreshold_Crouch = 0.05f;
static const FFloatRange InputRange_SSST = FFloatRange(StandingStillSpeedThreshold, StandingStillSpeedThreshold+20);

UWWeaponStateComponent::UWWeaponStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UWWeaponStateComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	OwnerPawn = Cast<APawn>(GetOwner());
	CMC = Cast<UCharacterMovementComponent>(OwnerPawn->GetMovementComponent());
	if (IWComponentInterface* Provider = Cast<IWComponentInterface>(GetOwner()))
	{
		TagComponent = Provider->GetTagComponent();
		if (!TagComponent) WHIPLASH_LOG(LogWhiplash,Error,TEXT("TagComponentisNULLinWeaponInstance"));
	}
}


void UWWeaponStateComponent::BeginPlay()
{
	Super::BeginPlay();
	float ShotInterval = 60.f / WeaponProperties->WeaponHandling.RoundsPerMinute;
	CachedDecayThreshold = ShotInterval*1.5f;
	OutPutRange_TM = FFloatRange(1.f,WeaponProperties->WeaponHandling.SpreadAngleMultiplier_Moving);
	
}
void UWWeaponStateComponent::Fire()
{
	TimeLastFired = GetWorld()->GetTimeSeconds();
	AccumulatedSpreadAngle += WeaponProperties->WeaponHandling.SpreadAngleAccumulationPerShot;
	CurrentMagazineAmmo = FMath::Max(0,CurrentMagazineAmmo-1);
	bHas1InTheChamber = CurrentMagazineAmmo>0;
}

bool UWWeaponStateComponent::CanFire() const
{
	return WeaponProperties and CurrentMagazineAmmo>0 and (TimeLastFired==0 || GetWorld()->TimeSince(TimeLastFired)>=FireIntervalSeconds);
}

bool UWWeaponStateComponent::CanReload() const
{
	if (!WeaponProperties) return false;
	const int32 MaxCapacity = WeaponProperties->Ammunition.MagazineSize;
	return (CurrentMagazineAmmo < MaxCapacity) and (CurrentReserveAmmo>0);
	
}


void UWWeaponStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!WeaponProperties)return;
	bMinSpread=IsSpreadAtMinimum(DeltaTime);
	bMinMultipliers=AreMultipliersAtMinimum(DeltaTime);
	bApplyFirstShotAccuracy= WeaponProperties->bHasFirstShotAccuracy and bMinMultipliers and bMinSpread;
	IsSpreadAtMinimum(DeltaTime);
	AreMultipliersAtMinimum(DeltaTime);
	
}


bool UWWeaponStateComponent::IsSpreadAtMinimum(float DeltaTime)
{
	float TimeSinceLastFired = GetWorld()->TimeSince(TimeLastFired);
	//float DecayThreshold = 60.0f/WeaponProperties->WeaponHandling.RoundsPerMinute*1.5;
	if (TimeLastFired>0 and TimeSinceLastFired>CachedDecayThreshold)
	{
		AccumulatedSpreadAngle = FMath::Max(0,
			AccumulatedSpreadAngle-(WeaponProperties->WeaponHandling.SpreadAngleAccumulationDecayPerSecond*DeltaTime)); 
		if (AccumulatedSpreadAngle < KINDA_SMALL_NUMBER) AccumulatedSpreadAngle = 0;
	}
	return FMath::IsNearlyEqual(AccumulatedSpreadAngle,0.f,KINDA_SMALL_NUMBER);
}

bool UWWeaponStateComponent::AreMultipliersAtMinimum(float DeltaTime)
{
	if (!OwnerPawn)
	{	
		WHIPLASH_LOG(LogWhiplash,Error,TEXT("OwnerPawnIsNullin_AreMultiplierAtMinimun"));
		if (!CMC)
		{
			WHIPLASH_LOG(LogWhiplash,Error,TEXT("CMCIsNullin_AreMultiplierAtMinimun"));
		}
		return false;
	}
	//check(OwnerPawn);
	// if Standing still->smoothlyApply
	float PawnSpeed = OwnerPawn->GetVelocity().Size();
	const float MovementTargetValue = FMath::GetMappedRangeValueClamped(
		InputRange_SSST,
		OutPutRange_TM,PawnSpeed);
	CurrentMultiplier_StandingStill = FMath::FInterpTo(CurrentMultiplier_StandingStill,
		MovementTargetValue,
		DeltaTime,5.f);
	const bool bStandingStillMultiplierAtMin = FMath::IsNearlyEqual(CurrentMultiplier_StandingStill,1.f,0.1f);
	//
	const bool bIsCrouching = CMC and CMC->IsCrouching();
	float CrouchingTargetValue = bIsCrouching ? WeaponProperties->WeaponHandling.SpreadAngleMultiplier_Crouch :1.f;
	CurrentMultiplier_Crouching = FMath::FInterpTo(CurrentMultiplier_Crouching,CrouchingTargetValue,DeltaTime,5.f);
	const bool bCrouchingMultiplierAtTarget = FMath::IsNearlyEqual(CurrentMultiplier_Crouching,CrouchingTargetValue,MultiplierNearlyEqualThreshold);
	
	// in air 
	const bool bInAir = CMC and CMC->IsFalling();
	const float InAirTargetValue = bInAir ? WeaponProperties->WeaponHandling.SpreadAngleMultiplier_Falling : 1.f;
	CurrentMultiplier_Falling = FMath::FInterpTo(CurrentMultiplier_Falling,InAirTargetValue,DeltaTime,5.f);
	const bool bInAirMultiplierIs1 = FMath::IsNearlyEqual(CurrentMultiplier_Falling,1.f,MultiplierNearlyEqualThreshold);
	
	// AimDownSight
	
	float AimingAlpha=0.f;
	
	if (TagComponent) AimingAlpha = TagComponent->HasTag(WhiplashTags::State_ADS) ? 1.f : 0.f;
	
	const float AimingMultiplier = FMath::GetMappedRangeValueClamped(
		FFloatRange(0.f,1.f),
		FFloatRange(WeaponProperties->WeaponHandling.SpreadAngleMultiplier_HipFire,WeaponProperties->WeaponHandling.SpreadAngleMultiplier_ADS),
		AimingAlpha
		) ;
	
	const bool bAimingMultiplierAtTarget = FMath::IsNearlyEqual(AimingMultiplier,WeaponProperties->WeaponHandling.SpreadAngleMultiplier_ADS,KINDA_SMALL_NUMBER);
	
	//combine all multipliers 
	const float CombinedMultiplier = AimingMultiplier*CurrentMultiplier_StandingStill*CurrentMultiplier_Crouching*CurrentMultiplier_Falling;
	
	AccumulatedSpreadAngleMultiplier = CombinedMultiplier;
	// need to handle these spread multipliers indicating we are not at min spread ?????????????????
	return bStandingStillMultiplierAtMin and bCrouchingMultiplierAtTarget and bInAirMultiplierIs1 and bAimingMultiplierAtTarget;
}

float UWWeaponStateComponent::GetDistanceAttenuation(float Distance) const
{
	if (!WeaponProperties) return 1.0f;
	const FRichCurve* Curve =  WeaponProperties->DistanceDamageFallOff.GetRichCurveConst();
	return Curve->HasAnyData() ? Curve->Eval(Distance) : 1.f;
}

float UWWeaponStateComponent::GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhyMaterial) const
{
	if (!WeaponProperties) return 1.f;
	float CombinedMultiplier =1.f;
	if (const UWPhysicalMaterialWithTags* PhyMatWithTags = Cast<const UWPhysicalMaterialWithTags>(PhyMaterial))
	{
		for (const FGameplayTag MaterialTag : PhyMatWithTags->Tags)
		{
			if (const float* pTagMultiplier = WeaponProperties->DamageZoneMultipliers.Find(MaterialTag))
			{
				CombinedMultiplier *= *pTagMultiplier;
			}
		}
	}
	return CombinedMultiplier;
}

