#pragma once

#include "CoreMinimal.h"
#include "WAbility.h"
#include "WAbility_WeaponEquip.generated.h"

class AWCharacter;
class UWWeaponDA;
class UWWeaponStateComponent;
UCLASS()
class WHIPLASH_API UWAbility_WeaponEquip : public UWAbility
{
	GENERATED_BODY()
	UWAbility_WeaponEquip();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UWWeaponDA> WeaponToEquip;
public:

	virtual void StartAbility_Implementation(AActor* Instigator) override;

	virtual void StopAbility_Implementation(AActor* Instigator) override;
private:
	UPROPERTY()
	TObjectPtr<UWWeaponStateComponent> CachedWeaponState;
	
	UPROPERTY()
	TObjectPtr<	AWCharacter>Owner;

};
