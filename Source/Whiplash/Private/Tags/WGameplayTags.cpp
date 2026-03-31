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

	// Abilities
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action, "Ability.Action");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Shoot, "Ability.Action.Shoot");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Reload, "Ability.Action.Reload");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Melee, "Ability.Action.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Dodge, "Ability.Action.Dodge");

	// Detection
	UE_DEFINE_GAMEPLAY_TAG(Detection_Sound, "Detection.Sound");
}