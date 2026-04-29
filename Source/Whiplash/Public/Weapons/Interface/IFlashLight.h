
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IFlashLight.generated.h"

UINTERFACE()
class UIFlashLight : public UInterface
{
	GENERATED_BODY()
};

class WHIPLASH_API IIFlashLight
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Weapon|FlashLight")
	void ToggleFlashLight();
	virtual void ToggleFlashLight_Implementation() {}
};
