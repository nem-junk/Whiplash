#include "WInteractionTInterface.h"

bool UInteractionTInterfaceBFL::SetInteractionTransform(UObject* InteractionTransformObject,
	const FTransform& InteractionTransform)
{
	if (IsValid(InteractionTransformObject) && InteractionTransformObject->Implements<UWInteractionTInterface>())
	{
		IWInteractionTInterface::Execute_SetInteractionTransform(InteractionTransformObject,InteractionTransform);
		return true;
	}
	return false;
}

bool UInteractionTInterfaceBFL::GetInteractionTransform(UObject* InteractionTransformObject,
	FTransform& OutInteractionTransform)
{
	if (IsValid(InteractionTransformObject) && InteractionTransformObject->Implements<UWInteractionTInterface>())
	{
		IWInteractionTInterface::Execute_GetInteractionTransform(InteractionTransformObject,OutInteractionTransform);
		return true;
	}
	return false;
}
