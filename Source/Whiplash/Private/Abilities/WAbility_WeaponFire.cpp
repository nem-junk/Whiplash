#include "Abilities/WAbility_WeaponFire.h"
#include "WCharacter.h"
#include "Weapons/WWeaponStateComponent.h"
#include "Weapons/WWeaponDA.h"
#include "Tags/WTagComponent.h"
#include "Tags/WGameplayTags.h"
#include "Component/WAbilityComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/Interface/IWeapon.h"



UWAbility_WeaponFire::UWAbility_WeaponFire()
{
	AbilityTag = WhiplashTags::Ability_Action_Shoot;
	//BlockedTags = WhiplashTags::Ability_Action_Reload 
	BlockedTags.AddTag(WhiplashTags::State_Dead);
	BlockedTags.AddTag(WhiplashTags::State_Stunned);
	BlockedTags.AddTag(WhiplashTags::State_Reloading);
	
}

bool UWAbility_WeaponFire::CanStart_Implementation(AActor* Instigator)
{
	if (!Super::CanStart_Implementation(Instigator)) return false;
	IIWeapon*WeaponOwner = Cast<IIWeapon>(Instigator);
	if (!WeaponOwner) return false;
	UWWeaponStateComponent*WeaponState = WeaponOwner->GetWeaponState();
	if (!WeaponState) return false;
	CachedWeaponState = WeaponState;
	return WeaponState->CanFire(); 
	
}

void UWAbility_WeaponFire::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);
	if (!CachedWeaponState)
	{
		StopAbility(nullptr);
		return;
	}
	bIsFirstShot = true;
	if (TagComponent)
	{
		TagComponent->AddTags(WhiplashTags::State_Firing);
		FireOnce();
		if (!CachedWeaponState) return;
		if(CachedWeaponState->GetWeaponDA()->Ammunition.bIsAutomatic)
		{
			GetWorld()->GetTimerManager().SetTimer(FireLoopTimerHandle,
				this, &UWAbility_WeaponFire::FireOnce,
				60.f/CachedWeaponState->GetWeaponDA()->WeaponHandling.RoundsPerMinute,
				true);
		}
		else
		{
			TWeakObjectPtr<UWAbility_WeaponFire> WeakThis(this);
			GetWorld()->GetTimerManager().SetTimerForNextTick(
				[WeakThis](){if (WeakThis.IsValid()) WeakThis->StopAbility(nullptr);}
				);
		}
	}
	
}

void UWAbility_WeaponFire::StopAbility_Implementation(AActor* Instigator)
{
	Super::StopAbility_Implementation(Instigator);
}

void UWAbility_WeaponFire::FireOnce()
{
	if (!CachedWeaponState || !CachedWeaponState->CanFire())
	{
		StopAbility(nullptr);
		return;
	}
	CachedWeaponState->Fire();
	const UWWeaponDA* WeaponDA = CachedWeaponState->GetWeaponDA();
	if (!WeaponDA || !OwnerPawn)
	{
		WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("WeaponDA||OwnerPawn null"));
		return;
	}
	// tu he tho sab thi, tu he tho sab hai 
	// kab se mai tera ho kab se tu meri laila 
	FVector AimTarget = GetAimTargetPoint();
	USceneComponent* MuzzleRoot = CachedWeaponState->GetWeaponMeshRootComponent();
	if (!MuzzleRoot)
	{
		WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("MuzzleRoot null"));
		return;
	}
	FVector MuzzleLocation = MuzzleRoot->GetSocketLocation(MuzzleSocketName);
	FVector BaseDirection = (AimTarget - MuzzleLocation).GetSafeNormal();
	FRotator SpreadRot = GetSpreadRotation();
	FVector FinalDirection = SpreadRot.RotateVector(BaseDirection);
	FHitResult HitResult;
	FVector TraceEnd = MuzzleLocation + FinalDirection * WeaponDA->MaxDamageRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerPawn);
	if (WeaponDA->BulletTraceSweepRadius > 0)
	{
		GetWorld()->SweepSingleByChannel(HitResult,
			MuzzleLocation,
			TraceEnd,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(WeaponDA->BulletTraceSweepRadius),Params);
	}
	else
	{
		GetWorld()->LineTraceSingleByChannel(HitResult,
			MuzzleLocation,
			TraceEnd,
			ECC_Visibility,Params);
	}
#if !UE_BUILD_SHIPPING
	DrawDebugLine(GetWorld(),MuzzleLocation,
		HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd,
		HitResult.bBlockingHit ? FColor::Red : FColor::Green,
		false,1.f,0,1.f);
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,5.f,8,FColor::Red,1.f);
	}
#endif
	
}

FVector UWAbility_WeaponFire::GetAimTargetPoint() const
{
	if (!OwnerPawn)
	{
		WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("OwnerPawn null GetAimTargetPoint"));
		return FVector::ZeroVector;
	}
	AController* PC = OwnerPawn->GetController();
	if (!PC)
	{
		WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("PC null"));
		return FVector::ZeroVector;
	}
	FVector ViewLocation ;
	FRotator ViewRotation ;
	PC->GetPlayerViewPoint(ViewLocation, ViewRotation);
	if (!CachedWeaponState)
	{
		WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("weaponstate null in Get aim Target Point"));
		return FVector::ZeroVector;
	}
	FVector TraceEnd = ViewLocation + ViewRotation.Vector()*CachedWeaponState->GetWeaponDA()->MaxDamageRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerPawn);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult,
		ViewLocation,TraceEnd,
		ECC_Visibility,Params);
	return HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	
}

FRotator UWAbility_WeaponFire::GetSpreadRotation() const
{
	if (!CachedWeaponState || CachedWeaponState->HasFirstShotAccuracy()) return FRotator::ZeroRotator;
	float HalfCone = CachedWeaponState->GetSpreadAngle()* CachedWeaponState->GetSpreadAngleMultiplier();
	float Pitch = FMath::RandRange(-HalfCone,HalfCone);
	float Yaw = FMath::RandRange(-HalfCone,HalfCone);
	return FRotator(Pitch,Yaw,0);
}
