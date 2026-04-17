#pragma once

#include "CoreMinimal.h"
#include "WAbility.h"
#include "WAbility_WeaponFire.generated.h"
class IIWeapon;
class UWWeaponStateComponent;
UCLASS()
class WHIPLASH_API UWAbility_WeaponFire : public UWAbility
{
	GENERATED_BODY()
	UWAbility_WeaponFire();
public:
	
	virtual bool CanStart_Implementation(AActor* Instigator) override;
	
	virtual void StartAbility_Implementation(AActor* Instigator) override;
	
	virtual void StopAbility_Implementation(AActor* Instigator) override;
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="WeaponSocketName")
	FName MuzzleSocketName = FName("Muzzle");
private:
	void FireOnce();
	FVector GetAimTargetPoint() const;
	FRotator GetSpreadRotation() const;
	
	UPROPERTY()
	TObjectPtr<UWWeaponStateComponent> CachedWeaponState;
	FTimerHandle FireLoopTimerHandle;
	bool bIsFirstShot = true;
	/*UPROPERTY()
	TScriptInterface<IIWeapon> WeaponOwner;*/
	


};
