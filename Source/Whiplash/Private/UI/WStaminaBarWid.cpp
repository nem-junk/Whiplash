
#include "UI/WStaminaBarWid.h"

void UWStaminaBarWid::UpdateStaminaBar(float CurrentStamina, float MaxStamina)
{
	if (!StaminaBar || !StaminaLost) return;
	float Percentage = FMath::Clamp(CurrentStamina / MaxStamina, 0.0f, 1.0f);
	StaminaBar->SetPercent(Percentage);
}
