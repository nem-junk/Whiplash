#pragma once

#include "CoreMinimal.h"
#include "WAbility.h"
#include "WAbility_WeaponFire.generated.h"
class UWWeaponStateComponent;
UCLASS()
class WHIPLASH_API UWAbility_WeaponFire : public UWAbility
{
	GENERATED_BODY()
	UWAbility_WeaponFire();
public:
	UFUNCTION(BlueprintNativeEvent)
	virtual bool CanStart_Implementation(AActor* Instigator) override;
	UFUNCTION(BlueprintNativeEvent)
	virtual void Start_Implementation(AActor* Instigator) override;
	UFUNCTION(BlueprintNativeEvent)
	virtual void StopAbility_Implementation(AActor* Instigator) override;
private:
	
	void FireOnce();
	FVector GetAimTargetPoint() const;
	FRotator GetSpreadRotation() const;
	
	
	TObjectPtr<UWWeaponStateComponent> CachedWeaponState;
	FTimerHandle FireLoopTimerHandle;
	bool bIsFirstShot = true;
	
};
