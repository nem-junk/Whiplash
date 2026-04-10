#include "Weapons/WWeaponDA.h"

float UWWeaponDA::GetDamageMultiplier(float Distance) const
{
	const FRichCurve* RichCurve = DistanceDamageFallOff.GetRichCurveConst();
	if (RichCurve)
	{
		return RichCurve->Eval(Distance,1);
	}
	return 1;
}
