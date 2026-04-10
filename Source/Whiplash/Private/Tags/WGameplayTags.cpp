#include "Tags/WGameplayTags.h"

namespace WhiplashTags
{
	// States
	UE_DEFINE_GAMEPLAY_TAG(State_Crouching, "State.Crouching");
	UE_DEFINE_GAMEPLAY_TAG(State_Sprinting, "State.Sprinting");
	UE_DEFINE_GAMEPLAY_TAG(State_WallFlattened, "State.WallFlattened");
	UE_DEFINE_GAMEPLAY_TAG(State_Peeking, "State.Peeking");
	UE_DEFINE_GAMEPLAY_TAG(State_HoldingBreath, "State.HoldingBreath");
	UE_DEFINE_GAMEPLAY_TAG(State_Stunned, "State.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, "State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(State_ADS, "State.ADS");
	UE_DEFINE_GAMEPLAY_TAG(State_Firing, "State.Firing");
	UE_DEFINE_GAMEPLAY_TAG(State_WeaponEquipped,"State.WeaponEquipped");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Pistol,"Weapon.Pistol");

	// Abilities
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action, "Ability.Action");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Shoot, "Ability.Action.Shoot");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Reload, "Ability.Action.Reload");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Melee, "Ability.Action.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Dodge, "Ability.Action.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Crouch, "Ability.Action.Crouch");

	// Detection
	UE_DEFINE_GAMEPLAY_TAG(Detection_Sound, "Detection.Sound");
	//Modifiers
	UE_DEFINE_GAMEPLAY_TAG(Modifier_Stamina_Drain, "Modifier.Stamina.Drain");
	UE_DEFINE_GAMEPLAY_TAG(Modifier_Stamina_Regen, "Modifier.Stamina.Regen");
}