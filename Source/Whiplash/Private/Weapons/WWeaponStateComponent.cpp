#include "Weapons/WWeaponStateComponent.h"

#include "WCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "WCoreTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/Interface/IWeapon.h"
#include "Interface/WComponentInterface.h"

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
	/*bool bIsCrouching = WeaponUser->IsCrouching();
	bool bIsFalling = WeaponUser->IsFalling();
	float AimingAlpha = WeaponUser->GetAimingAlpha();*/
	

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
	
	

	
	
	
	
	return false;
	/*float TargetMultiplier = FMath::GetMappedRangeValueClamped(InputRange_SSST,OutPutRange_TM,PawnSpeed);
	CurrentMultiplier_StandingStill = FMath::FInterpTo(CurrentMultiplier_StandingStill,TargetMultiplier,DeltaTime,5.f);
	bool bStandingStillAtMin = FMath::IsNearlyEqual(CurrentMultiplier_StandingStill,1.f,0.1f);
	if (bIsCrouching)
	{
		
	}*/
}

