
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WWeaponDA.h"

///////////////////////////////////////////
#include "WWeaponStateComponent.generated.h"

class UCharacterMovementComponent;
class AWPlayerController;
class AWCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WHIPLASH_API UWWeaponStateComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	UWWeaponStateComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

private:

	TWeakObjectPtr<AWCharacter> WCachedPawn;
	TWeakObjectPtr<UCharacterMovementComponent> WCachedCMC;// remove this can get CMC through character;
	
	TObjectPtr<UWWeaponDA> WeaponProperties;
	TObjectPtr<AActor> Mesh;
	
	float TimeLastFired = 0;
	float TimeLastEquipped = 0;
	int32 CurrentMagazineAmmo=0;
	int32 CurrentReserveAmmo=0;
	float AccumulatedSpreadAngle=0;
	float AccumulatedSpreadAngleMultiplier=1;
	bool  bHas1InTheChamber=false;
	bool  bApplyFirstShotAccuracy=false;
	float CurrentMultiplier_StandingStill=1;
	float CurrentMultiplier_Crouching=1;
	float CurrentMultiplier_Falling=1;
	
	float FireIntervalSeconds =0;//calculated once in EquipWeapon as 60.0/sec also in CanFire fucntion when implmented 
	bool  bMinSpread=false;
	bool  bMinMultipliers=false;
	
	
	
	bool IsSpreadAtMinimum(float DeltaTime);
	bool AreMultipliersAtMinimum(float DeltaTime);
	
	
	

public:
	
};
