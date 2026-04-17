
#pragma once

#include "CoreMinimal.h"
#include "WCharacter.h"
#include "Components/ActorComponent.h"
#include "Weapons/WWeaponDA.h"
#include "Physics/WPhysicalMaterialWithTags.h"

///////////////////////////////////////////
#include "WWeaponStateComponent.generated.h"

class UCharacterMovementComponent;
class AWPlayerController;
class AWCharacter;
class IIWeapon;
class UWTagComponent;
class UPhysicalMaterial;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WHIPLASH_API UWWeaponStateComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	UWWeaponStateComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;
	
	/*returns world time that this weapon was last fired / 0 never fired*/
	UFUNCTION(BlueprintCallable,Category="Weapon|Timing")
	float GetLastTimeFired() const {return TimeLastFired;}
	UFUNCTION(BlueprintCallable,Category="Weapon|Timing")
	float GetTimeLastEquipped() const {return TimeLastEquipped;}
	UFUNCTION(BlueprintCallable,Category="Weapon|Ammunition", meta = (DisplayName = "Has 1 in the Chamber? "))
	bool GetHas1InTheChamber() const {return bHas1InTheChamber;}
	UFUNCTION(BlueprintCallable,Category="Weapon|Spread")
	float GetSpreadAngle() const {return WeaponProperties ? (WeaponProperties->WeaponHandling.SpreadAngleBase + AccumulatedSpreadAngle) : (AccumulatedSpreadAngle);}
	UFUNCTION(BlueprintCallable,Category="Weapon|Spread")
    float GetSpreadAngleMultiplier() const {return bApplyFirstShotAccuracy ? 0 :AccumulatedSpreadAngleMultiplier;}
    UFUNCTION(BlueprintCallable,Category="Weapon|Spread")
	bool HasFirstShotAccuracy() const {return bApplyFirstShotAccuracy;}
	UFUNCTION(BlueprintCallable,Category="Weapon")
	const UWWeaponDA* GetWeaponDA() const {return WeaponProperties;}
	// returns a multiplier between 0||1 based on  the distance from the Target
	UFUNCTION(BlueprintCallable)
	virtual float GetDistanceAttenuation(float Distance) const;
	//returns a multiplier b/w 1||0 based on the phy. material 
	UFUNCTION(BlueprintCallable)
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhyMaterial) const;
	
	void Fire();
	
	UFUNCTION(BlueprintCallable)
	bool CanFire()const;
	
	UFUNCTION(BlueprintCallable)
	bool CanReload() const;
	UFUNCTION(BlueprintCallable)
	USceneComponent* GetWeaponMeshRootComponent() const;
	
	void Reload();
	
	void EquipWeapon(UWWeaponDA * WeaponDef);
	
	void UnEquipWeapon();

protected:
	virtual void BeginPlay() override;

private:

	TObjectPtr<APawn> OwnerPawn;
	TObjectPtr<UCharacterMovementComponent> CMC;
	TObjectPtr<UWTagComponent> TagComponent; // tag comp from character class;
	TWeakObjectPtr<AWCharacter> WCachedPawn;
	TWeakObjectPtr<UCharacterMovementComponent> WCachedCMC;// remove this can get CMC through character;
	
	TObjectPtr<UWWeaponDA> WeaponProperties; // fuckkkkkkkkkkkk!! WeaponProperties have the same name as Struct FWeaponProperties but they both are diff things God help me 
	TObjectPtr<AActor> WeaponMesh;
	//TScriptInterface<IIWeapon> WeaponUser; // TScriptInterface has an assignment Operator that takes a UObject* and internally calls Owner->GetInterfaceAddress(UWeaponUser::StaticClass())
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
	float CurrentMultiplier_ADS = 1.f;
	
	FFloatRange OutPutRange_TM;
	
	float CachedDecayThreshold = 0;
	const float /*Cached*/MultiplierNearlyEqualThreshold = 5.f; // this 
	
	float FireIntervalSeconds =0;//calculated once in EquipWeapon as 60.0/sec also in CanFire fucntion when implmented 
	/*bool  bMinSpread=false;
	bool  bMinMultipliers=false;*/
	
	
	
	bool IsSpreadAtMinimum(float DeltaTime);
	bool AreMultipliersAtMinimum(float DeltaTime);
	
	
	

public:
	
};
