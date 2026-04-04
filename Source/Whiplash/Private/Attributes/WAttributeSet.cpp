
#include "Attributes/WAttributeSet.h"


UWorld* UWAttributeSet::GetWorld() const
{
	UObject* Outer = GetOuter();
	check(Outer);
	return Outer->GetWorld();
}
