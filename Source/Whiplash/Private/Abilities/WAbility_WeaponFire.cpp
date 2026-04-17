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



UWAbility_WeaponFire::UWAbility_WeaponFire()
{
	AbilityTag = WhiplashTags::Ability_Action_Shoot;
	//BlockedTags = WhiplashTags::Ability_Action_Reload 
	BlockedTags.AddTag(WhiplashTags::State_Dead);
	BlockedTags.AddTag(WhiplashTags::State_Stunned);
	BlockedTags.AddTag(WhiplashTags::State_Reloading);
	
}

bool UWAbility_WeaponFire::CanStart_Implementation_Implementation(AActor* Instigator)
{
	if (!Super::CanStart_Implementation(Instigator)) return false;

}
