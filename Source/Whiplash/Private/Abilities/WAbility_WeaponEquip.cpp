// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/WAbility_WeaponEquip.h"

#include "Tags/WGameplayTags.h"
#include "Weapons/WWeaponDA.h"
#include "Weapons//WWeaponStateComponent.h"
#include "WCharacter.h"
#include "Tags/WTagComponent.h"
#include "Tags/WGameplayTags.h"
#include "Component/WAbilityComponent.h"

UWAbility_WeaponEquip::UWAbility_WeaponEquip()
{
	BlockedTags.AddTag(WhiplashTags::State_Dead);
	bAutoStart = true;
}

void UWAbility_WeaponEquip::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);
	CachedWeaponState = AbilityComponent->GetOwner()->FindComponentByClass<UWWeaponStateComponent>();
	if (!CachedWeaponState || !WeaponToEquip)
	{
		WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("CachedWeaponState is null in StartAbility_Implementation in WeaponEquip or WeaponToEquip is null"));
		return;
	}
	CachedWeaponState->EquipWeapon(WeaponToEquip);
	if (TagComponent)
	{
		TagComponent->AddTags(WhiplashTags::State_WeaponEquipped);
	}
	if (WeaponToEquip->Mesh.EquipMontage)
	{
		Owner = Cast<AWCharacter>(OwnerPawn);
		if (!Owner)
		{
			WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("Owner is null in WeaponEquip"));
			return;
		}
		Owner->PlayAnimMontage(WeaponToEquip->Mesh.EquipMontage);
	}
	
	
	
}

void UWAbility_WeaponEquip::StopAbility_Implementation(AActor* Instigator)
{
	if (WeaponToEquip and WeaponToEquip->Mesh.UnEquipMontage)
	{
		if (!Owner)
		{
			WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("Owner is null in StopAbility_Implementation"));
			return;
		}
		Owner->PlayAnimMontage(WeaponToEquip->Mesh.UnEquipMontage);
		TagComponent->RemoveTags(WhiplashTags::State_WeaponEquipped);
		CachedWeaponState = nullptr;
	}
	Super::StopAbility_Implementation(Instigator);
}
