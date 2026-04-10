
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Curves/CurveFloat.h"
#include "GameplayTags.h"
#include "FWWeaponProperties.h"

///////////////////////////////
#include "WWeaponDA.generated.h"


UCLASS()
class WHIPLASH_API UWWeaponDA : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Specs")
	float MuzzleSpeed = 60000.0f;// cm/s
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Specs")
	FRuntimeFloatCurve DistanceDamageFallOff;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Specs")
	float BulletTraceSweepRadius = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Specs")
	float MaxDamageRange = 50000;
	//UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Specs")
	// AActor Projectile Class  if i want to add projectilke in future .. 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Specs")
	bool bHasFirstShotAccuracy = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Specs")
	float BaseDamage = 15.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Specs")
	TMap<FGameplayTag,float> DamageZoneMultipliers;
	// 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Properties")
	FWeaponHandling WeaponHandling;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Properties")
	FRecoilProperties WeaponRecoil;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Properties")
	FAmmunition Ammunition;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Properties")
	FWeaponMesh Mesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Properties")
	FWeaponUI UI;


public:
	float GetDamageMultiplier(float Distance) const;	
};
